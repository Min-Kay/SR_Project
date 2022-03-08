#include "stdafx.h"
#include "Boss.h"

#include "Texture.h"
#include "Transform.h"
#include "Renderer.h"
#include "VIBuffer_Cube.h"
#include "BoxCollider.h"
#include "GameInstance.h"

#include "Arm.h"
#include "Impact.h"
#include "Player.h"
#include "Texture.h"
#include "Shader.h"

#include "AttackRange.h"
#include "Missile.h"
#include "Targeting.h"

#include "Shield.h"
#include "Shield_Effect.h"
#include "UI.h"

CBoss::CBoss(LPDIRECT3DDEVICE9 m_pGraphic_Device)
	:CEnemy(m_pGraphic_Device)
{
}

CBoss::CBoss(const CBoss& rhs)
	:CEnemy(rhs)
	,m_pTransform(rhs.m_pTransform)
	, m_pOnlyRotation(rhs.m_pOnlyRotation)
	,m_pTexture(rhs.m_pTexture)
	,m_pRenderer(rhs.m_pRenderer)
	,m_pCollider(rhs.m_pCollider)
	,m_pBuffer(rhs.m_pBuffer)
	,m_pShader(rhs.m_pShader)
{
	Safe_AddRef(m_pOnlyRotation);
	Safe_AddRef(m_pTransform);
	Safe_AddRef(m_pTexture);
	Safe_AddRef(m_pRenderer);
	Safe_AddRef(m_pCollider);
	Safe_AddRef(m_pBuffer);
	Safe_AddRef(m_pShader);

}

HRESULT CBoss::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CBoss::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	if (FAILED(SetUp_UI()))
		return E_FAIL;

	return S_OK;
}

_int CBoss::Tick(_float fTimeDelta)
{
	if (Get_Dead() || fTimeDelta <= 0.f)
		return 0;

	if (0 > __super::Tick(fTimeDelta))
		return -1;

	Set_OnShield(m_Shield->Get_Valid());

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	if(p_instance->Get_Key_Down(DIK_P))
	{
		m_Resizing = false;
		m_Sizing = false;
		m_Reset = false;
		Set_BossState(BOSS_IDLE);
	}

	RELEASE_INSTANCE(CGameInstance);

	if (m_State != BOSS_DIE && Check_HP())
	{
		m_ImageIndex = 5;
		CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
		p_instance->StopSound(CSoundMgr::ENEMY_EFFECT3);
		p_instance->Play_Sound(TEXT("Boss_Die.wav"), CSoundMgr::ENEMY_EFFECT3, 1.f);
		RELEASE_INSTANCE(CGameInstance);
		Set_BossState(BOSS_DIE);
	}

	m_pCollider->Set_Collider();


	Spawn_Shield();

	Shield_Effect(fTimeDelta);

	State_Machine(fTimeDelta);

	if(m_Resizing && m_Sizing)
		Synchronize_Transform();


	Setting_HpUi();

	return 0;
}

_int CBoss::LateTick(_float fTimeDelta)
{
	if (Get_Dead())
		return 0;

	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return 0;
}

HRESULT CBoss::Render()
{
	if (Get_Dead())
		return 0;

	m_pOnlyRotation->Bind_OnShader(m_pShader);

	m_pShader->SetUp_ValueOnShader("g_ColorStack", &g_ControlShader, sizeof(_float));

	m_pTexture->Bind_OnShader(m_pShader, "g_Texture", m_ImageIndex);

	m_pShader->Begin_Shader(SHADER_SETCOLOR_CUBE);
	m_pBuffer->Render();
	m_pShader->End_Shader();

	return S_OK; 
}

HRESULT CBoss::SetUp_Component()
{
	CTransform::TRANSFORMDESC desc;
	desc.fRotationPerSec = 90.f;
	desc.fSpeedPerSec = 5.f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, TEXT("OnlyRotation"), (CComponent**)&m_pOnlyRotation, &desc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransform, &desc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLLIDER, COM_COLLIDER, (CComponent**)&m_pCollider)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRenderer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_CUBE, COM_BUFFER, (CComponent**)&m_pBuffer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss"), COM_TEXTURE, (CComponent**)&m_pTexture)))
		return E_FAIL;


	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_SHADER_CUBE, COM_SHADER, (CComponent**)&m_pShader)))
		return E_FAIL;

	_float3 vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
	_float3 vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	D3DXVec3Normalize(&vRight, &vRight);


	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);

	m_pPlayer = static_cast<CPlayer*>(p_instance->Get_GameObject_End(g_CurrLevel, TEXT("Layer_Player")));
	m_pPlayerTr = static_cast<CTransform*>(m_pPlayer->Get_Component(COM_TRANSFORM));

	if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("Arm_Left"), TEXT("Prototype_GameObject_Arm"))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_LeftArm = static_cast<CArm*>(static_cast<CEnemy*>((p_instance->Get_GameObject_End(g_CurrLevel, TEXT("Arm_Left")))));
	m_LeftArm->Set_Parent(this);
	m_LeftArm->Set_Position(vPos - vRight * 15.f);
	m_LeftArm->Set_Player(m_pPlayer);
	m_LeftArm->Set_ArmPos(CArm::ARMPOS_LEFT);
	m_LeftArmTr = static_cast<CTransform*>(m_LeftArm->Get_Component(COM_TRANSFORM));


	if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("Arm_Right"), TEXT("Prototype_GameObject_Arm"))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_RightArm = static_cast<CArm*>(static_cast<CEnemy*>((p_instance->Get_GameObject_End(g_CurrLevel, TEXT("Arm_Right")))));
	m_RightArm->Set_Parent(this);
	m_RightArm->Set_Position(vPos + vRight * 15.f);
	m_RightArm->Set_Player(m_pPlayer);
	m_RightArm->Set_ArmPos(CArm::ARMPOS_RIGHT);
	m_RightArmTr = static_cast<CTransform*>(m_RightArm->Get_Component(COM_TRANSFORM));


	Set_Type(OBJ_ENEMY);

	m_pCollider->Set_ParentInfo(this);
	m_pCollider->Set_CollStyle(CCollider::COLLSTYLE_ENTER);
	m_pCollider->Set_State(CBoxCollider::COLL_SIZE, m_vScale);

	m_pTransform->Scaled(m_vScale);
	m_pOnlyRotation->Scaled(m_vScale);

	//
	if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("Layer_AttackRange"), TEXT("Prototype_GameObject_AttackRange"))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}
	m_pAttackRange = static_cast<CAttackRange*>(p_instance->Get_GameObject_End(g_CurrLevel, TEXT("Layer_AttackRange")));
	m_RangeTrans = static_cast<CTransform*>(m_pAttackRange->Get_Component(COM_TRANSFORM));
	m_pAttackRange->Set_Valid(false);

	if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("Shield"), TEXT("Prototype_GameObject_Shield"))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_Shield = static_cast<CShield*>(p_instance->Get_GameObject_End(g_CurrLevel, TEXT("Shield")));
	m_Shield->Set_ParentTransform(m_pOnlyRotation);
	m_Shield->Set_Valid(false);
	m_Shield->Set_Parent(this);

	for(_uint i = 0 ; i < 60 ; ++i)
	{
		if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("Shield_Effect"), TEXT("Prototype_GameObject_Shield_Effect"))))
		{
			RELEASE_INSTANCE(CGameInstance);
			return E_FAIL;
		}

		CShield_Effect* se = static_cast<CShield_Effect*>(p_instance->Get_GameObject_End(g_CurrLevel, TEXT("Shield_Effect")));
		se->Set_Valid(false);

		se->Set_TurnType(i >= 20 ? true : false, i >= 40 ? true : false );
		se->Set_Parent(m_pOnlyRotation);
		m_shield_effects_.push_back(se);
	}

	p_instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_OBJ, m_pCollider);

	p_instance->PlayBGM(TEXT("Boss_Stage.mp3"));

	p_instance->Play_Sound(TEXT("Boss_Sound.wav"), CSoundMgr::ENEMY_EFFECT1, 1.0f);

	RELEASE_INSTANCE(CGameInstance);

	if (!m_pPlayer || !m_RightArm || !m_LeftArm)
		return E_FAIL;
	
	m_Hp = m_InitHp;
	m_Damage = 10;
	m_EnemyType = ENEMY_BOSS;
	m_CanPortal = false;
	m_Phase = BOSS_PHASEONE;
	return S_OK; 

}

HRESULT CBoss::SetUp_UI()
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);



	/* Player_hit_UI*/
	CUI::UIDESC BossUI_Black;
	ZeroMemory(&BossUI_Black, sizeof(BossUI_Black));
	BossUI_Black.WinCX = g_iWinCX;
	BossUI_Black.WinCY = g_iWinCY;

	BossUI_Black.Layer = 2;
	BossUI_Black.FrameCount = 0;
	BossUI_Black.Alpha = CUI::ALPHA_BLEND;
	BossUI_Black.PosX = g_iWinCX / 1.92f;
	BossUI_Black.PosY = g_iWinCY / 14.f;
	BossUI_Black.SizeX = g_iWinCX / 1.6f;
	BossUI_Black.SizeY = g_iWinCY / 18.f;
	BossUI_Black.AnimateSpeed = 30.f;
	BossUI_Black.Style = CUI::STYLE_FIX;
	BossUI_Black.Texture = TEXT("Prototype_Component_BossHP_Black");
	BossUI_Black.Shader_Style = SHADER_SETCOLOR_BLEND;
	BossUI_Black.Shader_Control = &g_ControlShader;



	if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("BossUI_Block"), PROTO_UI, &BossUI_Black)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_BossUI_Black = static_cast<CUI*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("BossUI_Block")));


	CUI::UIDESC BossUI_HP;
	ZeroMemory(&BossUI_HP, sizeof(BossUI_HP));
	BossUI_HP.WinCX = g_iWinCX;
	BossUI_HP.WinCY = g_iWinCY;

	BossUI_HP.Layer = 2;
	BossUI_HP.FrameCount = 1;
	BossUI_HP.Alpha = CUI::ALPHA_DEFAULT;
	BossUI_HP.PosX = g_iWinCX / 1.92f;
	BossUI_HP.PosY = g_iWinCY / 14.f;
	BossUI_HP.SizeX = g_iWinCX / 1.6f;
	BossUI_HP.SizeY = g_iWinCY / 18.f;
	BossUI_HP.AnimateSpeed = 30.f;
	BossUI_HP.Style = CUI::STYLE_FIX;
	BossUI_HP.Texture = TEXT("Prototype_Component_BossHP");
	BossUI_HP.Shader_Style = SHADER_SETCOLOR_BLEND;
	BossUI_HP.Shader_Control = &g_ControlShader;


	if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("BossUI_HP"), PROTO_UI, &BossUI_HP)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}


	m_BossUI_HP = static_cast<CUI*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("BossUI_HP")));

	m_fHpbarPos = g_iWinCX / 1.92f;
	m_beforeHp = Get_HP();



	CUI::UIDESC BossUI_ShieldHP;
	ZeroMemory(&BossUI_ShieldHP, sizeof(BossUI_ShieldHP));
	BossUI_ShieldHP.WinCX = g_iWinCX;
	BossUI_ShieldHP.WinCY = g_iWinCY;

	BossUI_ShieldHP.Layer = 1;
	BossUI_ShieldHP.FrameCount = 2;
	BossUI_ShieldHP.Alpha = CUI::ALPHA_TEST;
	BossUI_ShieldHP.PosX = g_iWinCX / 1.91f;
	BossUI_ShieldHP.PosY = g_iWinCY / 48.f;
	BossUI_ShieldHP.SizeX = g_iWinCX / 2.8f; // 457
	BossUI_ShieldHP.SizeY = g_iWinCY / 36.f;
	BossUI_ShieldHP.AnimateSpeed = 30.f;
	BossUI_ShieldHP.Style = CUI::STYLE_FIX;
	BossUI_ShieldHP.Texture = TEXT("Prototype_Component_BossShield");
	BossUI_ShieldHP.Shader_Style = SHADER_SETCOLOR_BLEND;
	BossUI_ShieldHP.Shader_Control = &g_ControlShader;

	if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("BossUI_ShieldHP"), PROTO_UI, &BossUI_ShieldHP)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_BossUI_ShieldHP = static_cast<CUI*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("BossUI_ShieldHP")));


	m_beforeShiledHP = Get_ShieldHp();
	m_BossUI_ShieldHP->Set_CurrFrameIndex(0);
	m_ShiledHpPos = g_iWinCX / 1.91f;

	/* Player_hit_UI*/
	CUI::UIDESC BossUI_HPBaar;
	ZeroMemory(&BossUI_HPBaar, sizeof(BossUI_HPBaar));
	BossUI_HPBaar.WinCX = g_iWinCX;
	BossUI_HPBaar.WinCY = g_iWinCY;

	BossUI_HPBaar.Layer = 2;
	BossUI_HPBaar.FrameCount = 0;
	BossUI_HPBaar.Alpha = CUI::ALPHA_BLEND;
	BossUI_HPBaar.PosX = g_iWinCX * 0.5f;
	BossUI_HPBaar.PosY = g_iWinCY * 0.06f;
	BossUI_HPBaar.SizeX = g_iWinCY / 14.2f;
	BossUI_HPBaar.SizeY = g_iWinCY / 9.f;
	BossUI_HPBaar.AnimateSpeed = 30.f;
	BossUI_HPBaar.Style = CUI::STYLE_FIX;
	BossUI_HPBaar.Texture = TEXT("Prototype_Component_BossHPBar");
	BossUI_HPBaar.Shader_Style = SHADER_SETCOLOR_BLEND;
	BossUI_HPBaar.Shader_Control = &g_ControlShader;

	if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("BossUI_HPBaar"), PROTO_UI, &BossUI_HPBaar)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_BossUI_HpBar = static_cast<CUI*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("BossUI_HPBaar")));


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CBoss::Setting_HpUi()
{
	m_uChageHp = Get_HP();

	if (m_beforeHp != m_uChageHp)
	{
		if (m_beforeHp <= 0)
			m_beforeHp = Get_HP();

		_int hp = m_beforeHp - m_uChageHp;
		m_BossUI_HP->Set_Size(m_uChageHp * 8.f, g_iWinCY / 18.f);
		m_BossUI_HP->Set_Pos((m_fHpbarPos -= hp * 4.f), g_iWinCY / 14.f);
		m_beforeHp = m_uChageHp;


	}

	if (m_OnShield == true)
	{

		m_uChageShiledHp = Get_ShieldHp();
		m_BossUI_ShieldHP->Set_CurrFrameIndex(1);

		if (m_beforeShiledHP != m_uChageShiledHp)
		{
			_int hp = m_beforeShiledHP - m_uChageShiledHp;
			m_BossUI_ShieldHP->Set_Size(m_uChageShiledHp * 4.5f, g_iWinCY / 36.f);
			m_BossUI_ShieldHP->Set_Pos((m_ShiledHpPos -= hp * 4.5f * 0.5), g_iWinCY / 48.f);
			m_beforeShiledHP = m_uChageShiledHp;
		}
	}
	else
		m_BossUI_ShieldHP->Set_CurrFrameIndex(0);




}
void CBoss::Set_InitPos(_float3 _pos)
{
	m_InitPos = _pos;
}

void CBoss::Add_HP(_int _add)
{
	if(!m_Shield->Get_Valid())
		__super::Add_HP(_add);
}

const _int& CBoss::Get_InitHP() const
{
	return m_InitHp;
}

void CBoss::Set_OnShield(_bool _bool)
{
	m_OnShield = _bool;
}

const _bool CBoss::Get_OnShield() const
{
	return m_OnShield;
}

void CBoss::Add_ShieldHp(_int _add)
{
	if (!m_Shield->Get_Valid())
		return;

	m_Shield->Add_ShieldHp(_add);
}

const _int CBoss::Get_ShieldHp()
{
	return m_Shield->Get_HP();
}

void CBoss::Set_Grogy()
{
	Set_BossState(BOSS_GROGY);
}

_bool CBoss::InitArmPosition(_float fTimeDelta, _bool _left, _bool _right)
{
	if (!m_LeftArmTr || !m_RightArmTr)
		return false;

	if(!initPos)
	{
		_float3 vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
		_float3 vUp = m_pTransform->Get_State(CTransform::STATE_UP);
		_float3 vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);

		D3DXVec3Normalize(&vUp, &vUp);
		D3DXVec3Normalize(&vRight, &vRight);


		if(_left)
		{
			m_LeftTimer = 0.f;
			Set_ArmPos(ARM_LEFT,m_LeftArmTr->Get_State(CTransform::STATE_POSITION), vPos + vUp * m_fUpMidPos - vRight * m_fRightMidPos, vPos + vUp * m_fUpPos - vRight * m_fRightPos);
			m_LeftArm->Set_Rolling(true, _float3(1.f, 1.f, 0.f));
			
		}
		if(_right)
		{
			m_RightTimer = 0.f;
			Set_ArmPos(ARM_RIGHT, m_RightArmTr->Get_State(CTransform::STATE_POSITION), vPos + vUp * m_fUpMidPos + vRight * m_fRightMidPos, vPos + vUp * m_fUpPos + vRight * m_fRightPos);
			m_RightArm->Set_Rolling(true, _float3(1.f, -1.f, 0.f));
		}


		idlePos = false;
		initPos = true;
	}

	if (!idlePos)
	{
		_bool leftReach, rightReadch;
		if (_left)
		{
			leftReach = Move_By_Bazier(ARM_LEFT, fTimeDelta);
		}

		if(_right)
		{
			rightReadch = Move_By_Bazier(ARM_RIGHT, fTimeDelta);
		}

		if(_left && _right && leftReach && rightReadch)
		{
			m_LeftTimer = 0.f;
			m_RightTimer = 0.f;
			initPos = false;
			idlePos = true;
			m_init = false;
			return true;

		}

		if(_left && leftReach)
		{
			m_LeftTimer = 0.f;
			initPos = false;
			idlePos = true;
			m_init = false;
			return true;
		}

		if (_right && rightReadch)
		{
			m_RightTimer = 0.f;
			initPos = false;
			idlePos = true;
			m_init = false;
			return true;
		}
	}
	return false;
}

void CBoss::Synchronize_Transform()
{
	m_pOnlyRotation->Set_State(CTransform::STATE_POSITION, m_pTransform->Get_State(CTransform::STATE_POSITION));

	_float3 vRight, vUp, vLook, vScale;

	vRight = m_pOnlyRotation->Get_State(CTransform::STATE_RIGHT);
	vUp = m_pTransform->Get_State(CTransform::STATE_UP);

	vScale = m_pTransform->Get_Scale();

	vLook = *D3DXVec3Cross(&vLook, &vRight, &vUp);
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vLook, &vLook);

	if (0 >= D3DXVec3Length(&vLook))
		return;

	m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
	m_pTransform->Set_State(CTransform::STATE_LOOK, vLook * vScale.z);

}

void CBoss::Set_BossState(BOSSSTATE _state)
{
	// 여기에 자기 패턴이나 상태관련 변수 초기화 작성
	m_fTimer = 0.f;

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->StopSound(CSoundMgr::ENEMY_EFFECT2);
	RELEASE_INSTANCE(CGameInstance);

	Init_Idle();
	Init_Attack_Punch();
	Init_Attack_Missile();
	m_State = _state;
	m_OnPattern = false;
	
}

void CBoss::Init_Idle()
{
	m_init = true;
	m_LeftTimer = 0.f;
	m_RightTimer = 0.f;
	initPos = false;
	idlePos = true;

	if (m_State == BOSS_MOVE || m_State == BOSS_IDLE)
		return;

	m_Resizing = false;
	m_Sizing = false;
	m_Reset = false;
}

void CBoss::Sizing_Particles(_float4 _color, _int time, _float _speed)
{
	CImpact::IMPACT Impact1;
	ZeroMemory(&Impact1, sizeof(Impact1));
	Impact1.Pos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	Impact1.Size = _float3(0.4f, 0.4f, 0.4f);
	Impact1.randomPos = 7;
	Impact1.Speed = _speed;
	Impact1.deleteCount = time;//rand() % 5 + 2;
	Impact1.Color = D3DXCOLOR(_color);

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	for (int i = 0; i < 10 ; ++i)
	{
		if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("Impact"), TEXT("Prototype_GameObject_Impact"), &Impact1)))
		{
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
	}
	RELEASE_INSTANCE(CGameInstance);
}

void CBoss::Gravity_Blowing(_float fTimeDelta, _bool _watchPlayer)
{
	if (fTimeDelta <= 0.f)
		return;

	m_fTimer += fTimeDelta;

	m_pTransform->Gravity(0.3f, fTimeDelta);
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	list<CCollision_Manager::COLLPOINT> collList = p_instance->Get_Ray_Collision_List(-m_pTransform->Get_State(CTransform::STATE_UP), m_pTransform->Get_State(CTransform::STATE_POSITION), 100, true);

	if (collList.empty() || collList.size() == 1 )
	{
		RELEASE_INSTANCE(CGameInstance);
		return;
	}


	auto iter = collList.begin();

	for(; iter != collList.end();)
	{
		if (iter->CollObj->Get_Type() != OBJ_STATIC)
			++iter;
		else
			break;
	}

	if (iter == collList.end())
		return; 

	if(_watchPlayer)
		m_pOnlyRotation->LookAt(m_pPlayerTr->Get_State(CTransform::STATE_POSITION));

	m_pTransform->Set_State(CTransform::STATE_POSITION, (*iter).Point + _float3(0.f, 10.f, 0.f) + _float3(0.f, 1.f, 0.f) * sinf(D3DXToDegree(m_fTimer * 0.01f) * 2.f));

	RELEASE_INSTANCE(CGameInstance);
}

void CBoss::Arm_Posing(_float fTimeDelta, _bool _left, _bool _right)
{
	_float3 vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
	_float3 vUp = m_pTransform->Get_State(CTransform::STATE_UP);

	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	if(_left)
	{
		m_LeftArm->Set_Position(vPos - vRight * m_fRightPos - vRight * sinf(D3DXToDegree(m_fTimer * 0.05f)) * 2.f + vUp * m_fUpPos * cosf(D3DXToDegree(m_fTimer * 0.04f)));
		Reset_Arm_Direction(ARM_LEFT);
		
	}

	if(_right)
	{
		m_RightArm->Set_Position(vPos + vRight * m_fRightPos + vRight * cosf(D3DXToDegree(m_fTimer * 0.05f)) * 2.f + vUp * m_fUpPos * cosf(D3DXToDegree(m_fTimer * 0.04f)));
		Reset_Arm_Direction(ARM_RIGHT);
	}

}

void CBoss::Start_Pattern(_tchar* filename)
{
	if (m_OnPattern)
		return;
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->StopSound(CSoundMgr::ENEMY_EFFECT2);
	p_instance->Play_Sound(filename, CSoundMgr::ENEMY_EFFECT2, 1.f); 
	RELEASE_INSTANCE(CGameInstance);
	m_OnPattern = true;
}

void CBoss::Spawn_Shield()
{
	if (m_SpawnShield)
		return;

	if (m_Hp < m_InitHp * 0.5f && !m_SpawnShield)
	{
		Sizing_Particles(_float4(1.f, 0.f, 0.55f, 1.f), 10, 30.f);
		m_Shield->Set_Valid(true);
		m_SpawnShield = true;

		_float timer = 0.f;
		for(auto& se : m_shield_effects_)
		{
			se->Set_CoolTime(timer);
			timer += 0.15f;
		}

		CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
		for(_uint i = 0; i < 10; ++i)
		{
			if(FAILED(p_instance->Add_GameObject(g_CurrLevel,TEXT("Minimy"),TEXT("Prototype_GameObject_Minimy"))))
			{
				RELEASE_INSTANCE(CGameInstance);
				return;
			}

			CTransform* tr = static_cast<CTransform*>(p_instance->Get_GameObject_End(g_CurrLevel, TEXT("Minimy"))->Get_Component(COM_TRANSFORM));

			_float3 vPos = m_InitPos;
			vPos.x += rand() % 2 == 0 ? rand() % 20: -(rand() % 20);
			vPos.z += rand() % 2 == 0 ? rand() % 20 : -(rand() % 20);

			tr->Set_State(CTransform::STATE_POSITION, vPos);
		}
		RELEASE_INSTANCE(CGameInstance); 
	}
}

void CBoss::Shield_Effect(_float fTimeDelta)
{
	if (!m_OnShield)
		return;

	m_ShieldTimer += fTimeDelta;
	for (auto& se : m_shield_effects_)
	{
		se->Set_SpreadRange(2.f * sinf(m_ShieldTimer) + 7.f);
	}

}

void CBoss::Init_Attack_Punch()
{
	m_bCalled = false;
	m_Hand = false;
	m_RightArm->Set_State(CArm::ARM_IDLE);
	m_LeftArm->Set_State(CArm::ARM_IDLE);
	m_pAttackRange->Set_Valid(false);
	m_CurrPunchCount = 0;
}


void CBoss::Resizing(_float fTimeDelta)
{
	if(m_fTimer == 0.f)
	{
		m_Invincible = true;
		CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
		p_instance->Play_Sound(TEXT("Resizing.wav"), CSoundMgr::ENEMY_EFFECT3, 1.f);
		RELEASE_INSTANCE(CGameInstance);
	}


	m_fTimer += fTimeDelta * 1.8f;

	m_pOnlyRotation->Turn(m_SizingAxis, fTimeDelta * 0.3f);

	m_pOnlyRotation->Scaled(_float3(m_vScale.x - m_fTimer, m_vScale.y - m_fTimer, m_vScale.z - m_fTimer));

	Sizing_Particles(_float4(0.f,0.f,0.f,1.f), 2, 20.f);

	if(m_pOnlyRotation->Get_Scale().x <= 0.1f)
	{
		m_pOnlyRotation->Set_State(CTransform::STATE_POSITION, m_InitPos);
		m_pTransform->Set_State(CTransform::STATE_POSITION, m_InitPos);
		m_Resizing = true;
		m_fTimer = 0.f;
	}

}

void CBoss::Sizing(_float fTimeDelta)
{
	m_fTimer += fTimeDelta * 1.8f;

	m_pOnlyRotation->Turn(-m_SizingAxis, fTimeDelta * 0.3f);

	m_pOnlyRotation->Scaled(_float3( m_fTimer,  m_fTimer, m_fTimer));

	Sizing_Particles(_float4(0.f, 0.f, 0.f, 1.f), 2, 20.f);

	if (m_fTimer >= m_vScale.x)
	{
		m_pOnlyRotation->Set_WorldMatrix(m_pTransform->Get_WorldMatrix());

		m_Sizing = true;
		m_fTimer = 0.f;
	}
}

void CBoss::Reset_Arm_Direction(ARM _arm)
{
	_float3 vRight, vLook, vUp , vScale;
	vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
	vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
	vUp = m_pTransform->Get_State(CTransform::STATE_UP);

	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vUp, &vUp);

	switch (_arm)
	{
	case ARM_LEFT:
		vScale = m_LeftArmTr->Get_Scale();

		m_LeftArmTr->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
		m_LeftArmTr->Set_State(CTransform::STATE_LOOK, vLook * vScale.y);
		m_LeftArmTr->Set_State(CTransform::STATE_UP, vUp * vScale.z);
		m_LeftArm->Synchronize_Direction();
		m_LeftArm->Set_Rolling(false);
		break;
	case ARM_RIGHT:
		vScale = m_RightArmTr->Get_Scale();

		m_RightArmTr->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
		m_RightArmTr->Set_State(CTransform::STATE_LOOK, vLook * vScale.y);
		m_RightArmTr->Set_State(CTransform::STATE_UP, vUp * vScale.z);
		m_RightArm->Synchronize_Direction();
		m_RightArm->Set_Rolling(false);
		break;
	}
}

void CBoss::Set_ArmPos(ARM _arm, _float3 _start, _float3 _mid, _float3 _end)
{
	switch (_arm)
	{
	case ARM_LEFT:
		leftArmBazier[0] = _start;
		leftArmBazier[1] = _mid;
		leftArmBazier[2] = _end;
		break;
	case ARM_RIGHT:
		rightArmBazier[0] = _start;
		rightArmBazier[1] = _mid;
		rightArmBazier[2] = _end;
		break;
	}
}

void CBoss::Blowing(_float fTimeDelta)
{
	Gravity_Blowing(fTimeDelta,true);

	Arm_Posing(fTimeDelta);

}

void CBoss::Randomize_Pattern(_float fTimeDelta)
{
	if( m_Reset && (m_State == BOSS_IDLE || m_State == BOSS_MOVE))
	{
		m_AttPatternTimer += fTimeDelta;

		switch (m_Phase)
		{
		case BOSS_PHASEONE:
			if (m_AttPatternTimer > 5.f)
			{
				m_AttState = m_AttState == BOSSATT_PUNCH ? BOSSATT_MISSILE : BOSSATT_PUNCH;
				m_AttPatternTimer = 0.f;
				Set_BossState(BOSS_ATTACK);
			}
			break;
		case BOSS_PHASETWO:
			if (m_AttPatternTimer > 3.f)
			{
				m_RageState = m_RageState == BOSSRAGE_TAEBO ? BOSSRAGE_LASER : BOSSRAGE_TAEBO;
				m_AttPatternTimer = 0.f;
				Set_BossState(BOSS_ATTACK);
			}
			break;
		}
	}
}


_bool CBoss::Move_By_Bazier(ARM _arm , _float fTimeDelta)
{
	switch(_arm)
	{
	case ARM_LEFT:
		if ( m_LeftTimer >= 1.f ||  1.5f >= D3DXVec3Length(&(m_LeftArmTr->Get_State(CTransform::STATE_POSITION) - leftArmBazier[2])))
		{
			m_LeftTimer = 0.f;
			Reset_Arm_Direction(ARM_LEFT);
			return true;
		}

		m_LeftTimer += fTimeDelta * 0.7f;
		m_LeftArmTr->Set_State(CTransform::STATE_POSITION, powf(1 - m_LeftTimer, 2) * leftArmBazier[0] + 2 * m_LeftTimer * (1 - m_LeftTimer) * leftArmBazier[1] + powf(m_LeftTimer, 2)* leftArmBazier[2]);
		break;
	case ARM_RIGHT:
		if (m_RightTimer >= 1.f ||1.5f >= D3DXVec3Length(&(m_RightArmTr->Get_State(CTransform::STATE_POSITION) - rightArmBazier[2])))
		{
			m_RightTimer = 0.f;

			Reset_Arm_Direction(ARM_RIGHT);

			return true;
		}

		m_RightTimer += fTimeDelta * 0.7f;
		m_RightArmTr->Set_State(CTransform::STATE_POSITION, powf(1 - m_RightTimer, 2) * rightArmBazier[0] + 2 * m_RightTimer * (1 - m_RightTimer) * rightArmBazier[1] + powf(m_RightTimer, 2) * rightArmBazier[2]);
		break;
	}
	return false;

}

HRESULT CBoss::Init_Attack_Missile()
{
	m_bMissile = false;
	m_btargetCollider = true;
	m_fFireFrame = 0.f;
	m_fFireCount = 0;
	m_fWaiting = 0.f;
	m_LaunchTimer = 0.f;
	return S_OK;
}

void CBoss::State_Machine(_float fTimeDelta)
{
	Randomize_Pattern(fTimeDelta);
	switch (m_State)
	{
	case BOSS_IDLE:
		Idle(fTimeDelta);
		break;
	case BOSS_MOVE:
		Move(fTimeDelta);
		break;
	case BOSS_ATTACK:
		Phase(fTimeDelta);
		break;
	case BOSS_GROGY:
		Grogy(fTimeDelta);
		break;
	case BOSS_PHASECHANGE:
		break; 
	case BOSS_DIE:
		Die(fTimeDelta);
		break;
	}
}

void CBoss::Idle(_float fTimeDelta)
{
	m_ImageIndex = m_Phase == BOSS_PHASEONE ? 0 : 3;
	// 멍때리기
	if(!m_Resizing)
	{
		Resizing(fTimeDelta);
	}
	else if(!m_Sizing)
	{
		Sizing(fTimeDelta);
	}
	else if(!m_Reset)
	{
		
		Gravity_Blowing(fTimeDelta, false);
		
		if(InitArmPosition(fTimeDelta))
		{
			m_Invincible = false;
			m_Reset = true;
			CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
			p_instance->StopSound(CSoundMgr::ENEMY_EFFECT3);
			RELEASE_INSTANCE(CGameInstance);
		}
	}
	else if(m_fMoveLength <= D3DXVec3Length(&(m_pTransform->Get_State(CTransform::STATE_POSITION) - m_pPlayerTr->Get_State(CTransform::STATE_POSITION))))
	{
		Set_BossState(BOSS_MOVE);
	}
	else if (!m_init)
	{
		Blowing(fTimeDelta);
	}
	else
		InitArmPosition(fTimeDelta);

}

void CBoss::Move(_float fTimeDelta)
{
	m_ImageIndex = m_Phase == BOSS_PHASEONE ? 0 : 3;

	_float3 vOnlyRight = m_pOnlyRotation->Get_State(CTransform::STATE_RIGHT);

	_float3 vUp = m_pTransform->Get_State(CTransform::STATE_UP);
	_float3 vScale = m_pTransform->Get_Scale();

	D3DXVec3Normalize(&vOnlyRight, &vOnlyRight);
	D3DXVec3Normalize(&vUp, &vUp);

	_float3 vLook = *D3DXVec3Cross(&vLook, &vOnlyRight,&vUp);

	m_pTransform->Set_State(CTransform::STATE_RIGHT, vOnlyRight *	vScale.x);
	m_pTransform->Set_State(CTransform::STATE_LOOK, vLook * vScale.z);
	m_pTransform->Go_Straight(fTimeDelta);

	Gravity_Blowing(fTimeDelta, true);

	Arm_Posing(fTimeDelta);

	if(m_fMoveLength - 3.f > D3DXVec3Length(&(m_pTransform->Get_State(CTransform::STATE_POSITION) - m_pPlayerTr->Get_State(CTransform::STATE_POSITION))))
	{
		Set_BossState(BOSS_IDLE);
	}
}

void CBoss::Phase(_float fTimeDelta)
{
	switch (m_Phase)
	{
	case BOSS_PHASEONE:
		Attack(fTimeDelta);
		break;
	case BOSS_PHASETWO:
		Rage(fTimeDelta);
		break;
	}
}


void CBoss::Attack(_float fTimeDelta)
{
	m_ImageIndex = 1;
	// 1페이즈 패턴 구현
	switch (m_AttState)
	{
	case BOSSATT_PUNCH:
		Attack_Punch(fTimeDelta);
		break;
	case BOSSATT_MISSILE:
		Attack_Missile(fTimeDelta);
		break;
	}
}

void CBoss::Rage(_float fTimeDelta)
{
	m_ImageIndex = 4;
}

void CBoss::Grogy(_float fTimeDelta)
{
	m_fTimer += fTimeDelta;

	m_pTransform->Gravity(1.0f, fTimeDelta);

	if (!m_Grogy)
	{
		g_ControlTime = 0.1f;
		g_ControlShader = 0.75f;
		m_Grogy = true;
		m_ImageIndex = 2;


		_float3 vRight, vUp;
		vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
		vUp = m_pTransform->Get_State(CTransform::STATE_UP);

		D3DXVec3Normalize(&vRight, &vRight);
		D3DXVec3Normalize(&vUp, &vUp);


		Set_ArmPos(ARM_LEFT, m_LeftArmTr->Get_State(CTransform::STATE_POSITION), m_LeftArmTr->Get_State(CTransform::STATE_POSITION) -  vRight * 10.f + vUp * 30.f, m_LeftArmTr->Get_State(CTransform::STATE_POSITION) - vRight * 20.f - vUp * 10.f);

		Set_ArmPos(ARM_RIGHT, m_RightArmTr->Get_State(CTransform::STATE_POSITION), m_RightArmTr->Get_State(CTransform::STATE_POSITION) + vRight * 10.f + vUp * 30.f, m_RightArmTr->Get_State(CTransform::STATE_POSITION) + vRight * 20.f - vUp * 10.f);

		m_LeftArm->Set_RollingSpeed(0.4f);
		m_RightArm->Set_RollingSpeed(0.4f);

		m_LeftArm->Set_Rolling(true);
		m_RightArm->Set_Rolling(true);

		m_LeftArm->Set_CanPortal(false);
		m_RightArm->Set_CanPortal(false);


		Start_Pattern(TEXT("Grogy.wav"));
		Sizing_Particles(_float4(1.f, 0.f, 0.55f, 1.f), 4.f, 30.f);

		for(auto iter = m_shield_effects_.begin(); iter != m_shield_effects_.end();)
		{
			(*iter)->Set_Split(true);
			iter = m_shield_effects_.erase(iter);
		}

		return;
	}
	
	if (m_LeftArmTr->Get_OnCollide())
	{
		m_LeftArm->Set_Rolling(false);
		m_LeftArmTr->Gravity(1.f, fTimeDelta);
	}
	else
		Move_By_Bazier(ARM_LEFT, fTimeDelta);

	if (m_RightArmTr->Get_OnCollide())
	{
		m_RightArm->Set_Rolling(false);

		m_RightArmTr->Gravity(1.f, fTimeDelta);
	}
	else
		Move_By_Bazier(ARM_RIGHT, fTimeDelta);


	g_ControlTime += 0.005f;
	g_ControlShader -= 0.005f;

	if (g_ControlTime >= 1.f)
		g_ControlTime = 1.f;
	if (g_ControlShader <= 0.f)
		g_ControlShader = 0.f;

	if(m_fTimer >= 5.f)
	{
		g_ControlTime = 1.f;
		g_ControlShader = 0.f;

		m_LeftArm->Set_CanPortal(true);
		m_RightArm->Set_CanPortal(true);

		Set_BossState(BOSS_IDLE);
		m_Phase = BOSS_PHASETWO;
		m_ImageIndex = 0;
	}
	
}

void CBoss::Die(_float fTimeDelta)
{
	m_LeftArm->Set_CanPortal(false);
	m_RightArm->Set_CanPortal(false);


	m_pTransform->Gravity(0.3f, fTimeDelta);
	m_LeftArmTr->Gravity(0.3f, fTimeDelta);
	m_RightArmTr->Gravity(0.3f, fTimeDelta);

	if(m_pTransform->Get_OnCollide() && m_LeftArmTr->Get_OnCollide() && m_RightArmTr->Get_OnCollide())
	{
		CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
		p_instance->StopSound(CSoundMgr::ENEMY_EFFECT1);
		p_instance->StopSound(CSoundMgr::ENEMY_EFFECT2);
		p_instance->StopSound(CSoundMgr::ENEMY_EFFECT3);
		RELEASE_INSTANCE(CGameInstance);
		static_cast<CBoxCollider*>(m_LeftArm->Get_Component(COM_COLLIDER))->Set_Dead(true);
		m_pCollider->Set_Dead(true);
		static_cast<CBoxCollider*>(m_RightArm->Get_Component(COM_COLLIDER))->Set_Dead(true);
		m_LeftArm->Set_Dead(true);
		m_RightArm->Set_Dead(true);
		m_Shield->Set_Dead(true);
		Set_Dead(true);
	}
}
void CBoss::Attack_Missile(_float fTimeDelta)
{
	// 미사일

	Start_Pattern(TEXT("Boss_AttackAlarm.wav"));
	
	CMissile::ARMMISSLE Armtarget1;
	Armtarget1.FireCount = 8;
	Armtarget1.ArmMissle = CMissile::ARMMISSLE_LEFT;
	Armtarget1.pParent = this;


	Gravity_Blowing(fTimeDelta, true);

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	m_fWaiting += fTimeDelta;
	if (m_fFireCount >= Armtarget1.FireCount)
	{
		p_instance->StopSound(CSoundMgr::ENEMY_EFFECT3); 
		m_RightArm->Set_Rolling(false);
		m_LeftArm->Set_Rolling(false);
		Reset_Arm_Direction(ARM_RIGHT);
		Reset_Arm_Direction(ARM_LEFT);

		if (m_fWaiting >= 5.f)
		{
			Set_BossState(BOSS_IDLE);
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
	}

	if(m_fFireCount > 0 && Armtarget1.FireCount < m_fFireCount)
	{
		m_LaunchTimer += fTimeDelta;
		_float3 vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
		D3DXVec3Normalize(&vLook, &vLook);
		switch (m_CurrLaunchArm)
		{
		case ARM_LEFT:
			m_LeftArmTr->Set_State(CTransform::STATE_POSITION, m_InitLaunchPos[ARM_LEFT] + vLook * sinf(D3DXToRadian(m_LaunchTimer * 120.f)) * 10.f);
			break;
		case ARM_RIGHT:
			m_RightArmTr->Set_State(CTransform::STATE_POSITION, m_InitLaunchPos[ARM_RIGHT] + vLook * sinf(D3DXToRadian(m_LaunchTimer * 120.f)) * 10.f);
			break;
		}
	}
	
	if (m_btargetCollider && m_fFireCount < Armtarget1.FireCount)
	{

		m_fFireFrame += fTimeDelta * 3;
		if (m_fFireFrame >= 1.f)
		{
			m_fFireFrame = 0.f;
			++m_fFireCount;
			switch (m_fFireCount % 2)
			{
			case 0:
				Armtarget1.ArmMissle = CMissile::ARMMISSLE_LEFT;
				Armtarget1.Left = false;
				Armtarget1.Right = true;
				m_LeftArm->Set_Rolling(true, -m_pTransform->Get_State(CTransform::STATE_LOOK));
				m_RightArm->Set_Rolling(false);
				Reset_Arm_Direction(ARM_RIGHT);
				m_CurrLaunchArm = ARM_LEFT;
				m_LaunchTimer = 0.f;
				m_InitLaunchPos[ARM_LEFT] = m_LeftArmTr->Get_State(CTransform::STATE_POSITION);
				p_instance->StopSound(CSoundMgr::ENEMY_EFFECT3);
				p_instance->Play_Sound(rand() % 2 == 0 ? TEXT("Missile_Launch_0.wav") : TEXT("Missile_Launch_1.wav"),CSoundMgr::ENEMY_EFFECT3,1.f );
				break;
			case 1:
				Armtarget1.ArmMissle = CMissile::ATMMISSLE_RIGHT;
				Armtarget1.Left = true;
				Armtarget1.Right = false;
				m_RightArm->Set_Rolling(true, m_pTransform->Get_State(CTransform::STATE_LOOK));
				m_LeftArm->Set_Rolling(false);
				Reset_Arm_Direction(ARM_LEFT);
				m_CurrLaunchArm = ARM_RIGHT;
				m_LaunchTimer = 0.f;
				m_InitLaunchPos[ARM_RIGHT] = m_RightArmTr->Get_State(CTransform::STATE_POSITION);
				p_instance->StopSound(CSoundMgr::ENEMY_EFFECT3);
				p_instance->Play_Sound(rand() % 2 == 0 ? TEXT("Missile_Launch_0.wav") : TEXT("Missile_Launch_1.wav"), CSoundMgr::ENEMY_EFFECT3, 1.f);
				break;
			}

			if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("Missile"), TEXT("Prototype_GameObject_Missile"), &Armtarget1)))
			{
				RELEASE_INSTANCE(CGameInstance);
			}

			m_fWaiting = 0.f;
		}

	}
	RELEASE_INSTANCE(CGameInstance);

}

void CBoss::Attack_Punch(_float fTimeDelta)
{
	// 펀치

	Start_Pattern(TEXT("Boss_AttackAlarm1.wav"));

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	Gravity_Blowing(fTimeDelta, false);

	if (!m_bCalled)
	{
		_float3 left_position = m_LeftArmTr->Get_State(CTransform::STATE_POSITION);
		_float3 right_position = m_RightArmTr->Get_State(CTransform::STATE_POSITION);
		_float3 player_Pos = m_pPlayerTr->Get_State(CTransform::STATE_POSITION);
		_float3 vUp,vRight, vDest;
		vDest = _float3(player_Pos.x, player_Pos.y - 0.3f, player_Pos.z);
		m_RangeTrans->Set_State(CTransform::STATE_POSITION, vDest);
		m_pAttackRange->Set_Valid(true);
		vDest.y += m_LeftArmTr->Get_Scale().y * 0.5f - 1.f;

		if(D3DXVec3Length(&(left_position- player_Pos)) <= D3DXVec3Length(&(right_position - player_Pos)))
		{
			vUp = m_LeftArmTr->Get_State(CTransform::STATE_UP);
			vRight = m_LeftArmTr->Get_State(CTransform::STATE_RIGHT);

			D3DXVec3Normalize(&vUp, &vUp);
			D3DXVec3Normalize(&vRight, &vRight);
			Set_ArmPos(ARM_LEFT, left_position, left_position + vUp * 5.f - vRight * 4.f, vDest);
			m_LeftArm->Set_State(CArm::ARM_ATTACK);
			m_RightArm->Set_State(CArm::ARM_IDLE);
			m_Hand = true;
		}
		else
		{
			vUp = m_LeftArmTr->Get_State(CTransform::STATE_UP);
			vRight = m_LeftArmTr->Get_State(CTransform::STATE_RIGHT);

			D3DXVec3Normalize(&vUp, &vUp);
			D3DXVec3Normalize(&vRight, &vRight);

			Set_ArmPos(ARM_RIGHT, right_position, right_position + vUp * 5.f + vRight * 4.f, vDest);
			m_RightArm->Set_State(CArm::ARM_ATTACK);
			m_LeftArm->Set_State(CArm::ARM_IDLE);
			m_Hand = false;
		}
		m_Shaking = false;
		m_bCalled = true;
	}

	if(m_Hand)
	{
		Arm_Posing(fTimeDelta, false, true);

		if (m_LeftArm->Get_Portaling())
		{
			m_LeftArmTr->Go_Straight(fTimeDelta * 10.f );
			if (m_LeftArm->Get_ParentCollide() || m_LeftArmTr->Get_OnCollide())
			{
				if (!m_Shaking)
				{
					m_pPlayer->Set_Shake(0.5f, 2.f);
					m_Shaking = true;
				}
				p_instance->StopSound( CSoundMgr::WEAPON_EFFECT3);
				p_instance->Play_Sound(rand() % 2 == 0 ? TEXT("Explosion_Punch_0.wav") : TEXT("Explosion_Punch_1.wav"), CSoundMgr::WEAPON_EFFECT3, 1.f);

				m_LeftArm->Set_Portaling(false);
				m_pAttackRange->Set_Valid(false);

				if(InitArmPosition(fTimeDelta,true, false))
				{
					p_instance->StopSound(CSoundMgr::WEAPON_EFFECT3);
					if (m_TotalPunchCount <= m_CurrPunchCount)
					{
						Set_BossState(BOSS_IDLE);
						RELEASE_INSTANCE(CGameInstance);
						return;
					}
					m_bCalled = false;
					++m_CurrPunchCount;
				}
				
			}
		}
		else if (Move_By_Bazier(ARM_LEFT, fTimeDelta))
		{
			if (!m_Shaking)
			{
				m_pPlayer->Set_Shake(0.5f, 2.f);
				m_Shaking = true;
			}
			p_instance->StopSound(CSoundMgr::WEAPON_EFFECT3);
			p_instance->Play_Sound(rand() % 2 == 0 ? TEXT("Explosion_Punch_0.wav") : TEXT("Explosion_Punch_1.wav"), CSoundMgr::WEAPON_EFFECT3, 1.f);

			m_pAttackRange->Set_Valid(false);

			if (InitArmPosition(fTimeDelta, true, false))
			{
				p_instance->StopSound(CSoundMgr::WEAPON_EFFECT3);
				if (m_TotalPunchCount <= m_CurrPunchCount)
				{
					Set_BossState(BOSS_IDLE);
					RELEASE_INSTANCE(CGameInstance);
					return;
				}
				m_bCalled = false;
				++m_CurrPunchCount;
			}
		}

	}
	else
	{
		Arm_Posing(fTimeDelta, true, false);

		if (m_RightArm->Get_Portaling())
		{
			m_RightArmTr->Go_Straight(fTimeDelta * 10.f);
			if (m_RightArm->Get_ParentCollide() ||m_RightArmTr->Get_OnCollide())
			{
				if (!m_Shaking)
				{
					m_pPlayer->Set_Shake(0.5f, 2.f);
					m_Shaking = true;
				}
				p_instance->StopSound(CSoundMgr::WEAPON_EFFECT3);
				p_instance->Play_Sound(rand() % 2 == 0 ? TEXT("Explosion_Punch_0.wav") : TEXT("Explosion_Punch_1.wav"), CSoundMgr::WEAPON_EFFECT3, 1.f);

				m_RightArm->Set_Portaling(false);
				m_pAttackRange->Set_Valid(false);

				if (InitArmPosition(fTimeDelta, false, true))
				{
					p_instance->StopSound(CSoundMgr::WEAPON_EFFECT3);
					if (m_TotalPunchCount <= m_CurrPunchCount)
					{
						Set_BossState(BOSS_IDLE);
						RELEASE_INSTANCE(CGameInstance);
						return;
					}
					m_bCalled = false;
					++m_CurrPunchCount;
				}
			}
		}
		else if (Move_By_Bazier(ARM_RIGHT, fTimeDelta))
		{
			if (!m_Shaking)
			{
				m_pPlayer->Set_Shake(0.5f, 2.f);
				m_Shaking = true;
			}

			p_instance->StopSound(CSoundMgr::WEAPON_EFFECT3);
			p_instance->Play_Sound(rand() % 2 == 0 ? TEXT("Explosion_Punch_0.wav") : TEXT("Explosion_Punch_1.wav"), CSoundMgr::WEAPON_EFFECT3, 1.f);

			m_pAttackRange->Set_Valid(false);

			if (InitArmPosition(fTimeDelta, false, true))
			{
				p_instance->StopSound(CSoundMgr::WEAPON_EFFECT3);
				if (m_TotalPunchCount <= m_CurrPunchCount)
				{
					Set_BossState(BOSS_IDLE);
					RELEASE_INSTANCE(CGameInstance);
					return;
				}
				m_bCalled = false;
				++m_CurrPunchCount;
			}
		}
	}
	RELEASE_INSTANCE(CGameInstance);

}


CBoss* CBoss::Create(LPDIRECT3DDEVICE9 m_pGraphic_Device)
{
	CBoss* p_boss = new CBoss(m_pGraphic_Device);
	if(FAILED(p_boss->NativeConstruct_Prototype()))
	{
		Safe_Release(p_boss);
		return nullptr;
	}
	return p_boss;
}

CGameObject* CBoss::Clone(void* pArg)
{
	CBoss* p_boss = new CBoss(*this);
	if (FAILED(p_boss->NativeConstruct(pArg)))
	{
		Safe_Release(p_boss);
		return nullptr;
	}
	return p_boss;
}

void CBoss::Free()
{
	__super::Free();
	Safe_Release(m_pOnlyRotation);
	Safe_Release(m_pTransform);
	Safe_Release(m_pTexture);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pCollider);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);

}


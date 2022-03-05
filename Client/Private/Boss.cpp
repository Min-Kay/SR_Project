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
{
	Safe_AddRef(m_pOnlyRotation);
	Safe_AddRef(m_pTransform);
	Safe_AddRef(m_pTexture);
	Safe_AddRef(m_pRenderer);
	Safe_AddRef(m_pCollider);
	Safe_AddRef(m_pBuffer);

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

	return S_OK;
}

_int CBoss::Tick(_float fTimeDelta)
{
	if (Get_Dead() || fTimeDelta <= 0.f)
		return 0;

	if (0 > __super::Tick(fTimeDelta))
		return -1;


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
		m_ImageIndex = 2;
		CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
		p_instance->StopSound(CSoundMgr::ENEMY_EFFECT3);
		p_instance->Play_Sound(TEXT("Boss_Die.wav"), CSoundMgr::ENEMY_EFFECT3, 1.f);
		RELEASE_INSTANCE(CGameInstance);
		Set_BossState(BOSS_DIE);
	}

	m_pCollider->Set_Collider();

	State_Machine(fTimeDelta);

	if(m_Resizing && m_Sizing)
		Synchronize_Transform();

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

	if (FAILED(m_pOnlyRotation->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(m_pTexture->Bind_OnGraphicDevice(m_ImageIndex)))
		return E_FAIL;

	m_pBuffer->Render(); 

	if (FAILED(__super::Render()))
		return E_FAIL;
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


	

	_float3 vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
	_float3 vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	D3DXVec3Normalize(&vRight, &vRight);


	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);

	m_pPlayer = static_cast<CPlayer*>(p_instance->Get_GameObject_End(g_CurrLevel, TEXT("Layer_Player")));
	m_pPlayerTr = static_cast<CTransform*>(m_pPlayer->Get_Component(COM_TRANSFORM));
	
	if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("Arm"), TEXT("Prototype_GameObject_Arm"))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_LeftArm = static_cast<CArm*>(static_cast<CEnemy*>((p_instance->Get_GameObject_End(g_CurrLevel, TEXT("Arm")))));
	m_LeftArm->Set_Parent(this);
	m_LeftArm->Set_Position(vPos - vRight * 15.f);
	m_LeftArm->Set_Player(m_pPlayer);
	m_LeftArm->Set_ArmPos(CArm::ARMPOS_LEFT);
	m_LeftArmTr = static_cast<CTransform*>(m_LeftArm->Get_Component(COM_TRANSFORM));


	if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("Arm"), TEXT("Prototype_GameObject_Arm"))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_RightArm = static_cast<CArm*>(static_cast<CEnemy*>((p_instance->Get_GameObject_End(g_CurrLevel, TEXT("Arm")))));
	m_RightArm->Set_Parent(this);
	m_RightArm->Set_Position(vPos + vRight * 15.f);
	m_RightArm->Set_Player(m_pPlayer);
	m_RightArm->Set_ArmPos(CArm::ARMPOS_RIGHT);
	m_RightArmTr = static_cast<CTransform*>(m_RightArm->Get_Component(COM_TRANSFORM));


	Set_Type(OBJ_ENEMY);

	m_pCollider->Set_ParentInfo(this);
	m_pCollider->Set_CollStyle(CCollider::COLLSTYLE_ENTER);

	m_pTransform->Scaled(m_vScale);
	m_pOnlyRotation->Scaled(m_vScale);
	m_pCollider->Set_State(CBoxCollider::COLL_SIZE, m_vScale);

	p_instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_OBJ, m_pCollider);

	p_instance->PlayBGM(TEXT("Boss_Stage.wav"));

	p_instance->Play_Sound(TEXT("Boss_Sound.wav"), CSoundMgr::ENEMY_EFFECT1, 1.0f);
	RELEASE_INSTANCE(CGameInstance);

	if (!m_pPlayer || !m_RightArm || !m_LeftArm)
		return E_FAIL;
	
	m_Hp = 100;
	m_Damage = 10;
	
	return S_OK; 

}

void CBoss::Set_InitPos(_float3 _pos)
{
	m_InitPos = _pos;
}

_bool CBoss::InitArmPosition(_float fTimeDelta)
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

		m_LeftTimer = 0.f;
		m_RightTimer = 0.f;

		Set_ArmPos(ARM_LEFT,m_LeftArmTr->Get_State(CTransform::STATE_POSITION), vPos + vUp * m_fUpMidPos - vRight * m_fRightMidPos, vPos + vUp * m_fUpPos - vRight * m_fRightPos);
		Set_ArmPos(ARM_RIGHT, m_RightArmTr->Get_State(CTransform::STATE_POSITION), vPos + vUp * m_fUpMidPos + vRight * m_fRightMidPos, vPos + vUp * m_fUpPos + vRight * m_fRightPos);

		m_RightArm->Set_Rolling(true, _float3(1.f, -1.f, 0.f));
		m_LeftArm->Set_Rolling(true, _float3(1.f, 1.f, 0.f));

		idlePos = false;
		initPos = true;
	}

	if(!idlePos)
	{
		_bool leftReach  = Move_By_Bazier(ARM_LEFT,fTimeDelta);
		_bool rightReadch = Move_By_Bazier(ARM_RIGHT, fTimeDelta);

		if (leftReach && rightReadch)
		{
			m_LeftTimer = 0.f;
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

	Init_Idle();

	m_State = _state;
	
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

void CBoss::Sizing_Particles()
{
	CImpact::IMPACT Impact1;
	ZeroMemory(&Impact1, sizeof(Impact1));
	Impact1.Pos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	Impact1.Size = _float3(0.05f, 0.05f, 0.05f);
	Impact1.randomPos = 5;
	Impact1.Speed = 5;
	Impact1.deleteCount = 1;//rand() % 5 + 2;
	Impact1.DeleteImpact = false;
	Impact1.Gradation = CImpact::GRADATION_UP;
	Impact1.Color = D3DXCOLOR(0.0f, 0.1f, 0.8f, 0.0f);
	Impact1.ChangeColor = D3DXCOLOR(0.0f, 0.05f, 0.0001f, 0.0f);

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	for (int i = 0; i < rand() % 10 + 25; ++i)
	{
		if (FAILED(p_instance->Add_GameObject(LEVEL_STAGEONE, TEXT("Impact"), TEXT("Prototype_GameObject_Impact"), &Impact1)))
		{
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
	}
	RELEASE_INSTANCE(CGameInstance);
}


void CBoss::Resizing(_float fTimeDelta)
{
	if(m_fTimer == 0.f)
	{
		m_Invincible = true;
		CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
		//p_instance->StopSound(CSoundMgr::ENEMY_EFFECT2);
		p_instance->Play_Sound(TEXT("Resizing.wav"), CSoundMgr::ENEMY_EFFECT3, 1.f);
		RELEASE_INSTANCE(CGameInstance);
	}


	m_fTimer += fTimeDelta * 1.8f;

	m_pOnlyRotation->Turn(m_SizingAxis, fTimeDelta * 0.3f);

	m_pOnlyRotation->Scaled(_float3(m_vScale.x - m_fTimer, m_vScale.y - m_fTimer, m_vScale.z - m_fTimer));

	Sizing_Particles();

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

	Sizing_Particles();

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
	m_fTimer += fTimeDelta;

	m_pTransform->Gravity(0.3f, fTimeDelta);
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	list<CCollision_Manager::COLLPOINT> collList = p_instance->Get_Ray_Collision_List(-m_pTransform->Get_State(CTransform::STATE_UP), m_pTransform->Get_State(CTransform::STATE_POSITION), 100, true);

	if(collList.empty() || (collList.size() == 1 && collList.front().CollObj == this))
	{
		RELEASE_INSTANCE(CGameInstance);
		return;
	}

	auto iter = collList.begin();
	++iter;

	m_pOnlyRotation->LookAt(m_pPlayerTr->Get_State(CTransform::STATE_POSITION));

	m_pTransform->Set_State(CTransform::STATE_POSITION, (*iter).Point + _float3(0.f, 6.f,0.f) + _float3(0.f, 1.f, 0.f) * sinf(D3DXToDegree(m_fTimer * 0.01f) * 2.f));

	RELEASE_INSTANCE(CGameInstance);

	_float3 vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
	_float3 vUp = m_pTransform->Get_State(CTransform::STATE_UP);

	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	m_LeftArm->Set_Position(vPos - vRight * m_fRightPos - vRight * sinf(D3DXToDegree(m_fTimer * 0.05f)) * 2.f + vUp * m_fUpPos * cosf(D3DXToDegree(m_fTimer * 0.04f)));
	m_RightArm->Set_Position(vPos + vRight * m_fRightPos + vRight * cosf(D3DXToDegree(m_fTimer * 0.05f)) * 2.f + vUp * m_fUpPos * cosf(D3DXToDegree(m_fTimer * 0.04f)));


	Reset_Arm_Direction(ARM_LEFT);
	Reset_Arm_Direction(ARM_RIGHT);

}

void CBoss::Randomize_Pattern(_float fTimeDelta)
{
	if( m_Reset && (m_State == BOSS_IDLE || m_State == BOSS_MOVE))
	{
		m_AttPatternTimer += fTimeDelta;
		if(m_AttPatternTimer > 5.f)
		{
			m_AttState = (BOSSATTACK)(rand() % 2);
			m_AttPatternTimer = 0.f;
			Set_BossState(BOSS_ATTACK);
		}
	}
}


_bool CBoss::Move_By_Bazier(ARM _arm , _float fTimeDelta)
{
	switch(_arm)
	{
	case ARM_LEFT:
		if (m_LeftTimer >= 1.f ||  2.f >= D3DXVec3Length(&(m_LeftArmTr->Get_State(CTransform::STATE_POSITION) - leftArmBazier[2])))
		{
			m_LeftTimer = 0.f;
			Reset_Arm_Direction(ARM_LEFT);
			return true;
		}

		m_LeftTimer += fTimeDelta * 0.7f;
		m_LeftArmTr->Set_State(CTransform::STATE_POSITION, (_float)pow(1 - m_LeftTimer, 2) * leftArmBazier[0] + 2 * m_LeftTimer * (1 - m_LeftTimer) * leftArmBazier[1] + (_float)pow(m_LeftTimer, 2)* leftArmBazier[2]);
		break;
	case ARM_RIGHT:
		if (m_RightTimer >= 1.f || 2.f >= D3DXVec3Length(&(m_RightArmTr->Get_State(CTransform::STATE_POSITION) - rightArmBazier[2])))
		{
			m_RightTimer = 0.f;

			Reset_Arm_Direction(ARM_RIGHT);

			return true;
		}

		m_RightTimer += fTimeDelta * 0.7f;
		m_RightArmTr->Set_State(CTransform::STATE_POSITION, (_float)pow(1 - m_RightTimer, 2) * rightArmBazier[0] + 2 * m_RightTimer * (1 - m_RightTimer) * rightArmBazier[1] + (_float)pow(m_RightTimer, 2) * rightArmBazier[2]);
		break;
	}
	return false;

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
	case BOSS_PHASECHANGE:
		break; 
	case BOSS_DIE:
		Die(fTimeDelta);
		break;
	}
}

void CBoss::Idle(_float fTimeDelta)
{
	
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
	m_fTimer += fTimeDelta;

	m_pTransform->Gravity(0.3f, fTimeDelta);

	m_pOnlyRotation->LookAt(m_pPlayerTr->Get_State(CTransform::STATE_POSITION));
	_float3 vOnlyRight = m_pOnlyRotation->Get_State(CTransform::STATE_RIGHT);

	_float3 vUp = m_pTransform->Get_State(CTransform::STATE_UP);
	_float3 vScale = m_pTransform->Get_Scale();

	D3DXVec3Normalize(&vOnlyRight, &vOnlyRight);
	D3DXVec3Normalize(&vUp, &vUp);

	_float3 vLook = *D3DXVec3Cross(&vLook, &vOnlyRight,&vUp);

	m_pTransform->Set_State(CTransform::STATE_RIGHT, vOnlyRight *	vScale.x);
	m_pTransform->Set_State(CTransform::STATE_LOOK, vLook * vScale.z);

	m_pTransform->Go_Straight(fTimeDelta);

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	list<CCollision_Manager::COLLPOINT> collList = p_instance->Get_Ray_Collision_List(-vUp, m_pTransform->Get_State(CTransform::STATE_POSITION), 100, true);

	if (collList.empty() || (collList.size() == 1 && collList.front().CollObj == this))
	{
		RELEASE_INSTANCE(CGameInstance);
		return;
	}

	auto iter = collList.begin();
	++iter;

	_float3 vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	m_pTransform->Set_State(CTransform::STATE_POSITION, (*iter).Point + _float3(0.f, 6.f, 0.f) + _float3(0.f, 1.f, 0.f) * sinf(D3DXToDegree(m_fTimer * 0.01f) * 2.f));

	RELEASE_INSTANCE(CGameInstance);


	m_LeftArm->Set_Position(vPos - vOnlyRight * m_fRightPos - vOnlyRight * sinf(D3DXToDegree(m_fTimer * 0.05f)) * 2.f + vUp * m_fUpPos * cosf(D3DXToDegree(m_fTimer * 0.04f)));
	m_RightArm->Set_Position(vPos + vOnlyRight * m_fRightPos + vOnlyRight * cosf(D3DXToDegree(m_fTimer * 0.05f)) * 2.f + vUp * m_fUpPos * cosf(D3DXToDegree(m_fTimer * 0.04f)));


	Reset_Arm_Direction(ARM_LEFT);
	Reset_Arm_Direction(ARM_RIGHT);

	if(m_fMoveLength - 3.f > D3DXVec3Length(&(vPos - m_pPlayerTr->Get_State(CTransform::STATE_POSITION))))
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
		//페이즈 2
		break;
	}
}


void CBoss::Attack(_float fTimeDelta)
{
	// 1페이즈 패턴 구현
	switch (m_AttState)
	{
	case BOSSATT_PUNCH:
		Attack_Punch(fTimeDelta);
		break;
	case BOSSATT_MISSILE:
		Attack_Missile(fTimeDelta);
		break;
	case BOSSATT_MIXED:
		Attack_Mixed(fTimeDelta);
		break;
	}
}

void CBoss::Die(_float fTimeDelta)
{
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
		Set_Dead(true);
	}
}

void CBoss::Attack_Missile(_float fTimeDelta)
{
	// 미사일
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->StopSound(CSoundMgr::ENEMY_EFFECT2);
	p_instance->Play_Sound(TEXT("Boss_AttackAlarm.wav"), CSoundMgr::ENEMY_EFFECT2, 1.f);
	RELEASE_INSTANCE(CGameInstance);

	Set_BossState(BOSS_IDLE);
}

void CBoss::Attack_Punch(_float fTimeDelta)
{
	// 펀치
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->StopSound(CSoundMgr::ENEMY_EFFECT2);
	p_instance->Play_Sound(TEXT("Boss_AttackAlarm1.wav"), CSoundMgr::ENEMY_EFFECT2, 1.f);
	RELEASE_INSTANCE(CGameInstance);

	Set_BossState(BOSS_IDLE);

}

void CBoss::Attack_Mixed(_float fTimeDelta)
{
	 // 주먹 와리가리
	m_fTimer += fTimeDelta;




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

}


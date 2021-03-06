#include "stdafx.h"
#include "CubeMonster.h"

#include "Ball.h"
#include "Transform.h"
#include "Renderer.h"
#include "BoxCollider.h"
#include "CubeBullet.h"
#include "VIBuffer_Cube.h"
#include "GameInstance.h"

#include "Player.h"
#include "Effect_Alert.h"
#include "Impact.h"
#include "Shield.h"

CCubeMonster::CCubeMonster(LPDIRECT3DDEVICE9 m_pGraphic_Device)
	:CEnemy(m_pGraphic_Device)
{
}

CCubeMonster::CCubeMonster(const CCubeMonster& rhs)
	: CEnemy(rhs)
	,m_pTransform(rhs.m_pTransform)
	, m_pTexture(rhs.m_pTexture)
	, m_pVIBuffer(rhs.m_pVIBuffer)
	, m_pRenderer(rhs.m_pRenderer)
	, m_pBoxCollider(rhs.m_pBoxCollider)
	, m_pShader(rhs.m_pShader)

{

	Safe_AddRef(m_pTransform);
	Safe_AddRef(m_pTexture);
	Safe_AddRef(m_pVIBuffer);
	Safe_AddRef(m_pBoxCollider);
	Safe_AddRef(m_pShader);
	Safe_AddRef(m_pRenderer);
}

HRESULT CCubeMonster::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCubeMonster::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	return S_OK;
}

_int CCubeMonster::Tick(_float fTimeDelta)
{
	if (FAILED(__super::Tick(fTimeDelta)))
		return -1;

	if (m_Player->Get_PlayerDead())
		m_State = STATE_IDLE;
		
	State_Machine(fTimeDelta);
	m_pBoxCollider->Set_Collider();

	return 0;
}

_int CCubeMonster::LateTick(_float fTimeDelta)
{
	if(FAILED(__super::LateTick(fTimeDelta)))
		return -1;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA,this);

	return 0;
}

HRESULT CCubeMonster::Render()
{
	m_pTransform->Bind_OnShader(m_pShader);

	m_pShader->SetUp_ValueOnShader("g_ColorStack", &g_ControlShader, sizeof(_float));
	m_pShader->SetUp_ValueOnShader("g_Color", m_Color, sizeof(_float4));
	m_pTexture->Bind_OnShader(m_pShader, "g_Texture", m_MonsterType);
	m_pShader->Begin_Shader(SHADER_SETCOLOR_CUBE);
	m_pVIBuffer->Render();
	m_pShader->End_Shader();
	m_Color = _float4(0.f,0.f,0.f,0.f);
	m_pShader->SetUp_ValueOnShader("g_Color", m_Color, sizeof(_float4));


	return CEnemy::Render();
}

HRESULT CCubeMonster::SetUp_Component()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransform, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRenderer)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_CUBE, COM_BUFFER, (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CubeMonster"), COM_TEXTURE, (CComponent**)&m_pTexture)))
		return E_FAIL;

	/* For.Com_Box */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLLIDER, COM_COLLIDER, (CComponent**)&m_pBoxCollider)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_SHADER_CUBE, COM_SHADER, (CComponent**)&m_pShader)))
		return E_FAIL;

	Set_Type(OBJ_ENEMY);

	m_pBoxCollider->Set_State(CBoxCollider::COLL_SIZE, _float3(1.5f,1.5f,1.5f));
	m_pBoxCollider->Set_ParentInfo(this);
	m_pBoxCollider->Set_CollStyle(CCollider::COLLSTYLE_ENTER);

	if(m_Player)
		m_PlayerPos = static_cast<CTransform*>(m_Player->Get_Component(COM_TRANSFORM));

	m_Hp = 50;
	m_Damage = 20;

	CGameInstance* p_Instance = GET_INSTANCE(CGameInstance);
	p_Instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_OBJ, m_pBoxCollider);

	m_Player = static_cast<CPlayer*>(p_Instance->Get_GameObject(g_CurrLevel, TEXT("Layer_Player")));

	m_PlayerPos = static_cast<CTransform*>(m_Player->Get_Component(COM_TRANSFORM));

	CEffect::EFFECTDESC desc;
	ZeroMemory(&desc, sizeof(CEffect::EFFECTDESC));

	desc.FrameCount = 1;
	desc.Alpha = CEffect::EFFECTALPHA_BLEND;
	desc.Bilboard = true;
	desc.Style = CEffect::EFFECTSTYLE_FIX;
	desc.Texture = TEXT("Prototype_Component_Texture_Alert");
	desc.Shader_Style = SHADER_SETCOLOR_BLEND;
	desc.Shader_Control = &g_ControlShader;

	if (FAILED(p_Instance->Add_GameObject(g_CurrLevel,TEXT("Alert"), TEXT("Prototype_GameObject_Alert"), &desc)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_Effect = static_cast<CEffect*>(p_Instance->Get_GameObject_End(g_CurrLevel, TEXT("Alert")));
	m_Effect->Set_Vaild(false);

	static_cast<CEffect_Alert*>(m_Effect)->Set_TargetPos(m_pTransform);
	static_cast<CEffect_Alert*>(m_Effect)->Set_CamPos(m_Player->Get_Camera()->Get_CameraTransform());

	RELEASE_INSTANCE(CGameInstance);

	m_EnemyType = ENEMY_CUBEMONSTER;

	return S_OK;

}

void CCubeMonster::Move(_float fTimeDelta)
{
	_float3 myPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	if(0.5f >= D3DXVec3Length(&(myPos - m_MovePoint)) || m_pTransform->Get_OnCollide())
	{
		m_MovePoint.x = rand() % 2 == 0 ? m_InitPoint.x + rand() % 5 : m_InitPoint.x - rand() % 5;
		m_MovePoint.y = rand() % 2 == 0 ? m_InitPoint.y + rand() % 5 : m_InitPoint.y - rand() % 5;
		m_MovePoint.z = rand() % 2 == 0 ? m_InitPoint.z + rand() % 5 : m_InitPoint.z - rand() % 5;
	}

	_float3 vDir = m_MovePoint - myPos;

	D3DXVec3Normalize(&vDir,&vDir);

	Target_Turn(vDir,fTimeDelta);

	m_pTransform->Set_State(CTransform::STATE_POSITION,myPos + vDir * m_IdleSpeed);
	
}

void CCubeMonster::Blow(_float fTimeDelta)
{
	if (fTimeDelta <= 0.f)
		return;

	_float3 vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
	_float3 vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vUp = m_pTransform->Get_State(CTransform::STATE_UP);

	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	m_Angle += 10.f;

	m_YMove = (_int)m_Angle % 360 == 0 ? !m_YMove : m_YMove;
	m_RMove = (_int)m_Angle % 720 == 0 ? !m_RMove : m_RMove;


	vUp = m_YMove ? vUp * sinf(D3DXToRadian(m_Angle)) * 0.01f : -vUp * sinf(D3DXToRadian(m_Angle)) * 0.01f; 
	vRight = m_RMove ? vRight * 0.003f : -vRight * 0.003f;

	m_pTransform->Turn(vUp, fTimeDelta * 0.5f);
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos + vRight + vUp);

}

const CCubeMonster::STATE& CCubeMonster::Get_MonsterState() const
{
	return m_State;
}

void CCubeMonster::Set_MonsterState(STATE _state)
{
	m_Timer = 0.f;
	m_State = _state;
}

void CCubeMonster::Target_Turn(_float3 dir, _float fTimeDelta)
{
	_float3 vAxis = *D3DXVec3Cross(&vAxis, &(m_pTransform->Get_State(CTransform::STATE_LOOK)), &(_float3(dir.x, 0.f, dir.z)));

	m_pTransform->Turn(vAxis, fTimeDelta);
}

void CCubeMonster::Add_HP(_int _add)
{
	m_Color = _float4(1.f,1.f,1.f,0.5f);
	__super::Add_HP(_add);
}

void CCubeMonster::Dying(_float fTimeDelta)
{
	m_pTransform->Gravity(1.f, fTimeDelta);
	m_pTransform->Add_Force(fTimeDelta);
	Set_Type(OBJ_INTERACTION);

	if (m_pTransform->Get_CollideFormalForce() >= 1.f && !Get_Grab())
	{
		CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
		list<CGameObject*> colllist = p_instance->Get_Collision_Object_List(m_pBoxCollider);

		if (colllist.empty())
		{
			RELEASE_INSTANCE(CGameInstance);
			return;
		}

		list<CEnemy*> enemyList;

		for (auto& obj : colllist)
		{
			if (obj->Get_Type() == OBJ_ENEMY)
			{
				enemyList.push_back(static_cast<CEnemy*>(obj));
			}
		}

		if (!enemyList.empty())
		{
			enemyList.sort([](CEnemy* a, CEnemy* b) { return a->Get_EnemyType() > b->Get_EnemyType(); });

			switch (enemyList.front()->Get_EnemyType())
			{
			case ENEMY_SHIELD:
				static_cast<CShield*>(enemyList.front())->Add_ShieldHp(-m_Damage * 3);
				break;
			case ENEMY_BOSS:
				enemyList.front()->Add_HP(-m_Damage);
				break;
			}

			p_instance->StopSound(CSoundMgr::ENEMY_EFFECT1);
			p_instance->Play_Sound(rand() % 2 == 0 ? TEXT("Minimy_Die1.wav") : TEXT("Minimy_Die.wav"), CSoundMgr::ENEMY_EFFECT1, 1.0f);
			Impact();
			m_pBoxCollider->Set_Dead(true);
			Set_Dead(true);
			RELEASE_INSTANCE(CGameInstance);
			return;
		}

		p_instance->StopSound(CSoundMgr::ENEMY_EFFECT1);
		p_instance->Play_Sound(rand() % 2 == 0 ? TEXT("Minimy_Die1.wav") : TEXT("Minimy_Die.wav"), CSoundMgr::ENEMY_EFFECT1, 1.0f);
		Impact();
		m_pBoxCollider->Set_Dead(true);
		Set_Dead(true);
		RELEASE_INSTANCE(CGameInstance);
	}
}


_bool CCubeMonster::Sizing(_float fTimeDelta)
{
	m_Timer += fTimeDelta;
	if (!m_Sizing)
	{
		Impact();
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta);
		m_pTransform->Scaled(_float3(m_Timer, m_Timer, m_Timer));
		if (m_Timer >= 1.0f)
		{
			m_pTransform->Scaled(_float3(1.f, 1.f, 1.f));
			m_Sizing = true;
			m_Timer = 0.f;

		}
		return false;
	}

	return true;

}
void CCubeMonster::Impact()
{
	CImpact::IMPACT Impact1;
	ZeroMemory(&Impact1, sizeof(Impact1));
	Impact1.Position = m_pTransform->Get_State(CTransform::STATE_POSITION);
	Impact1.Size = _float3(0.08f, 0.08f, 0.08f);
	Impact1.RandomDirection = 5;
	Impact1.SpreadSpeed = 5;
	Impact1.DeleteTime = 1.f;//rand() % 5 + 2;
	Impact1.Color = _float4(0.f, 0.7f, 0.f, 0.f);

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	for (int i = 0; i < rand() % 5 + 5; ++i)
	{
		if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("Impact"), TEXT("Prototype_GameObject_Impact"), &Impact1)))
		{
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
	}
	RELEASE_INSTANCE(CGameInstance);
}

void CCubeMonster::Set_InitPos(_float3 _pos)
{
	m_InitPoint = _pos;
	m_MovePoint = m_InitPoint;

	m_pTransform->Set_State(CTransform::STATE_POSITION,_pos);
}

void CCubeMonster::Set_MonsterType(MONSTERTYPE _type)
{
	m_MonsterType = _type;
}

void CCubeMonster::Charging(_float fTimeDelta)
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->Play_Sound(TEXT("Charging.mp3"), CSoundMgr::ENEMY_EFFECT1, 1.f);
	RELEASE_INSTANCE(CGameInstance);

	_float3 playerPos = m_PlayerPos->Get_State(CTransform::STATE_POSITION);
	_float3 myPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

	_float3 vDir = playerPos - myPos;
	//Target_Turn(vDir, fTimeDelta * 2.f);

	if (m_ChargingTimer >= 0.5f)
		isBound = true;
	else if (m_ChargingTimer <= -0.5f)
		isBound = false;

	m_ChargingTimer = isBound ? m_ChargingTimer - fTimeDelta * 30.f : m_ChargingTimer + fTimeDelta * 30.f;

	m_pTransform->Turn(isBound ? vDir : -vDir, fTimeDelta * 5.f);

	if (0.75f <= m_Timer)
	{
		_float3 vUp, vRight, vLook, vScale;
		D3DXVec3Normalize(&vUp,&m_vChargingUp);
		D3DXVec3Normalize(&vLook, &m_vChargingLook);
		vRight = *D3DXVec3Cross(&vRight, &vUp, &vLook);
		vScale = m_pTransform->Get_Scale();

		m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
		m_pTransform->Set_State(CTransform::STATE_UP, vUp * vScale.y);
		m_pTransform->Set_State(CTransform::STATE_LOOK, vLook * vScale.z);


		m_Timer = 0.f;
		m_isFiring = true;
		m_isCharging = false;
	}


}

void CCubeMonster::Firing(_float fTimeDelta)
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->StopSound(CSoundMgr::ENEMY_EFFECT2);
	p_instance->Play_Sound(TEXT("Cube_Fire.wav"), CSoundMgr::ENEMY_EFFECT2, 1.f);

	_float3 vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);

	// ???? ???? ????

	if(FAILED(p_instance->Add_GameObject(g_CurrLevel,TEXT("CubeBullet"),TEXT("Prototype_GameObject_CubeBullet"))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return;
	}

	static_cast<CCubeBullet*>(p_instance->Get_GameObject_End(g_CurrLevel,TEXT("CubeBullet")))->Set_Init(m_pTransform->Get_State(CTransform::STATE_POSITION) + vLook * 2.f, vLook);


	RELEASE_INSTANCE(CGameInstance);

	m_isFiring = false;
	m_Rebounding = true;
	isBound = false;
	m_Timer = 0.f;
}

void CCubeMonster::Rebounding(_float fTimeDelta)
{
	_float3 playerPos = m_PlayerPos->Get_State(CTransform::STATE_POSITION);
	_float3 myPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
	 D3DXVec3Normalize(&vLook, &vLook);

	_float3 vDir = playerPos - myPos;
	Target_Turn(vDir, fTimeDelta * 2.f);

	_float length = D3DXVec3Length(&vDir);

	m_Timer += fTimeDelta;

	if(!isBound)
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, myPos - vLook * fTimeDelta * 30.f);
		if (m_Timer > 0.2f)
		{
			isBound = true;
			m_Timer = 0.f;
		}
	}
	else
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, myPos + vLook * fTimeDelta * 30.f);
		if (m_Timer > 0.2f)
		{
			m_Timer = 0.f;
			m_Rebounded = true;
		}
	}


	if(m_Rebounded)
	{
		if(length > m_AttackRange)
		{
			Set_MonsterState(STATE_CHASE);
		}
		m_Timer = 0.f;
		m_Rebounded = false;
		m_Rebounding = false;

	}
}

void CCubeMonster::Search_Player(_float fTimeDelta)
{
	if (!m_Player || fTimeDelta <= 0.f)
		return;

	if (m_MonsterType == MT_SEARCH)
	{
		_float3 playerPos = m_PlayerPos->Get_State(CTransform::STATE_POSITION);
		_float3 myPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

		if (m_SearchRange >= D3DXVec3Length(&(myPos - playerPos)))
		{
			Set_MonsterState(STATE_ALERT);
			return;
		}

	}

	Move(fTimeDelta);

	Blow(fTimeDelta);

	m_Timer += fTimeDelta;

	if(m_Timer >= 3.f)
	{
		m_Timer = 0.f;

		if (rand() % 2 != 0)
		{
			Set_MonsterState(STATE_IDLE);
		}
	}
}

void CCubeMonster::Alert_Company(_float fTimeDelta)
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	if(m_AlertTime <= m_Timer)
	{
		m_Effect->Set_Vaild(false);
		Set_MonsterState(STATE_CHASE);
		p_instance->StopSound(CSoundMgr::ENEMY_EFFECT1);

		RELEASE_INSTANCE(CGameInstance);
		return;
	}
	
	m_Timer += fTimeDelta;

	
	if(!m_Alert)
	{
		m_Alert = true;
		p_instance->Play_Sound(TEXT("Alarm.wav"), CSoundMgr::ENEMY_EFFECT1, 1.f);
		m_Effect->Set_Vaild(true);
	}


	list<CGameObject*> pMonsters = p_instance->Get_Layer(g_CurrLevel, TEXT("CubeMonster"));
	RELEASE_INSTANCE(CGameInstance);

	_float3 myPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

	m_CurrAlertRange += fTimeDelta * 10.f;
	if (m_AlertRange <= m_CurrAlertRange)
		m_CurrAlertRange = m_AlertRange;

	for(auto& mon : pMonsters)
	{
		CCubeMonster* company = static_cast<CCubeMonster*>(mon);
		_float3 monPos = static_cast<CTransform*>(company->Get_Component(COM_TRANSFORM))->Get_State(CTransform::STATE_POSITION);
		
		if (m_CurrAlertRange >= D3DXVec3Length(&(myPos - monPos)) && (company->Get_MonsterState() == STATE_IDLE || company->Get_MonsterState() == STATE_SEARCH))
		{
			company->Set_MonsterState(STATE_ALERT);
		}
	}

	m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_UP), fTimeDelta * 10.f);

	// AlertAnimation
}

void CCubeMonster::Chase_Player(_float fTimeDelta)
{
	_float3 playerPos = m_PlayerPos->Get_State(CTransform::STATE_POSITION);
	_float3 myPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

	_float3 vDir = playerPos - myPos;

	//Target_Turn(vDir, fTimeDelta * 2.f);
	m_pTransform->LookAt(playerPos);

	_float length = D3DXVec3Length(&vDir);

	if (length < m_AttackRange)
	{
		Set_MonsterState(STATE_ATTACK);
		m_pTransform->LookAt(playerPos);
		return;
	}

	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Set_State(CTransform::STATE_POSITION, myPos + vDir * m_ChaseSpeed);

}

void CCubeMonster::Attack(_float fTimeDelta)
{

	m_Timer += fTimeDelta;

	if (!m_isCharging && !m_isFiring && !m_Rebounding)
	{
		if(m_Timer >= 0.5f)
		{
			m_ChargingTimer = 0.25f;
			m_vChargingLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
			m_vChargingUp = m_pTransform->Get_State(CTransform::STATE_UP);
			D3DXVec3Normalize(&m_vChargingLook, &m_vChargingLook);
			m_isCharging = true;
			m_Timer = 0.f;
		}
	}
	else if(m_isCharging)
	{
		Charging(fTimeDelta);
	}
	else if(m_isFiring)
	{
		Firing(fTimeDelta);
	}
	else if(m_Rebounding)
	{
		Rebounding(fTimeDelta);

	}
}

void CCubeMonster::Idle(_float fTimeDelta)
{
	
	if (!m_Player)
		return;

	if(m_MonsterType == MT_SEARCH)
	{
		_float3 playerPos = m_PlayerPos->Get_State(CTransform::STATE_POSITION);
		_float3 myPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

		if (m_SearchRange >= D3DXVec3Length(&(myPos - playerPos)))
		{
			Set_MonsterState(STATE_ALERT);
			return;
		}
	}
	else
	{
		if (!Sizing(fTimeDelta))
			return;
	}

	m_Timer += fTimeDelta;

	Blow(fTimeDelta);

	if (m_Timer >= 5.f)
	{
		m_Timer = 0.f;
		if (rand() % 2 != 0)
		{
			Set_MonsterState(STATE_SEARCH);
			m_MovePoint = _float3(m_InitPoint.x + rand() % 10, m_InitPoint.x + rand() % 10, m_InitPoint.x + rand() % 10);
		}
	}

}

void CCubeMonster::State_Machine(_float fTimeDelta)
{
	if (Check_HP())
	{
		m_Effect->Set_Vaild(false);
		Set_MonsterState(STATE_DIE);
	}

	switch(m_State)
	{
	case STATE_IDLE:
		Idle(fTimeDelta);
		break;
	case STATE_SEARCH:
		Search_Player(fTimeDelta);
		break;
	case STATE_ALERT:
		Alert_Company(fTimeDelta);
		break;
	case STATE_CHASE:
		Chase_Player(fTimeDelta);
		break;
	case STATE_ATTACK:
		Attack(fTimeDelta);
		break; 
	case STATE_DIE:
		Dying(fTimeDelta);
		break;
	}
}

CCubeMonster* CCubeMonster::Create(LPDIRECT3DDEVICE9 m_pGraphic_Device)
{
	CCubeMonster* p_instance = new CCubeMonster(m_pGraphic_Device);

	if(FAILED(p_instance->NativeConstruct_Prototype()))
	{
		Safe_Release(p_instance);
		return nullptr;
	}
	return p_instance;
}

CGameObject* CCubeMonster::Clone(void* pArg)
{
	CCubeMonster* p_instance = new CCubeMonster(*this);

	if (FAILED(p_instance->NativeConstruct(pArg)))
	{
		Safe_Release(p_instance);
		return nullptr;
	}
	return p_instance;
}

void CCubeMonster::Free()
{
	__super::Free();
	Safe_Release(m_pTransform);
	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pShader);
	Safe_Release(m_pRenderer);
}

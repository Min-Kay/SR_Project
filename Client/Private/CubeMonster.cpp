#include "stdafx.h"
#include "CubeMonster.h"

#include "Transform.h"
#include "Renderer.h"
#include "BoxCollider.h"
#include "VIBuffer_Cube.h"
#include "GameInstance.h"

#include "Player.h"

Client::CCubeMonster::CCubeMonster(LPDIRECT3DDEVICE9 m_pGraphic_Device)
	:CEnemy(m_pGraphic_Device)
{
}

Client::CCubeMonster::CCubeMonster(const CCubeMonster& rhs)
	: CEnemy(rhs)
	,m_pTransform(rhs.m_pTransform)
	, m_pTexture(rhs.m_pTexture)
	, m_pVIBuffer(rhs.m_pVIBuffer)
	, m_pRenderer(rhs.m_pRenderer)
	, m_pBoxCollider(rhs.m_pBoxCollider)

{
}

HRESULT Client::CCubeMonster::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT Client::CCubeMonster::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	return S_OK;
}

_int Client::CCubeMonster::Tick(_float fTimeDelta)
{
	State_Machine(fTimeDelta);
	m_pBoxCollider->Set_Collider();

	return 0;
}

_int Client::CCubeMonster::LateTick(_float fTimeDelta)
{
	if(FAILED(__super::LateTick(fTimeDelta)))
		return -1;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA,this);

	return 0;
}

HRESULT Client::CCubeMonster::Render()
{

	if (FAILED(m_pTransform->Bind_OnGraphicDevice()))
		return -1;

	if (FAILED(m_pTexture->Bind_OnGraphicDevice()))
		return -1;


	m_pVIBuffer->Render();

	return CEnemy::Render();
}

HRESULT Client::CCubeMonster::SetUp_Component()
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
	Set_Type(OBJ_ENEMY);

	m_pBoxCollider->Set_State(CBoxCollider::COLL_SIZE, _float3(1.f,1.f,1.f));
	m_pBoxCollider->Set_ParentInfo(this);
	m_pBoxCollider->Set_CollStyle(CCollider::COLLSTYLE_ENTER);

	CGameInstance* p_Instance = GET_INSTANCE(CGameInstance);
	p_Instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_OBJ,m_pBoxCollider);

	m_Player = static_cast<CPlayer*>(p_Instance->Get_GameObject(g_CurrLevel, TEXT("Layer_Player")));
	RELEASE_INSTANCE(CGameInstance);

	if(m_Player)
		m_PlayerPos = static_cast<CTransform*>(m_Player->Get_Component(COM_TRANSFORM));

	m_Hp = 100;
	m_Damage = 5;

	return S_OK;

}

void CCubeMonster::Move(_float fTimeDelta)
{

	_float3 myPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	if(0.5f >= D3DXVec3Length(&(myPos - m_MovePoint)) || m_pBoxCollider->Get_OnCollide())
	{
		m_MovePoint = _float3(m_InitPoint.x + rand() % 10, m_InitPoint.x + rand() % 10, m_InitPoint.x + rand() % 10);
	}

	_float3 vDir = m_MovePoint - myPos;

	D3DXVec3Normalize(&vDir,&vDir);

	Target_Turn(vDir,fTimeDelta);

	m_pTransform->Set_State(CTransform::STATE_POSITION,myPos + vDir * m_IdleSpeed);
	
}

void CCubeMonster::Blow(_float3& pos)
{
	pos.y = m_YMove ? pos.y + 0.01f : pos.y - 0.01f;
	m_YPos = m_YMove ? m_YPos + 0.01f : m_YPos - 0.01f;
	if ((m_YMove && m_YPos >= m_YMax) || (!m_YMove && m_YPos <= -m_YMax))
		m_YMove = !m_YMove;

}

const CCubeMonster::STATE& CCubeMonster::Get_MonsterState() const
{
	return m_State;
}

void CCubeMonster::Set_MonsterState(STATE _state)
{
	m_State = _state;
}

void CCubeMonster::Target_Turn(_float3 dir, _float fTimeDelta)
{
	_float3 vAxis = *D3DXVec3Cross(&vAxis, &(m_pTransform->Get_State(CTransform::STATE_LOOK)), &(_float3(dir.x, 0.f, dir.z)));

	m_pTransform->Turn(vAxis, fTimeDelta);
}

void CCubeMonster::Set_InitPos(_float3 _pos)
{
	m_InitPoint = _pos;
	m_MovePoint = _pos;

	m_pTransform->Set_State(CTransform::STATE_POSITION,_pos);
}

void CCubeMonster::Search_Player(_float fTimeDelta)
{
	if (!m_Player)
		return;

	_float3 playerPos = m_PlayerPos->Get_State(CTransform::STATE_POSITION);
	_float3 myPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

	if( m_SearchRange >= D3DXVec3Length(&(myPos-playerPos)))
	{
		m_State = STATE_ALERT;
		return; 
	}

	Move(fTimeDelta);

	Blow(myPos);

	m_Timer += fTimeDelta;

	if(m_Timer >= 3.f)
	{
		m_Timer = 0.f;

		if (rand() % 2 != 0)
		{
			m_State = STATE_IDLE;
		}
	}
}

void CCubeMonster::Alert_Company(_float fTimeDelta)
{
	if(m_AlertTime <= m_Timer)
	{
		m_State = STATE_CHASE;
		m_Timer = 0.f;
		return;
	}

	m_Timer += fTimeDelta;

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	list<CGameObject*> pMonsters = p_instance->Get_Layer(g_CurrLevel, TEXT("CubeMonster"));
	RELEASE_INSTANCE(CGameInstance);
	_float3 myPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

	for(auto& mon : pMonsters)
	{
		CCubeMonster* company = static_cast<CCubeMonster*>(mon);
		_float3 monPos = static_cast<CTransform*>(company->Get_Component(COM_TRANSFORM))->Get_State(CTransform::STATE_POSITION);
		
		if (m_AlertRange >= D3DXVec3Length(&(myPos - monPos)) && company->Get_MonsterState() == STATE_IDLE)
		{
			company->Set_MonsterState(STATE_ALERT);
		}
	}


	// AlertAnimation
}

void CCubeMonster::Chase_Player(_float fTimeDelta)
{
	_float3 playerPos = m_PlayerPos->Get_State(CTransform::STATE_POSITION);
	_float3 myPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

	_float3 vDir = playerPos - myPos;

	Target_Turn(vDir, fTimeDelta * 2.f);

	_float length = D3DXVec3Length(&vDir);

	if (length < m_AttackRange)
	{
		m_State = STATE_ATTACK;
		return;
	}

	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Set_State(CTransform::STATE_POSITION, myPos + vDir * m_ChaseSpeed);

}

void CCubeMonster::Attack(_float fTimeDelta)
{
	_float3 playerPos = m_PlayerPos->Get_State(CTransform::STATE_POSITION);
	_float3 myPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

	_float3 vDir = playerPos - myPos;
	Target_Turn(vDir, fTimeDelta * 2.f);

	_float length = D3DXVec3Length(&vDir);

	if (m_AttackRange < length)
	{
		m_State = STATE_CHASE;
		m_Timer = 0.f;
		return;
	}

	m_Timer += fTimeDelta;

	if(m_AttackSpeed <= m_Timer)
	{
		// attack animation
		m_Player->Add_Hp(-m_Damage);
		m_Timer = 0.f;
	}

}
void CCubeMonster::Idle(_float fTimeDelta)
{
	
	if (!m_Player)
		return;
	_float3 playerPos = m_PlayerPos->Get_State(CTransform::STATE_POSITION);
	_float3 myPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

	if (m_SearchRange >= D3DXVec3Length(&(myPos - playerPos)))
	{
		m_State = STATE_ALERT;
		return;
	}

	m_Timer += fTimeDelta;

	Blow(myPos);

	if (m_Timer >= 3.f)
	{
		m_Timer = 0.f;
		if (rand() % 2 != 0)
		{
			m_State = STATE_SEARCH;
			m_MovePoint = _float3(m_InitPoint.x + rand() % 10, m_InitPoint.x + rand() % 10, m_InitPoint.x + rand() % 10);
		}
	}

}

void CCubeMonster::State_Machine(_float fTimeDelta)
{
	if (Check_HP())
		m_State = STATE_DIE;

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
		CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
		p_instance->Release_Collider(CCollision_Manager::COLLOBJTYPE_OBJ,m_pBoxCollider);
		RELEASE_INSTANCE(CGameInstance);
		break;
	}
}

Client::CCubeMonster* Client::CCubeMonster::Create(LPDIRECT3DDEVICE9 m_pGraphic_Device)
{
	CCubeMonster* p_instance = new CCubeMonster(m_pGraphic_Device);

	if(FAILED(p_instance->NativeConstruct_Prototype()))
	{
		Safe_Release(p_instance);
		return nullptr;
	}
	return p_instance;
}

CGameObject* Client::CCubeMonster::Clone(void* pArg)
{
	CCubeMonster* p_instance = new CCubeMonster(*this);

	if (FAILED(p_instance->NativeConstruct(pArg)))
	{
		Safe_Release(p_instance);
		return nullptr;
	}
	return p_instance;
}

void Client::CCubeMonster::Free()
{
	__super::Free();
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
}

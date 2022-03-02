#include "stdafx.h"
#include "Ball.h"
#include "BoxCollider.h"
#include "GameInstance.h"
#include "Player.h"


CBall::CBall(LPDIRECT3DDEVICE9 m_pGraphic_Device)
	:CEffect(m_pGraphic_Device)
{
}

CBall::CBall(const CBall& rhs)
	:CEffect(rhs)
	, m_pBoxCollider(rhs.m_pBoxCollider)
{
	Safe_AddRef(m_pBoxCollider);
}

HRESULT CBall::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CBall::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;
	return S_OK;
}

_int CBall::Tick(_float fTimeDelta)
{
	if (Get_Dead())
		return 0;

	m_Timer += fTimeDelta;

	if (0 > __super::Tick(fTimeDelta))
		return -1;

	Move(fTimeDelta);

	if (FAILED(m_pBoxCollider->Set_Collider()))
		return -1;
	return 0;
}

_int CBall::LateTick(_float fTimeDelta)
{
	if (Get_Dead())
		return 0;

	if (m_Timer >= 5.f || Check_Collide())
	{
		m_pBoxCollider->Set_Dead(true);
		Set_Dead(true);
		return 0;
	}


	if (0 > __super::LateTick(fTimeDelta))
		return -1;
	return 0;

}

HRESULT CBall::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;

}

void CBall::Set_Init(_float3 _pos, _float3 _dir)
{
	m_Dir = _dir;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _pos);
	m_pTransformCom->LookAt(_pos + m_Dir);
}

void CBall::Move(_float fTimeDelta)
{
	m_pTransformCom->Go_Straight(-m_Speed * fTimeDelta);
}

_bool CBall::Check_Collide()
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);

	list<CGameObject*> col_list = p_instance->Get_Collision_List(m_pBoxCollider);

	for(auto& obj : col_list)
	{
		if(obj->Get_Type() == OBJ_PLAYER)
		{
			static_cast<CPlayer*>(obj)->Add_Hp(-m_Damage);
			RELEASE_INSTANCE(CGameInstance);
			return true;
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	return false;
}

CBall* CBall::Create(LPDIRECT3DDEVICE9 m_pGraphic_Device)
{
	CBall* p_ball = new CBall(m_pGraphic_Device);

	if(FAILED(p_ball->NativeConstruct_Prototype()))
	{
		Safe_Release(p_ball);
		return nullptr;
	}
	return p_ball;
}

CGameObject* CBall::Clone(void* pArg)
{
	CBall* p_ball = new CBall(*this);

	if (FAILED(p_ball->NativeConstruct(pArg)))
	{
		Safe_Release(p_ball);
		return nullptr;
	}
	return p_ball;
	
}

void CBall::Free()
{
	__super::Free();
	Safe_Release(m_pBoxCollider);
}

HRESULT CBall::SetUp_Components()
{
	if (FAILED(__super::SetUp_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLLIDER, COM_COLLIDER, (CComponent**)&m_pBoxCollider)))
		return E_FAIL;

	m_pBoxCollider->Set_ParentInfo(this);
	m_pBoxCollider->Set_State(CBoxCollider::COLL_SIZE, _float3(1.f,1.f,1.f));
	Set_Type(OBJ_INTERACTION);
	m_pBoxCollider->Set_CollStyle(CCollider::COLLSTYLE_TRIGGER);

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);

	p_instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_OBJ, m_pBoxCollider);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;

}

HRESULT CBall::SetUp_RenderState()
{
	if (FAILED(__super::SetUp_RenderState()))
		return E_FAIL;
	return S_OK;

}

HRESULT CBall::Release_RenderState()
{
	if (FAILED(__super::Release_RenderState()))
		return E_FAIL;
	return S_OK;
}

#include "BoxCollider.h"
#include "Gameobject.h"
#include "Transform.h"

CBoxCollider::CBoxCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CCollider(pGraphic_Device)
{
}

CBoxCollider::CBoxCollider(const CBoxCollider& rhs)
	: CCollider(rhs)
{
}

HRESULT CBoxCollider::Update()
{
	return S_OK;
}

HRESULT CBoxCollider::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CBoxCollider::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;
	return S_OK;
}

_float3 CBoxCollider::Get_State(COLLIDERINFO info)
{
	
	return m_CollInfo[info];
}

HRESULT CBoxCollider::Set_State(COLLIDERINFO info, _float3 Input)
{
	if (info == COLLIDERINFO::COLL_SIZE)
	{
		m_CollInfo[info].x = abs(Input.x);
		m_CollInfo[info].y = abs(Input.y);
		m_CollInfo[info].z = abs(Input.z);
	}
	else 
		m_CollInfo[info] = Input;

	
	return S_OK;
}


HRESULT CBoxCollider::Get_Parentcom()
{
	
	m_ParentPos = (CTransform*)m_Parent->Get_Component(COM_TRANSFORM);

	if (!m_ParentPos)
	{
		MSGBOX("NO Parent!!!");
		return E_FAIL;
	}
	return S_OK;
	
}



HRESULT CBoxCollider::Set_Coilider()
{
	if (!m_ParentPos)
	{
		MSGBOX("NO Parent!!!");
		return E_FAIL;
	}

	m_CollInfo[COLLIDERINFO::COLL_CENTER] = m_ParentPos->Get_State(CTransform::STATE_POSITION);
	m_CollInfo[COLLIDERINFO::COLL_MIN] = m_CollInfo[COLLIDERINFO::COLL_CENTER] - (m_CollInfo[COLLIDERINFO::COLL_SIZE] * 0.5);
	m_CollInfo[COLLIDERINFO::COLL_MAX] = m_CollInfo[COLLIDERINFO::COLL_CENTER] + (m_CollInfo[COLLIDERINFO::COLL_SIZE] * 0.5);

	return S_OK;
}

void CBoxCollider::Draw_Box()
{
	LPD3DXMESH box;
	D3DXCreateBox(m_pGraphic_Device, m_CollInfo[COLLIDERINFO::COLL_SIZE].x, m_CollInfo[COLLIDERINFO::COLL_SIZE].y, m_CollInfo[COLLIDERINFO::COLL_SIZE].z, &box, NULL);
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//box->DrawSubset(0);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	Safe_Release(box);
}

CBoxCollider* CBoxCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBoxCollider* pInstance = new CBoxCollider(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CComponent* CBoxCollider::Clone(void* pArg)
{
	CBoxCollider* pInstance = new CBoxCollider(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CBoxCollider::Free()
{
	__super::Free();
}

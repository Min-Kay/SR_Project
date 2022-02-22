#include "BoxCollider.h"

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
}

HRESULT CBoxCollider::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;
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

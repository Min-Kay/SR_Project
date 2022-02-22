#include "SphereCollider.h"

CSphereCollider::CSphereCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CCollider(pGraphic_Device)
{
}

CSphereCollider::CSphereCollider(const CSphereCollider& rhs)
	:CCollider(rhs)
{
}

HRESULT CSphereCollider::Update()
{
	return S_OK;
}

HRESULT CSphereCollider::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
}

HRESULT CSphereCollider::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;
}

CSphereCollider* CSphereCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSphereCollider* pInstance = new CSphereCollider(pGraphic_Device);
	if(FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance; 
}

CComponent* CSphereCollider::Clone(void* pArg)
{
	CSphereCollider* pInstance = new CSphereCollider(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSphereCollider::Free()
{
	__super::Free();
}

#include "Collider.h"
#include "GameObject.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent(pGraphic_Device)
{
}

CCollider::CCollider(const CCollider& rhs)
	:CComponent(rhs)
{
}

HRESULT CCollider::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

}

HRESULT CCollider::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;
}

HRESULT CCollider::Set_Parent(CGameObject* _obj)
{
	m_Parent = _obj;
	Safe_AddRef(m_Parent);
	return S_OK;
}

const CGameObject* CCollider::Get_Parent() const
{
	return m_Parent;
}

HRESULT CCollider::Set_Type(COLLTYPE _type)
{
	if (_type >= COLLTYPE_END)
		return E_FAIL;

	m_type = _type;
}

const CCollider::COLLTYPE& CCollider::Get_Type() const
{
	return m_type;
}

const list<_float3>& CCollider::Get_IndexList() const
{
	return m_IndexList;
}

HRESULT CCollider::Bind_OnCollider()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);

	return E_NOTIMPL;
}

void CCollider::Free()
{
	__super::Free();
	m_IndexList.clear();
	Safe_Release(m_Parent);
}

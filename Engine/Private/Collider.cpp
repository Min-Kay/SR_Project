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
	return S_OK;

}

HRESULT CCollider::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;
	return S_OK;
}

HRESULT CCollider::Set_Parent(CGameObject* _obj)
{
	m_Parent = _obj;
	//Safe_AddRef(m_Parent);
	return S_OK;
}

CGameObject* CCollider::Get_Parent() 
{
	return m_Parent;
}

HRESULT CCollider::Set_CollType(COLLTYPE _type)
{
	if (_type >= COLLTYPE_END)
		return E_FAIL;

	m_CoLLtype = _type;
	return S_OK;
}

const CCollider::COLLTYPE& CCollider::Get_CollType() const
{
	return m_CoLLtype;
}

HRESULT CCollider::Set_ObjType(COLLOBJTYPE _type)
{
	if (_type >= COLLOBJTYPE_END)
		return E_FAIL;

	m_ObjType = _type;
	return S_OK;
}

const CCollider::COLLOBJTYPE & CCollider::Get_ObjType() const
{
	return m_ObjType;
}


void CCollider::Free()
{
	//Safe_Release(m_Parent);
	__super::Free();
	
}

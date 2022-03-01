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

HRESULT CCollider::Set_CollStyle(COLLSTYLE _type)
{
	if (_type >= COLLSTYLE_END)
		return E_FAIL;

	m_CollStyle = _type;
	return S_OK;
}

const CCollider::COLLSTYLE& CCollider::Get_CollStyle() const
{
	return m_CollStyle;
}

const _bool& CCollider::Get_OnCollide() const
{
	return m_OnCollide;
}

void CCollider::Set_OnCollide(_bool _bool)
{
	m_OnCollide = _bool;
}


void CCollider::Free()
{
	//Safe_Release(m_Parent);
	__super::Free();
	
}

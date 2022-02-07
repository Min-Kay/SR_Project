#include "..\Public\GameObject.h"
#include "GameInstance.h"
#include "Component.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pGraphic_Device(rhs.m_pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CGameObject::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::NativeConstruct(void * pArg)
{
	return S_OK;
}

_int CGameObject::Tick(_float fTimeDelta)
{
	return _int();
}

_int CGameObject::LateTick(_float fTimeDelta)
{
	return _int();
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent** ppOut, void* pArg)
{
	if (nullptr != Find_Component(pComponentTag))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CComponent*			pComponent = pGameInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.emplace(pComponentTag, pComponent);

	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CComponent * CGameObject::Find_Component(const _tchar * pComponentTag)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTagFinder(pComponentTag));

	if (iter == m_Components.end())
		return nullptr;

	return iter->second;	
}

void CGameObject::Free()
{
	Safe_Release(m_pGraphic_Device);

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);
	m_Components.clear();
}

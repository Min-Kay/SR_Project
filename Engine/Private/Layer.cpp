#include "..\Public\Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{

}

CComponent * CLayer::Get_Component(const _tchar * pComponentTag, _uint iIndex)
{
	if (iIndex >= m_Objects.size())
		return nullptr;

	auto	iter = m_Objects.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_Component(pComponentTag);	
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	m_Objects.push_back(pGameObject);

	return S_OK;
}

_int CLayer::Tick(_float fTimeDelta)
{
	for (auto& pGameObject : m_Objects)
	{
		if (0 > pGameObject->Tick(fTimeDelta))
			return -1;
	}
	return 0;
}

_int CLayer::LateTick(_float fTimeDelta)
{
	for (auto& pGameObject : m_Objects)
	{
		if (0 > pGameObject->LateTick(fTimeDelta))
			return -1;
	}

	return 0;
}

CLayer * CLayer::Create()
{
	CLayer* pLayer = new CLayer();

	return pLayer;	
}

void CLayer::Free()
{
	for (auto& pGameObject : m_Objects)
		Safe_Release(pGameObject);

	m_Objects.clear();
}

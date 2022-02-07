#include "..\Public\Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{

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

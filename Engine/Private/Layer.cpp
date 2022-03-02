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

list<class CGameObject*> CLayer::Get_List()
{
	return m_Objects;
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	m_Objects.push_back(pGameObject);

	return S_OK;
}

HRESULT CLayer::Release_GameObject(CGameObject* pTarget)
{
	auto iter = m_Objects.begin();
	
	for (; iter != m_Objects.end();)
	{
		if (*iter == pTarget)
		{
			Safe_Release(pTarget);
			iter = m_Objects.erase(iter);
			return S_OK;
		}
		else
			++iter;
	}
	return E_FAIL;
}

_int CLayer::Tick(_float fTimeDelta)
{

	for(auto iter = m_Objects.begin(); iter != m_Objects.end(); )
	{
		if ((*iter)->Get_Dead())
		{
			Safe_Release(*iter);
			iter = m_Objects.erase(iter);
		}
		else
		{
			if (0 > (*iter)->Tick(fTimeDelta))
				return -1;
			++iter;
		}
	}
	return 0;
}

_int CLayer::LateTick(_float fTimeDelta)
{
	for (auto& pGameObject : m_Objects)
	{
		if (pGameObject->Get_Dead())
			continue;

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

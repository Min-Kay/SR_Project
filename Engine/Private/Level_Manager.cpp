#include "..\Public\Level_Manager.h"
#include "Level.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CLevel_Manager)

CLevel_Manager::CLevel_Manager()
{
}

HRESULT CLevel_Manager::OpenLevel(_uint iLevelIndex, CLevel * pNextLevel)
{
	if (nullptr == pNextLevel)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	
	if(nullptr != m_pCurrentLevel)
	{
		if (FAILED(pGameInstance->Clear_LevelResource(m_iCurrentLevelIndex)))
			return E_FAIL;
	}

	Safe_Release(m_pCurrentLevel);

	m_pCurrentLevel = pNextLevel;

	m_iCurrentLevelIndex = iLevelIndex;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CLevel_Manager::Tick(_float fTimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return -1;

	return m_pCurrentLevel->Tick(fTimeDelta);
}

_int CLevel_Manager::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return -1;

	return m_pCurrentLevel->LateTick(fTimeDelta);
}

HRESULT CLevel_Manager::Render()
{
	if (nullptr == m_pCurrentLevel)
		return E_FAIL;

	return m_pCurrentLevel->Render();
}

void CLevel_Manager::Free()
{
	Safe_Release(m_pCurrentLevel);
}

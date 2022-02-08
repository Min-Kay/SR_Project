#include "stdafx.h"
#include "..\Public\Level_Loading.h"
#include "Loader.h"
#include "GameInstance.h"
#include "Level_GamePlay.h"

CLevel_Loading::CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{

}

HRESULT CLevel_Loading::NativeConstruct(LEVEL eNextLevel)
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	m_eNextLevel = eNextLevel;

	 m_pLoader = CLoader::Create(m_pGraphic_Device, m_eNextLevel);

	return S_OK;
}

_int CLevel_Loading::Tick(_float fTimeDelta)
{
	if(0 > __super::Tick(fTimeDelta))
		return -1;


	return 0;
}

_int CLevel_Loading::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (true == m_pLoader->isFinished())
	{
		CGameInstance*	pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		CLevel*		pLevel = nullptr;

		switch (m_eNextLevel)
		{
		case LEVEL_GAMEPLAY:
			pLevel = CLevel_GamePlay::Create(m_pGraphic_Device);
			break;
		case LEVEL_BOSS:
			// pLevel = CLevel_GamePlay::Create(m_pGraphic_Device);
			break;
		}				

		if (FAILED(pGameInstance->OpenLevel(m_eNextLevel, pLevel)))
			return E_FAIL;

		Safe_Release(pGameInstance);
	}

	return 0;
}

HRESULT CLevel_Loading::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("로딩레벨임. "));

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(eNextLevel)))
	{
		MSGBOX("Failed to Creating CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}

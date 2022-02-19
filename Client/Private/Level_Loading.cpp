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

	SetWindowText(g_hWnd, TEXT("PORTAL_LOADING"));

	m_eNextLevel = eNextLevel;

	 m_pLoader = CLoader::Create(m_pGraphic_Device, m_eNextLevel);

	 CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	 /* 사본객체를 생성ㅎ나다. */
	 CCamera::CAMERADESC		CameraDesc;
	 ZeroMemory(&CameraDesc, sizeof(CameraDesc));

	 CameraDesc.vEye = _float3(0.f, 0.f, -100.f);
	 CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	 CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	 CameraDesc.fFovy = D3DXToRadian(60.0f);
	 CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	 CameraDesc.fNear = 0.1f;
	 CameraDesc.fFar = 300.f;

	 CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	 CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	 CameraDesc.iLevel = LEVEL_LOADING; 

	 if (FAILED(pGameInstance->Add_Camera_Object(TEXT("Prototype_GameObject_Camera_Static"), TEXT("Loading_Camera"), &CameraDesc)))
		 return E_FAIL;

	 if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOADING,TEXT("Loading"), TEXT("Prototype_GameObject_Loading"))))
		 return E_FAIL;

	 RELEASE_INSTANCE(CGameInstance);
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
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

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

		RELEASE_INSTANCE(CGameInstance);
	}

	return 0;
}

HRESULT CLevel_Loading::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

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

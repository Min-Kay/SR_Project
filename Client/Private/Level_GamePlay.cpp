#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"
#include "Map.h"
#include "BackGround.h"
#include "GameInstance.h"
#include "Camera_Static.h"

CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{

}

HRESULT CLevel_GamePlay::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
	{
		MSGBOX("Failed to Creating NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(Ready_Prototype_GameObject()))
	{
		MSGBOX("Failed to Creating Ready_Prototype_GameObject");

		return E_FAIL;
	}

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
	{
		MSGBOX("Failed to Creating Ready_Layer_Camera");
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_Map(TEXT("Layer_Map"))))
	{
		MSGBOX("Failed to Creating Ready_Layer_Map");

		return E_FAIL;
	}
	return S_OK;
}

_int CLevel_GamePlay::Tick(_float fTimeDelta)
{
	if(0 > __super::Tick(fTimeDelta))
		return -1;

	return 0;
}

_int CLevel_GamePlay::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	return 0;
}

HRESULT CLevel_GamePlay::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("게임플레이임. "));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Prototype_GameObject()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* 원본객체 생성 */

	/* For.Prototype_GameObject_Map */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Map"), CMap::Create(m_pGraphic_Device))))
	{
		MSGBOX("Failed to Creating Prototype_GameObject_Map");
		return E_FAIL;
	}
	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Map(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* 사본객체를 생성ㅎ나다. */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Map"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* 사본객체를 생성ㅎ나다. */
	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CameraDesc));

	CameraDesc.vEye = _float3(0.f, -50.f, -50.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Camera_Static"), &CameraDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();
}

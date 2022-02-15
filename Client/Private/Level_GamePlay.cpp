#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Cam_Portal.h"

CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{

}

HRESULT CLevel_GamePlay::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

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

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* 사본객체를 생성ㅎ나다. */
	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CameraDesc));

	CameraDesc.vEye = _float3(0.f, 10.f, -7.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	CameraDesc.iLevel = LEVEL_GAMEPLAY;

	if (FAILED(pGameInstance->Add_Camera_Object(TEXT("Prototype_GameObject_Camera_Dynamic"), TEXT("Main_Camera"), &CameraDesc)))
		return E_FAIL;

	CCamera::CAMERADESC		CameraDesc2;
	ZeroMemory(&CameraDesc2, sizeof(CameraDesc2));

	CameraDesc2.vEye = _float3(0.f, 5.f, -5.f);
	CameraDesc2.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc2.vAxisY = _float3(0.f, 1.f, 0.f);
			  
	CameraDesc2.fFovy = D3DXToRadian(60.0f);
	CameraDesc2.fAspect = 1;
	CameraDesc2.fNear = 0.2f;
	CameraDesc2.fFar = 300.f;
			  
	CameraDesc2.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc2.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	CameraDesc2.iLevel = LEVEL_GAMEPLAY;


	if (FAILED(pGameInstance->Add_Camera_Object(TEXT("Prototype_GameObject_Camera_Portal"), TEXT("Portal_Camera_1"), &CameraDesc2)))
		return E_FAIL;

	CCamera::CAMERADESC		CameraDesc3;
	ZeroMemory(&CameraDesc3, sizeof(CameraDesc3));

	CameraDesc3.vEye = _float3(0.f, -5.f, -5.f);
	CameraDesc3.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc3.vAxisY = _float3(0.f, 1.f, 0.f);
			 
	CameraDesc3.fFovy = D3DXToRadian(60.0f);
	CameraDesc3.fAspect = 1;
	CameraDesc3.fNear = 0.2f;
	CameraDesc3.fFar = 300.f;
			 
	CameraDesc3.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc3.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	CameraDesc3.iLevel = LEVEL_GAMEPLAY;


	if (FAILED(pGameInstance->Add_Camera_Object(TEXT("Prototype_GameObject_Camera_Portal"), TEXT("Portal_Camera_2"), &CameraDesc3)))
		return E_FAIL;

	CCam_Portal* pPortal1 = static_cast<CCam_Portal*>(pGameInstance->Find_Camera_Object(TEXT("Portal_Camera_1")));
	CCam_Portal* pPortal2 = static_cast<CCam_Portal*>(pGameInstance->Find_Camera_Object(TEXT("Portal_Camera_2")));


	static_cast<CTransform*>(pPortal2->Get_Component(TEXT("Com_Transform_Portal")))->Set_State(CTransform::STATE_POSITION,_float3(-30.f,0.f,0.f));

	pPortal1->Set_ExitPortal(pPortal2);
	pPortal2->Set_ExitPortal(pPortal1);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Player"))))
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

#include "stdafx.h"
#include "..\Public\Level_StageOne.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Camera_Player.h"
#include "UI.h"
#include "Player.h"

CLevel_StageOne::CLevel_StageOne(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{

}

HRESULT CLevel_StageOne::NativeConstruct()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	pInstance->SetMouseMode(false,g_hWnd);
	RELEASE_INSTANCE(CGameInstance);


	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("PORTAL_GAMEPLAY"));

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;


	return S_OK;
}

_int CLevel_StageOne::Tick(_float fTimeDelta)
{
	if(0 > __super::Tick(fTimeDelta))
		return -1;

	return 0;
}

_int CLevel_StageOne::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	return 0;
}

HRESULT CLevel_StageOne::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_StageOne::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* 사본객체를 생성ㅎ나다. */
	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CameraDesc));

	CameraDesc.vEye = _float3(0.f, 0.f, -7.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	CameraDesc.iLevel = LEVEL_STAGEONE;

	if (FAILED(pGameInstance->Add_Camera_Object(CAM_PLAYER, MAIN_CAM, &CameraDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_StageOne::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, pLayerTag, TEXT("Prototype_GameObject_Terrain"))))
	{
		MSGBOX("Failed to add terrain clone in LevelGamePlay");
		return E_FAIL;
	}


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, pLayerTag, TEXT("Prototype_GameObject_Terrain"))))
	{
		MSGBOX("Failed to add terrain clone in LevelGamePlay");
		return E_FAIL;
	}

	static_cast<CTransform*>(pGameInstance->Get_GameObject(LEVEL_STAGEONE, pLayerTag, 0)->Get_Component(COM_TRANSFORM))->Set_State(CTransform::STATE_POSITION, _float3(5.f,0.f,0.f));


	static_cast<CTransform*>(pGameInstance->Get_GameObject(LEVEL_STAGEONE, pLayerTag, 1)->Get_Component(COM_TRANSFORM))->Set_State(CTransform::STATE_POSITION, _float3(0.f, -5.f, 0.f));

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("SkyBox"), TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;

	
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Ball"), TEXT("Prototype_GameObject_Ball"))))
		return E_FAIL;


	Engine::CUI::UIDESC desc2;
	ZeroMemory(&desc2, sizeof(desc2));
	desc2.Layer = 1;
	desc2.WinCX = g_iWinCX;
	desc2.WinCY = g_iWinCY;
	desc2.PosX = g_iWinCX * 0.5f;
	desc2.PosY = g_iWinCY * 0.5f;
	desc2.SizeX = g_iWinCX;
	desc2.SizeY = g_iWinCY;
	desc2.Alpha = Engine::CUI::ALPHA_BLEND;
	desc2.Texture = TEXT("Prototype_Component_Texture_BackUI");

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("BackUI"), TEXT("Prototype_GameObject_BackUI"), &desc2)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_StageOne::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, pLayerTag, TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_StageOne * CLevel_StageOne::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_StageOne*	pInstance = new CLevel_StageOne(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_StageOne::Free()
{
	__super::Free();
}

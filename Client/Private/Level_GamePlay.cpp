#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Camera_Player.h"
#include "UI.h"
#include "Player.h"

CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{

}

HRESULT CLevel_GamePlay::NativeConstruct()
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

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
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
	CameraDesc.iLevel = LEVEL_GAMEPLAY;

	if (FAILED(pGameInstance->Add_Camera_Object(CAM_PLAYER, MAIN_CAM, &CameraDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Terrain"))))
	{
		MSGBOX("Failed to add terrain clone in LevelGamePlay");
		return E_FAIL;
	}


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Terrain"))))
	{
		MSGBOX("Failed to add terrain clone in LevelGamePlay");
		return E_FAIL;
	}

	static_cast<CTransform*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, pLayerTag, 0)->Get_Component(COM_TRANSFORM))->Set_State(CTransform::STATE_POSITION, _float3(9.f,-5.f,0.f));

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("SkyBox"), TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;

	
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Ball"), TEXT("Prototype_GameObject_Ball"))))
		return E_FAIL;

	//for (int i = 0 ;i < 10; ++i)
	//{
	//	for (int j = 0; j < 10; ++j)
	//	{
	//		if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Bottom"), TEXT("Prototype_GameObject_BackGround"))))
	//			return E_FAIL;
	//		CGameObject* object_Bottom = pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Bottom"), i*10 + j);
	//		CTransform * Transform_Bottom = (CTransform*)object_Bottom->Get_Component(COM_TRANSFORM);
	//		Transform_Bottom->Set_State(CTransform::STATE_POSITION, _float3((j )* 1.f , 0.f, (i )* 1.f));
	//		Transform_Bottom->Rotation(_float3(1.f, 0.f, 0.f), 1.5708);

	//	}
	//}



	/*if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("left"), TEXT("Prototype_GameObject_BackGround"))))
		return E_FAIL;

	CGameObject* object_left = pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("left"));
	CTransform* Transform_left = (CTransform*)object_left->Get_Component(COM_TRANSFORM);
	Transform_left->Set_State(CTransform::STATE_POSITION, _float3(0.f, 2.f, (1 * 2) * 1.f));
	Transform_left->Rotation(_float3(0.f, 1.f, 0.f), -1.5708);*/

	////임시 벽
	//for (int z = 0; z < 20; ++z)
	//{
	//
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("left"), TEXT("Prototype_GameObject_BackGround"))))
	//		return E_FAIL;
	//
	//	CGameObject* object_left = pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("left"), z);
	//	CTransform * Transform_left = (CTransform*)object_left->Get_Component(COM_TRANSFORM);
	//	Transform_left->Set_State(CTransform::STATE_POSITION, _float3(0.f, 2.f, (z * 4)* 1.f));
	//	Transform_left->Rotation(_float3(0.f, 1.f, 0.f), -1.5708);
	//
	//
	//	/*if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("right"), TEXT("Prototype_GameObject_BackGround"))))
	//		return E_FAIL;*/
	//
	///*	CGameObject* object_right = pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("right"), z);
	//	CTransform * Transform_right = (CTransform*)object_right->Get_Component(COM_TRANSFORM);
	//	Transform_right->Set_State(CTransform::STATE_POSITION, _float3((z * 2)* 1.f, 2.f, 0.f));
	//	Transform_right->Rotation(_float3(0.f, 1.f, 0.f), -3.14159);*/
	//
	//
	//	//if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("up"), TEXT("Prototype_GameObject_BackGround"))))
	//	//	return E_FAIL;
	//
	//	//CGameObject* object_up = pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("up"), z);
	//	//CTransform * Transform_up = (CTransform*)object_up->Get_Component(COM_TRANSFORM);
	//	//Transform_up->Set_State(CTransform::STATE_POSITION, _float3((20 * 2)* 1.f, 2.f, (z * 2)* 1.f));
	//	//Transform_up->Rotation(_float3(0.f, 1.f, 0.f), 1.5708);
	//
	//	//if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("down"), TEXT("Prototype_GameObject_BackGround"))))
	//	//	return E_FAIL;
	//
	//	//CGameObject* object_down = pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("down"), z);
	//	//CTransform * Transform_down = (CTransform*)object_down->Get_Component(COM_TRANSFORM);
	//	//Transform_down->Set_State(CTransform::STATE_POSITION, _float3((z * 2)* 1.f, 2.f, (20 * 2)* 1.f));
	//	//Transform_down->Rotation(_float3(0.f, 1.f, 0.f), 0);
	//
	//}


	Engine::CUI::UIDESC desc;
	ZeroMemory(&desc,sizeof(desc));
	desc.WinCX = g_iWinCX;
	desc.WinCY = g_iWinCY;
	desc.PosX = g_iWinCX * 0.5f;
	desc.PosY = g_iWinCY * 0.5f;
	desc.SizeX = 50.f;
	desc.SizeY = 50.f;
	desc.Alpha = Engine::CUI::ALPHA_TEST;
	desc.Func = D3DCMP_GREATER;
	desc.Ref = 70;
	desc.Texture = TEXT("Prototype_Component_Texture_Crosshair");

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Crosshair"), PROTO_UI,&desc)))
		return E_FAIL;


	Engine::CUI::UIDESC desc2;
	ZeroMemory(&desc, sizeof(desc2));
	desc2.Layer = 1;
	desc2.WinCX = g_iWinCX;
	desc2.WinCY = g_iWinCY;
	desc2.PosX = g_iWinCX * 0.5f;
	desc2.PosY = g_iWinCY * 0.5f;
	desc2.SizeX = g_iWinCX;
	desc2.SizeY = g_iWinCY;
	desc2.Alpha = Engine::CUI::ALPHA_BLEND;
	desc2.Texture = TEXT("Prototype_Component_Texture_BackUI");

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("BackUI"), TEXT("Prototype_GameObject_BackUI"), &desc2)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;

	static_cast<CPlayer*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY,pLayerTag,0))->Set_Cam(pGameInstance->Find_Camera_Object(MAIN_CAM));

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

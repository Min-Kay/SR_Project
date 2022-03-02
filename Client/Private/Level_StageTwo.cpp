#include "stdafx.h"
#include "..\Public\Level_StageTwo.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Camera_Player.h"
#include "CubeMonster.h"
#include "Door_left.h"
#include "Door_right.h"
#include "UI.h"
#include "Player.h"
#include "Tile.h"
#include "Tile_Collider.h"
#include "Tile_Cube.h"

CLevel_StageTwo::CLevel_StageTwo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{

}

HRESULT CLevel_StageTwo::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Map()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("PORTAL_GAMEPLAY"));

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	pInstance->SetMouseMode(false,g_hWnd);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CLevel_StageTwo::Tick(_float fTimeDelta)
{
	if(0 > __super::Tick(fTimeDelta))
		return -1;

	/*if (FAILED(Open_Exit()))
	{
		MSGBOX("Failed to Open_Exit in CLevel_StageOne");
		return E_FAIL;
	}*/


	return 0;
}

_int CLevel_StageTwo::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	return 0;
}

HRESULT CLevel_StageTwo::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_StageTwo::Ready_Layer_Camera(const _tchar * pLayerTag)
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
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 300.f;

	CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	CameraDesc.iLevel = LEVEL_STAGETWO;

	if (FAILED(pGameInstance->Add_Camera_Object(CAM_PLAYER, MAIN_CAM, &CameraDesc)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_StageTwo::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("SkyBox"), TEXT("Prototype_GameObject_Sky"))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

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

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("BackUI"), TEXT("Prototype_GameObject_BackUI"), &desc2)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("CubeMonster"), TEXT("Prototype_GameObject_CubeMonster"))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("CubeMonster"), TEXT("Prototype_GameObject_CubeMonster"))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("CubeMonster"), TEXT("Prototype_GameObject_CubeMonster"))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("CubeMonster"), TEXT("Prototype_GameObject_CubeMonster"))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("CubeMonster"), TEXT("Prototype_GameObject_CubeMonster"))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("CubeMonster"), TEXT("Prototype_GameObject_CubeMonster"))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	CCubeMonster* monster = static_cast<CCubeMonster*>(pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("CubeMonster")));
	monster->Set_InitPos(_float3(-10.f, 15.f, 0.f));

	CCubeMonster* monster1 = static_cast<CCubeMonster*>(pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("CubeMonster"), 1));
	monster1->Set_InitPos(_float3(10.f, 10.f, 10.f));

	CCubeMonster* monster2 = static_cast<CCubeMonster*>(pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("CubeMonster"),2));
	monster2->Set_InitPos(_float3(-10.f, 15.f, 0.f));

	CCubeMonster* monster3 = static_cast<CCubeMonster*>(pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("CubeMonster"),4));
	monster3->Set_InitPos(_float3(-20.f, 20.f, 0.f));

	CCubeMonster* monster4 = static_cast<CCubeMonster*>(pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("CubeMonster"),5));
	monster4->Set_InitPos(_float3(-30.f, 30.f, 0.f));

	CCubeMonster* monster5 = static_cast<CCubeMonster*>(pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("CubeMonster"),3));
	monster5->Set_InitPos(_float3(-25.f, 20.f, 0.f));


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_StageTwo::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, pLayerTag, TEXT("Prototype_GameObject_Player"))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}


	CTransform* tr = static_cast<CTransform*>(pGameInstance->Get_GameObject(LEVEL_STAGETWO, pLayerTag, 0)->Get_Component(COM_TRANSFORM));

	tr->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.f, 0.f));
	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_StageTwo::Ready_Layer_Map()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//// Start Left Door
	//{

	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Left_Door"), TEXT("Prototype_GameObject_Left_Door"))))
	//		return E_FAIL;

	//	CGameObject* Left_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Left_Door"));
	//	CTransform* Lefttrans = (CTransform*)Left_Door->Get_Component(COM_TRANSFORM);

	//	Lefttrans->Scaled(_float3(fDoorSizeX, fDoorSizeY, 1.f));
	//	Lefttrans->Set_State(CTransform::STATE_POSITION, _float3(10.5f, 2.5f, 0.7f));

	//	CBoxCollider* box = static_cast<CBoxCollider*>(Left_Door->Get_Component(COM_COLLIDER));
	//	box->Set_State(CBoxCollider::COLL_SIZE, _float3(fDoorSizeX, fDoorSizeY, 1.f));
	//}
	//// Start Right Door
	//{
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Right_Door"), TEXT("Prototype_GameObject_Right_Door"))))
	//		return E_FAIL;

	//	CGameObject* Right_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Right_Door"));
	//	CTransform* Righttrans = (CTransform*)Right_Door->Get_Component(COM_TRANSFORM);

	//	Righttrans->Scaled(_float3(fDoorSizeX, fDoorSizeY, 1.f));
	//	Righttrans->Set_State(CTransform::STATE_POSITION, _float3(13.5f, 2.5f, 0.7f));

	//	CBoxCollider* box = static_cast<CBoxCollider*>(Right_Door->Get_Component(COM_COLLIDER));
	//	box->Set_State(CBoxCollider::COLL_SIZE, _float3(fDoorSizeX, fDoorSizeY, 1.f));
	//}

	//// Exit Left Door
	//{

	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Left_Exit"), TEXT("Prototype_GameObject_Left_Door"))))
	//		return E_FAIL;

	//	CGameObject* Left_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Left_Exit"));
	//	CTransform* Lefttrans = (CTransform*)Left_Door->Get_Component(COM_TRANSFORM);
	//	static_cast<CDoor_left*>(Left_Door)->Set_TextureIndex(1);
	//	Lefttrans->Scaled(_float3(fDoorSizeX, fDoorSizeY, 1.f));
	//	Lefttrans->Set_State(CTransform::STATE_POSITION, _float3(10.5f, 2.5f, (_float)iLineZ - 1.f));

	//	CBoxCollider* box = static_cast<CBoxCollider*>(Left_Door->Get_Component(COM_COLLIDER));
	//	box->Set_State(CBoxCollider::COLL_SIZE, _float3(fDoorSizeX, fDoorSizeY, 1.f));
	//}

	//// Exit Right Door
	//{
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Right_Exit"), TEXT("Prototype_GameObject_Right_Door"))))
	//		return E_FAIL;

	//	CGameObject* Right_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Right_Exit"));
	//	CTransform* Righttrans = (CTransform*)Right_Door->Get_Component(COM_TRANSFORM);
	//	static_cast<CDoor_right*>(Right_Door)->Set_TextureIndex(1);
	//	Righttrans->Scaled(_float3(fDoorSizeX, fDoorSizeY, 1.f));
	//	Righttrans->Set_State(CTransform::STATE_POSITION, _float3(13.5f, 2.5f, ((_float)iLineZ) - 1.f));

	//	CBoxCollider* box = static_cast<CBoxCollider*>(Right_Door->Get_Component(COM_COLLIDER));
	//	box->Set_State(CBoxCollider::COLL_SIZE, _float3(fDoorSizeX, fDoorSizeY, 1.f));
	//}


	//// Open Event용 충돌체 cube
	//{
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Open_Exit"), TEXT("Prototype_GameObject_Block"))))
	//		return E_FAIL;

	//	CGameObject* Switch = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Open_Exit"), 0);
	//	CTransform* trans = (CTransform*)Switch->Get_Component(COM_TRANSFORM);
	//	trans->Scaled(_float3(1.f, 1.f, 1.f));
	//	trans->Set_State(CTransform::STATE_POSITION, _float3(12.f, 1.f, ((_float)iLineZ) - 3.f));

	//	CBoxCollider* box = static_cast<CBoxCollider*>(Switch->Get_Component(COM_COLLIDER));

	//}

	//// 입구 천장
	//for (_int i = 0; i < iWaterZ + 1; ++i)
	//{
	//	for (_int j = 0; j < (iWaterZ + 1); ++j)
	//	{
	//		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_T"), TEXT("Prototype_GameObject_Tile"))))
	//			return E_FAIL;
	//		CGameObject* LeftTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_T"), (i * (iWaterZ + 1)) + j);
	//		CTransform* LeftTrans = (CTransform*)LeftTile->Get_Component(COM_TRANSFORM);
	//		static_cast<CTile*>(LeftTile)->Set_TextureIndex(0);

	//		LeftTrans->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(270));
	//		LeftTrans->Set_State(CTransform::STATE_POSITION, _float3((iWaterZ * 1.9f) + (j * 1.f), (_float)iWaterZ - 1.f, (i * -1.f)));

	//	}
	//}

	//// 입구 바닥
	//for (_int i = 0; i < (iWaterZ + 1); ++i)
	//{
	//	for (_int j = 0; j < (iWaterZ + 1); ++j)
	//	{
	//		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_Bot"), TEXT("Prototype_GameObject_Tile"))))
	//			return E_FAIL;
	//		CGameObject* LeftTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_Bot"), (i * (iWaterZ + 1)) + j);
	//		CTransform* LeftTrans = (CTransform*)LeftTile->Get_Component(COM_TRANSFORM);
	//		static_cast<CTile*>(LeftTile)->Set_TextureIndex(1);
	//		LeftTrans->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90));
	//		LeftTrans->Set_State(CTransform::STATE_POSITION, _float3((iWaterZ * 1.9f) + (j * 1.f), 1.f, (i * -1.f)));

	//	}
	//}

	//// 입구 바닥 충돌체
	//{
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_Bot_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
	//		return E_FAIL;

	//	CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_Bot_Collider"), 0);
	//	// tile의 위치
	//	CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
	//	static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
	//	transform->Set_State(CTransform::STATE_POSITION, _float3((iWaterZ * 1.9f), 0.f, 0.f));

	//	// collider의 위치
	//	CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

	//	box->Set_State(CBoxCollider::COLL_SIZE, _float3(7.f, 0.5f, 6.f));
	//	box->Set_AdditionalPos(_float3((iWaterZ * 0.5f), 0.5f, (-iWaterZ * 0.5f)));

	//}

	//// 입구 오른면
	//for (_int i = 0; i < iWaterZ + 1; ++i)
	//{
	//	for (_int j = 0; j < iWaterZ; ++j)
	//	{
	//		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_R"), TEXT("Prototype_GameObject_Tile"))))
	//			return E_FAIL;
	//		CGameObject* LeftTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_R"), (i * iWaterZ) + j);
	//		CTransform* LeftTrans = (CTransform*)LeftTile->Get_Component(COM_TRANSFORM);
	//		static_cast<CTile*>(LeftTile)->Set_TextureIndex(1);
	//		LeftTrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90));
	//		LeftTrans->Set_State(CTransform::STATE_POSITION, _float3((iWaterZ * 3.f), (j * 1.f), (i * -1.f)));

	//	}
	//}

	//// 입구 오른쪽면 충돌체
	//{
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_R_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
	//		return E_FAIL;

	//	CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_R_Collider"), 0);
	//	// tile의 위치
	//	CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
	//	static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
	//	transform->Set_State(CTransform::STATE_POSITION, _float3(_float3((iWaterZ * 3.f), 0.f, -2.f)));

	//	// collider의 위치
	//	CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

	//	box->Set_State(CBoxCollider::COLL_SIZE, _float3(1.f, 7.f, 6.f));
	//	box->Set_AdditionalPos(_float3(0.5f, 5.f, 0.f));

	//}

	//// 입구 왼면
	//for (_int i = 0; i < iWaterZ + 1; ++i)
	//{
	//	for (_int j = 0; j < iWaterZ; ++j)
	//	{
	//		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_L"), TEXT("Prototype_GameObject_Tile"))))
	//			return E_FAIL;
	//		CGameObject* LeftTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_L"), (i * iWaterZ) + j);
	//		CTransform* LeftTrans = (CTransform*)LeftTile->Get_Component(COM_TRANSFORM);
	//		static_cast<CTile*>(LeftTile)->Set_TextureIndex(1);
	//		LeftTrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(270));
	//		LeftTrans->Set_State(CTransform::STATE_POSITION, _float3((iWaterZ * 0.8f) + ((_float)iWaterZ), (j * 1.f), (i * -1.f)));

	//	}
	//}

	//// 입구 왼쪽면 충돌체
	//{
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_L_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
	//		return E_FAIL;

	//	CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_L_Collider"), 0);
	//	// tile의 위치
	//	CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
	//	static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
	//	transform->Set_State(CTransform::STATE_POSITION, _float3((iWaterZ * 0.5f) + ((_float)iWaterZ), ((_float)iWaterZ) * 0.5f, -(iWaterZ * 0.5f)));

	//	// collider의 위치
	//	CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

	//	box->Set_State(CBoxCollider::COLL_SIZE, _float3(1.f, 5.f, 6.f));
	//	box->Set_AdditionalPos(_float3(1.f, 1.f, 0.f));

	//}


	//// 입구 뒷면
	//for (_int i = 0; i < iWaterZ; ++i)
	//{
	//	for (_int j = 0; j < (iWaterZ + 1); ++j)
	//	{
	//		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_B"), TEXT("Prototype_GameObject_Tile"))))
	//			return E_FAIL;
	//		CGameObject* LeftTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_B"), (i * (iWaterZ + 1)) + j);
	//		CTransform* LeftTrans = (CTransform*)LeftTile->Get_Component(COM_TRANSFORM);
	//		static_cast<CTile*>(LeftTile)->Set_TextureIndex(1);
	//		LeftTrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180));
	//		LeftTrans->Set_State(CTransform::STATE_POSITION, _float3((iWaterZ * 1.9f) + (j * 1.f), (i * 1.f), -(_float)iWaterZ));

	//	}
	//}

	//// 입구 뒷면 충돌체
	//{
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_B_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
	//		return E_FAIL;

	//	CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_B_Collider"), 0);
	//	// tile의 위치
	//	CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
	//	static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
	//	// collider의 위치
	//	CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

	//	transform->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180));
	//	transform->Set_State(CTransform::STATE_POSITION, _float3((((_float)iWaterZ * 1.9f)), 1.f, -(_float)iWaterZ));

	//	box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ + 1, (_float)iWaterZ, 0.1f));
	//	box->Set_AdditionalPos(_float3((iWaterZ * 0.5f), (iWaterZ * 0.5f), 0.f));

	//}

	////뒷면_0
	//{
	//	for (_int i = 0; i < iLineY; ++i)
	//	{
	//		for (_int j = 0; j < (iWaterZ * 2); ++j)
	//		{
	//			if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Back0"), TEXT("Prototype_GameObject_Tile"))))
	//				return E_FAIL;
	//			CGameObject* FrontTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Back0"), (i * (iWaterZ * 2)) + j);
	//			CTransform* FrontTrans = (CTransform*)FrontTile->Get_Component(COM_TRANSFORM);


	//			FrontTrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180));
	//			FrontTrans->Set_State(CTransform::STATE_POSITION, _float3((j * 1.f), (i * 1.f), 1.f));

	//		}
	//	}
	//}

	//// 뒷면 충돌체_0
	//{
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_B_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
	//		return E_FAIL;

	//	CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_B_Collider"), 0);
	//	// tile의 위치
	//	CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
	//	static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
	//	// collider의 위치
	//	CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

	//	transform->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.f, 1.f));

	//	box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ * 2, (_float)iLineY + 1, 0.1f));
	//	box->Set_AdditionalPos(_float3((iWaterZ * 0.5f) + 1, iLineY * 0.5f, 0.f));

	//}

	////뒷면_1
	//{
	//	for (_int i = 0; i < iLineY; ++i)
	//	{
	//		for (_int j = 0; j < (iWaterZ * 2); ++j)
	//		{
	//			if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Back1"), TEXT("Prototype_GameObject_Tile"))))
	//				return E_FAIL;
	//			CGameObject* FrontTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Back1"), (i * (iWaterZ * 2)) + j);
	//			CTransform* FrontTrans = (CTransform*)FrontTile->Get_Component(COM_TRANSFORM);

	//			FrontTrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180));
	//			FrontTrans->Set_State(CTransform::STATE_POSITION, _float3((iWaterZ * 3) + (j * 1.f), (i * 1.f), 1.f));

	//		}
	//	}
	//}

	////뒷면 충돌체_1
	//{
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_B1_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
	//		return E_FAIL;

	//	CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_B1_Collider"), 0);
	//	// tile의 위치
	//	CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
	//	static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
	//	// collider의 위치
	//	CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

	//	transform->Set_State(CTransform::STATE_POSITION, _float3((iWaterZ * 3) + 2.f, 0.f, 1.f));

	//	box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ * 2 + 1, (_float)iLineY + 1, 0.1f));
	//	box->Set_AdditionalPos(_float3((iWaterZ * 0.5f), iLineY * 0.5f, 0.f));
	//}

	////뒷면_2
	//{
	//	for (_int i = 0; i < iLineY; ++i)
	//	{
	//		for (_int j = 0; j < (iWaterZ * 2); ++j)
	//		{
	//			if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Back2"), TEXT("Prototype_GameObject_Tile"))))
	//				return E_FAIL;
	//			CGameObject* FrontTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Back2"), (i * (iWaterZ * 2)) + j);
	//			CTransform* FrontTrans = (CTransform*)FrontTile->Get_Component(COM_TRANSFORM);

	//			FrontTrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180));
	//			FrontTrans->Set_State(CTransform::STATE_POSITION, _float3((iWaterZ * 2) + (j * 1.f), 4.f + (i * 1.f) * 0.5f, 1.f));

	//		}
	//	}
	//}

	//// 뒷면 충돌체_2
	//{
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_B2_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
	//		return E_FAIL;

	//	CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_B2_Collider"), 0);
	//	// tile의 위치
	//	CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
	//	static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
	//	// collider의 위치
	//	CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

	//	transform->Set_State(CTransform::STATE_POSITION, _float3((iWaterZ * 2.f), 4.f, 1.f));

	//	box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ * 2, (_float)iLineY, 0.1f));
	//	box->Set_AdditionalPos(_float3((iWaterZ * 0.5f), iLineY * 0.5f, 0.f));
	//}

	////정면_0
	//{
	//	for (_int i = 0; i < iLineY; ++i)
	//	{
	//		for (_int j = 0; j < (iWaterZ * 2); ++j)
	//		{
	//			if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Front0"), TEXT("Prototype_GameObject_Tile"))))
	//				return E_FAIL;
	//			CGameObject* FrontTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Front0"), (i * (iWaterZ * 2)) + j);
	//			CTransform* FrontTrans = (CTransform*)FrontTile->Get_Component(COM_TRANSFORM);

	//			FrontTrans->Set_State(CTransform::STATE_POSITION, _float3((j * 1.f), (i * 1.f), (_float)iLineZ - 1));

	//		}
	//	}
	//}
	//// 정면 충돌체_0
	//{
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_F_Collider0"), TEXT("Prototype_GameObject_TileCollider"))))
	//		return E_FAIL;

	//	CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_F_Collider0"), 0);
	//	// tile의 위치
	//	CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
	//	static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
	//	// collider의 위치
	//	CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

	//	transform->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.f, (_float)iLineZ - 1));

	//	box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ * 2, (_float)iLineY + 1, 0.1f));
	//	box->Set_AdditionalPos(_float3((iWaterZ * 0.5f) + 1, iLineY * 0.5f, 0.f));

	//}

	////정면_1
	//{
	//	for (_int i = 0; i < iLineY; ++i)
	//	{
	//		for (_int j = 0; j < (iWaterZ * 2); ++j)
	//		{
	//			if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Front1"), TEXT("Prototype_GameObject_Tile"))))
	//				return E_FAIL;
	//			CGameObject* FrontTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Front1"), (i * (iWaterZ * 2)) + j);
	//			CTransform* FrontTrans = (CTransform*)FrontTile->Get_Component(COM_TRANSFORM);


	//			FrontTrans->Set_State(CTransform::STATE_POSITION, _float3((iWaterZ * 3) + (j * 1.f), (i * 1.f), (_float)iLineZ - 1));

	//		}
	//	}
	//}

	////정면 충돌체_1
	//{
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_F_Collider1"), TEXT("Prototype_GameObject_TileCollider"))))
	//		return E_FAIL;

	//	CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_F_Collider1"), 0);
	//	// tile의 위치
	//	CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
	//	static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
	//	// collider의 위치
	//	CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

	//	transform->Set_State(CTransform::STATE_POSITION, _float3((iWaterZ * 3) + 2.f, 0.f, (_float)iLineZ - 1));

	//	box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ * 2 + 1, (_float)iLineY + 1, 0.1f));
	//	box->Set_AdditionalPos(_float3((iWaterZ * 0.5f), iLineY * 0.5f, 0.f));
	//}

	////정면_2
	//{
	//	for (_int i = 0; i < iLineY; ++i)
	//	{
	//		for (_int j = 0; j < (iWaterZ * 2); ++j)
	//		{
	//			if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Front2"), TEXT("Prototype_GameObject_Tile"))))
	//				return E_FAIL;
	//			CGameObject* FrontTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Front2"), (i * (iWaterZ * 2)) + j);
	//			CTransform* FrontTrans = (CTransform*)FrontTile->Get_Component(COM_TRANSFORM);

	//			FrontTrans->Set_State(CTransform::STATE_POSITION, _float3((iWaterZ * 2) + (j * 1.f), 4.f + (i * 1.f) * 0.5f, (_float)iLineZ - 1));

	//		}
	//	}
	//}

	//// 정면 충돌체_2
	//{
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_F_Collider2"), TEXT("Prototype_GameObject_TileCollider"))))
	//		return E_FAIL;

	//	CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_F_Collider2"), 0);
	//	// tile의 위치
	//	CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
	//	static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
	//	// collider의 위치
	//	CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

	//	transform->Set_State(CTransform::STATE_POSITION, _float3((iWaterZ * 2.f), 4.f, (_float)iLineZ - 1));

	//	box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ * 2, (_float)iLineY, 0.1f));
	//	box->Set_AdditionalPos(_float3((iWaterZ * 0.5f), iLineY * 0.5f, 0.f));
	//}

	//// 왼쪽면
	//for (_int i = 0; i < iLineY; ++i)
	//{
	//	for (_int j = 0; j < iLineZ; ++j)
	//	{
	//		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Left"), TEXT("Prototype_GameObject_Tile"))))
	//			return E_FAIL;
	//		CGameObject* LeftTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Left"), (i * iLineZ) + j);
	//		CTransform* LeftTrans = (CTransform*)LeftTile->Get_Component(COM_TRANSFORM);

	//		LeftTrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(270));
	//		LeftTrans->Set_State(CTransform::STATE_POSITION, _float3(1.f, (i * 1.f), (j * 1.f)));

	//	}
	//}

	//// 왼쪽면 충돌체
	//{
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_L_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
	//		return E_FAIL;

	//	CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_L_Collider"), 0);
	//	// tile의 위치
	//	CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
	//	static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
	//	// collider의 위치
	//	CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

	//	transform->Set_State(CTransform::STATE_POSITION, _float3(1.f, 0.f, 0.f));

	//	box->Set_State(CBoxCollider::COLL_SIZE, _float3(0.f, (_float)iLineY, (_float)iLineZ));
	//	box->Set_AdditionalPos(_float3(0.f, iLineY * 0.5f, iLineZ * 0.5f));

	//}

	//// 오른쪽면
	//for (_int i = 0; i < iLineY; ++i)
	//{
	//	for (_int j = 0; j < iLineZ; ++j)
	//	{
	//		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Right"), TEXT("Prototype_GameObject_Tile"))))
	//			return E_FAIL;
	//		CGameObject* RightTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Right"), (i * iLineZ) + j);
	//		CTransform* RightTrans = (CTransform*)RightTile->Get_Component(COM_TRANSFORM);

	//		RightTrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90));
	//		RightTrans->Set_State(CTransform::STATE_POSITION, _float3((_float)iLineX - 1, (i * 1.f), (j * 1.f)));

	//	}
	//}

	//// 오른쪽면 충돌체 big problem 충돌은 되는데 포탈 찍히는게 타일 밖으로 보임
	//{
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_R_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
	//		return E_FAIL;

	//	CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_R_Collider"), 0);
	//	// tile의 위치
	//	CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
	//	static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
	//	// collider의 위치
	//	CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

	//	transform->Set_State(CTransform::STATE_POSITION, _float3((_float)iLineX - 1.f, 0.1f, 0.1f));

	//	box->Set_State(CBoxCollider::COLL_SIZE, _float3(0.5f, (_float)iLineY, (_float)iLineZ));
	//	box->Set_AdditionalPos(_float3(0.1f, iLineY * 0.5f, iLineZ * 0.5f));

	//}

	//// 바닥_0 
	//for (_int i = 0; i < (iWaterZ * 2); ++i)
	//{
	//	for (_int j = 0; j < iLineX; ++j)
	//	{
	//		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Bottom"), TEXT("Prototype_GameObject_Tile"))))
	//			return E_FAIL;
	//		CGameObject* BottomTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Bottom"), (i * iLineX) + j);
	//		CTransform* BottomTrans = (CTransform*)BottomTile->Get_Component(COM_TRANSFORM);
	//		static_cast<CTile*>(BottomTile)->Set_TextureIndex(1);
	//		BottomTrans->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90));
	//		BottomTrans->Set_State(CTransform::STATE_POSITION, _float3((j * 1.f), 1.f, (i * 1.f)));

	//	}
	//}

	//// 바닥면 충돌체_0
	//{
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_bot_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
	//		return E_FAIL;

	//	CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_bot_Collider"), 0);
	//	// tile의 위치
	//	CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
	//	static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
	//	// collider의 위치
	//	CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

	//	transform->Set_State(CTransform::STATE_POSITION, _float3(0.1f, 0.5f, 0.1f));

	//	box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iLineX, 1.f, (_float)(iWaterZ * 2.9)));
	//	box->Set_AdditionalPos(_float3(iLineX * 0.5f, 0.f, iWaterZ * 0.5f));

	//}

	//// 바닥_1 (물 밑 바닥)
	//for (_int i = 0; i < iWaterZ; ++i)
	//{
	//	for (_int j = 0; j < iLineX; ++j)
	//	{
	//		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Bottom1"), TEXT("Prototype_GameObject_Tile"))))
	//			return E_FAIL;
	//		CGameObject* BottomTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Bottom1"), (i * iLineX) + j);
	//		CTransform* BottomTrans = (CTransform*)BottomTile->Get_Component(COM_TRANSFORM);
	//		static_cast<CTile*>(BottomTile)->Set_TextureIndex(1);
	//		BottomTrans->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90));
	//		BottomTrans->Set_State(CTransform::STATE_POSITION, _float3((j * 1.f), -2.f, (iWaterZ * 2) + (i * 1.f)));

	//	}
	//}


	//// 바닥면 충돌체_1 (물 밑 바닥)
	//{
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_bot_Collider1"), TEXT("Prototype_GameObject_TileCollider"))))
	//		return E_FAIL;

	//	CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_bot_Collider1"), 0);
	//	// tile의 위치
	//	CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
	//	static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
	//	// collider의 위치
	//	CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

	//	transform->Set_State(CTransform::STATE_POSITION, _float3(0.1f, -2.f, (iWaterZ * 2.f)));

	//	box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iLineX, 1.f, (_float)(iWaterZ)));
	//	box->Set_AdditionalPos(_float3(iLineX * 0.5f, 0.f, iWaterZ * 0.5f));

	//}

	////물 옆 면_0
	//{
	//	for (_int i = 0; i < 1; ++i)
	//	{
	//		for (_int j = 0; j < iLineX; ++j)
	//		{
	//			if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Side_Bottom0"), TEXT("Prototype_GameObject_Tile"))))
	//				return E_FAIL;
	//			CGameObject* FrontTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Side_Bottom0"), (i * iLineX) + j);
	//			CTransform* FrontTrans = (CTransform*)FrontTile->Get_Component(COM_TRANSFORM);

	//			static_cast<CTile*>(FrontTile)->Set_TextureIndex(1);
	//			FrontTrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180));
	//			FrontTrans->Set_State(CTransform::STATE_POSITION, _float3((j * 1.f), 0.5f, (iWaterZ * 2) - 0.5f));

	//		}
	//	}
	//}

	//// 물
	//{
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Water"), TEXT("Prototype_GameObject_Water"))))
	//		return E_FAIL;

	//	CGameObject* Water = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Water"), 0);
	//	CTransform* WaterTrans = (CTransform*)Water->Get_Component(COM_TRANSFORM);
	//	WaterTrans->Scaled(_float3(25.f, 2.f, 5.f));
	//	WaterTrans->Set_State(CTransform::STATE_POSITION, _float3((iWaterZ * 2.5f), -0.5f, (_float)iWaterZ * 2.4f));

	//	CBoxCollider* box = static_cast<CBoxCollider*>(Water->Get_Component(COM_COLLIDER));

	//	box->Set_State(CBoxCollider::COLL_SIZE, _float3(25.f, 1.f, 5.f));
	//}

	////물 옆 면_1
	//{
	//	for (_int i = 0; i < 1; ++i)
	//	{
	//		for (_int j = 0; j < iLineX; ++j)
	//		{
	//			if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Side_Bottom1"), TEXT("Prototype_GameObject_Tile"))))
	//				return E_FAIL;
	//			CGameObject* FrontTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Side_Bottom1"), (i * iLineX) + j);
	//			CTransform* FrontTrans = (CTransform*)FrontTile->Get_Component(COM_TRANSFORM);

	//			//FrontTrans->Scaled(_float3(2.f, 2.f, 2.f));
	//			static_cast<CTile*>(FrontTile)->Set_TextureIndex(1);
	//			FrontTrans->Set_State(CTransform::STATE_POSITION, _float3((j * 1.f), 0.5f, (iWaterZ * 3) - 0.5f));

	//		}
	//	}
	//}

	//// 바닥_2 
	//for (_int i = 0; i < (iWaterZ * 2); ++i)
	//{
	//	for (_int j = 0; j < iLineX; ++j)
	//	{
	//		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Bottom2"), TEXT("Prototype_GameObject_Tile"))))
	//			return E_FAIL;
	//		CGameObject* BottomTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Bottom2"), (i * iLineX) + j);
	//		CTransform* BottomTrans = (CTransform*)BottomTile->Get_Component(COM_TRANSFORM);
	//		static_cast<CTile*>(BottomTile)->Set_TextureIndex(1);
	//		BottomTrans->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90));
	//		BottomTrans->Set_State(CTransform::STATE_POSITION, _float3((j * 1.f), 1.f, (iWaterZ * 3) + (i * 1.f)));

	//	}
	//}

	//// 바닥면 충돌체_2
	//{
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_bot_Collider2"), TEXT("Prototype_GameObject_TileCollider"))))
	//		return E_FAIL;

	//	CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_bot_Collider2"), 0);
	//	// tile의 위치
	//	CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
	//	static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
	//	// collider의 위치
	//	CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

	//	transform->Set_State(CTransform::STATE_POSITION, _float3(0.1f, 0.5f, (iWaterZ * 3) + 0.1f));

	//	box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iLineX, 1.f, (_float)(iWaterZ * 2.9)));
	//	box->Set_AdditionalPos(_float3(iLineX * 0.5f, 0.f, iWaterZ * 0.5f * 2.6f));

	//}

	//// 천장
	//for (_int i = 0; i < iLineZ; ++i)
	//{
	//	for (_int j = 0; j < iLineX + 2; ++j)
	//	{
	//		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Top"), TEXT("Prototype_GameObject_Tile"))))
	//			return E_FAIL;
	//		CGameObject* TopTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Top"), (i * (iLineX + 2)) + j);
	//		CTransform* TopTrans = (CTransform*)TopTile->Get_Component(COM_TRANSFORM);

	//		TopTrans->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(270));
	//		TopTrans->Set_State(CTransform::STATE_POSITION, _float3((j * 1.f), (_float)iLineY - 1, (i * 1.f)));

	//	}
	//}

	//// 천장 충돌체 오른쪽 면과 같은 문제
	//{
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_T_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
	//		return E_FAIL;

	//	CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_T_Collider"), 0);
	//	// tile의 위치
	//	CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
	//	static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
	//	// collider의 위치
	//	CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

	//	transform->Set_State(CTransform::STATE_POSITION, _float3(0.f, (_float)iLineY - 1, 0.f));

	//	box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iLineX, 0.1f, (_float)iLineZ));
	//	box->Set_AdditionalPos(_float3(iLineX * 0.5f, 0.f, iLineZ * 0.5f));

	//}

	/*if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Bottom2"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;
	CGameObject* BottomTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Bottom2"));
	CTransform* BottomTrans = (CTransform*)BottomTile->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(BottomTile)->Set_TextureIndex(1);
	BottomTrans->Scaled(_float3(60.f, 60.f, 60.f));
	CBoxCollider* box = static_cast<CBoxCollider*>(BottomTile->Get_Component(COM_COLLIDER));
	box->Set_State(CBoxCollider::COLL_SIZE, _float3(60.f, 60.f, 60.f));
	box->Set_AdditionalPos(_float3(0.f,-30.f,0.f));
	BottomTrans->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	BottomTrans->Set_State(CTransform::STATE_POSITION, _float3(0.f,-1.f,0.f));


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Right"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* RightTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Right"));
	CTransform* RightTrans = (CTransform*)RightTile->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(RightTile)->Set_TextureIndex(1);
	RightTrans->Scaled(_float3(60.f, 60.f, 60.f));
	CBoxCollider* box2 = static_cast<CBoxCollider*>(RightTile->Get_Component(COM_COLLIDER));
	box2->Set_State(CBoxCollider::COLL_SIZE, _float3(60.f, 60.f, 60.f));
	RightTrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
	box2->Set_AdditionalPos(_float3(30.f, 0.f, 0.f));
	RightTrans->Set_State(CTransform::STATE_POSITION, _float3(15.f, 0.f, 0.f));


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Bottom1"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* BottomTile1 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Bottom1"));
	CTransform* BottomTrans1 = (CTransform*)BottomTile1->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(BottomTile1)->Set_TextureIndex(1);
	BottomTrans1->Scaled(_float3(20.f, 20.f, 20.f));
	CBoxCollider* box3 = static_cast<CBoxCollider*>(BottomTile1->Get_Component(COM_COLLIDER));
	box3->Set_State(CBoxCollider::COLL_SIZE, _float3(20.f, 1.f, 20.f));
	box3->Set_AdditionalPos(_float3(0.f, 0.f, 0.f));
	BottomTrans1->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	BottomTrans1->Set_State(CTransform::STATE_POSITION, _float3(0.f, 25.f, 0.f));*/

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_StageTwo::Open_Exit()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CGameObject* Switch = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_Open_Exit"));

	static_cast<CTile_Cube*>(Switch)->Open_Event(g_CurrLevel, TEXT("Layer_Left_Exit"), TEXT("Layer_Right_Exit"));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_StageTwo* CLevel_StageTwo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_StageTwo*	pInstance = new CLevel_StageTwo(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_StageTwo::Free()
{
	__super::Free();
}

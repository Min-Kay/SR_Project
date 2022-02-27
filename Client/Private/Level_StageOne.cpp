#include "stdafx.h"
#include "..\Public\Level_StageOne.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Camera_Player.h"
#include "UI.h"
#include "Player.h"
#include "Tile.h"

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

	if (FAILED(Ready_Layer_Map()))
		return E_FAIL;

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

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("SkyBox"), TEXT("Prototype_GameObject_Sky"))))
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


	CTransform* tr = static_cast<CTransform*>(pGameInstance->Get_GameObject(LEVEL_STAGEONE, pLayerTag, 0)->Get_Component(COM_TRANSFORM));

	tr->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.f, 0.f));
	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_StageOne::Ready_Layer_Map()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// Start Left Door
	//{

	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Left_Door"), TEXT("Prototype_GameObject_Left_Door"))))
	//		return E_FAIL;

	//	CGameObject* Left_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Left_Door"));
	//	CTransform* Lefttrans = (CTransform*)Left_Door->Get_Component(COM_TRANSFORM);

	//	Lefttrans->Scaled(_float3(5.f, 5.f, 2.f));
	//	//Lefttrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180));
	//	Lefttrans->Set_State(CTransform::STATE_POSITION, _float3(4.f, 4.f, -0.5f));
	//}
	//// Start Right Door
	//{
	//	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Right_Door"), TEXT("Prototype_GameObject_Right_Door"))))
	//		return E_FAIL;

	//	CGameObject* Right_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Right_Door"));
	//	CTransform* Righttrans = (CTransform*)Right_Door->Get_Component(COM_TRANSFORM);

	//	Righttrans->Scaled(_float3(5.f, 5.f, 2.f));
	//	//Righttrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180));
	//	Righttrans->Set_State(CTransform::STATE_POSITION, _float3(9.f, 4.f, -0.5f));
	//}



	// 입구 Tile_Cube_0
	//{
	//	for (_uint i = 0; i < 10; ++i)
	//	{
	//		for (_uint j = 0; j < 10; ++j)
	//		{
	//			if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Block0"), TEXT("Prototype_GameObject_Block"))))
	//				return E_FAIL;
	//			CGameObject* FrontTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Block0"), (i * 10) + j);
	//			CTransform* FrontTrans = (CTransform*)FrontTile->Get_Component(COM_TRANSFORM);

	//			FrontTrans->Scaled(_float3(1.f, 1.f, 1.f));
	//			FrontTrans->Set_State(CTransform::STATE_POSITION, _float3(4 + (j * 1.f), 0.f, (i * -0.5f)));

	//		}
	//	}
	//}

	//// 입구 Tile_Cube_1
	//{
	//	for (_uint i = 0; i < 10; ++i)
	//	{
	//		for (_uint j = 0; j < 10; ++j)
	//		{
	//			if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Block1"), TEXT("Prototype_GameObject_Block"))))
	//				return E_FAIL;
	//			CGameObject* FrontTile = pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Block1"), (i * 10) + j);
	//			CTransform* FrontTrans = (CTransform*)FrontTile->Get_Component(COM_TRANSFORM);

	//			FrontTrans->Scaled(_float3(1.f, 1.f, 1.f));
	//			FrontTrans->Set_State(CTransform::STATE_POSITION, _float3(11.5f, (j * 1.f), (i * -1.f)));

	//		}
	//	}
	//}

	//후면_0
	/*{
		for (_uint i = 0; i < 15; ++i)
		{
			for (_uint j = 0; j < 35; ++j)
			{
				if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Back0"), TEXT("Prototype_GameObject_Tile"))))
					return E_FAIL;
				CGameObject* FrontTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Back0"), (i * 35) + j);
				CTransform* FrontTrans = (CTransform*)FrontTile->Get_Component(COM_TRANSFORM);

				FrontTrans->Scaled(_float3(2.f, 2.f, 2.f));
				FrontTrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180));
				FrontTrans->Set_State(CTransform::STATE_POSITION, _float3(12.5 + (j * 1.f), (i * 1.f), 1.f));

			}
		}
	}*/

	// 입구 오른쪽
	/*for (_uint i = 0; i < 10; ++i)
	{
		for (_uint j = 0; j < 10; ++j)
		{
			if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_L"), TEXT("Prototype_GameObject_Tile"))))
				return E_FAIL;
			CGameObject* LeftTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_L"), (i * 10) + j);
			CTransform* LeftTrans = (CTransform*)LeftTile->Get_Component(COM_TRANSFORM);

			LeftTrans->Scaled(_float3(2.f, 2.f, 2.f));
			LeftTrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90));
			LeftTrans->Set_State(CTransform::STATE_POSITION, _float3(11.5f, (j * 1.f), (i * -1.f)));

		}
	}*/

	//후면_1
	/*{
		for (_uint i = 0; i < 10; ++i)
		{
			for (_uint j = 0; j < 17; ++j)
			{
				if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Back1"), TEXT("Prototype_GameObject_Tile"))))
					return E_FAIL;
				CGameObject* FrontTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Back1"), (i * 17) + j);
				CTransform* FrontTrans = (CTransform*)FrontTile->Get_Component(COM_TRANSFORM);

				FrontTrans->Scaled(_float3(2.f, 2.f, 2.f));
				FrontTrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180));
				FrontTrans->Set_State(CTransform::STATE_POSITION, _float3((j * 1.f), 7 + (i * 1.f), 1.f));

			}
		}
	}*/


	//정면 
	//for (_uint i = 0; i < 15; ++i)
	//{
	//	for (_uint j = 0; j < 60; ++j)
	//	{
	//		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Front"), TEXT("Prototype_GameObject_Tile"))))
	//			return E_FAIL;
	//		CGameObject* BackTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Front"), (i * 60) + j);
	//		CTransform* BackTrans = (CTransform*)BackTile->Get_Component(COM_TRANSFORM);

	//		BackTrans->Scaled(_float3(2.f, 2.f, 2.f));
	//		BackTrans->Set_State(CTransform::STATE_POSITION, _float3((j * 1.f), (i * 1.f), 60.f));
	//	}
	//}

	//// 왼쪽면
	//for (_uint i = 0; i < 15; ++i)
	//{
	//	for (_uint j = 0; j < 60; ++j)
	//	{
	//		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Left"), TEXT("Prototype_GameObject_Tile"))))
	//			return E_FAIL;
	//		CGameObject* LeftTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Left"), (i * 60) + j);
	//		CTransform* LeftTrans = (CTransform*)LeftTile->Get_Component(COM_TRANSFORM);

	//		LeftTrans->Scaled(_float3(2.f, 2.f, 2.f));
	//		LeftTrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(270));
	//		LeftTrans->Set_State(CTransform::STATE_POSITION, _float3(1.f, (i * 1.f), (j * 1.f)));

	//	}
	//}

	//// 오른쪽면
	//for (_uint i = 0; i < 15; ++i)
	//{
	//	for (_uint j = 0; j < 60; ++j)
	//	{
	//		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Right"), TEXT("Prototype_GameObject_Tile"))))
	//			return E_FAIL;
	//		CGameObject* RightTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Right"), (i * 60) + j);
	//		CTransform* RightTrans = (CTransform*)RightTile->Get_Component(COM_TRANSFORM);

	//		RightTrans->Scaled(_float3(2.f, 2.f, 2.f));
	//		RightTrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90));
	//		RightTrans->Set_State(CTransform::STATE_POSITION, _float3(60.f, (i * 1.f), (j * 1.f)));

	//	}
	//}


	//// 아랫면_0
	//for (_uint i = 0; i < 30; ++i)
	//{
	//	for (_uint j = 0; j < 60; ++j)
	//	{
	//		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Bottom"), TEXT("Prototype_GameObject_Tile"))))
	//			return E_FAIL;
	//		CGameObject* BottomTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Bottom"), (i * 60) + j);
	//		CTransform* BottomTrans = (CTransform*)BottomTile->Get_Component(COM_TRANSFORM);
	//		static_cast<CTile*>(BottomTile)->Set_TextureIndex(1);
	//		BottomTrans->Scaled(_float3(2.f, 2.f, 2.f));
	//		BottomTrans->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90));
	//		BottomTrans->Set_State(CTransform::STATE_POSITION, _float3((j * 1.f), 1.f, (i * 1.f)));

	//	}
	//}

	//// 아랫면_1
	//for (_uint i = 0; i < 5; ++i)
	//{
	//	for (_uint j = 0; j < 60; ++j)
	//	{
	//		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Bottom2"), TEXT("Prototype_GameObject_Tile"))))
	//			return E_FAIL;
	//		CGameObject* BottomTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Bottom2"), (i * 60) + j);
	//		CTransform* BottomTrans = (CTransform*)BottomTile->Get_Component(COM_TRANSFORM);
	//		static_cast<CTile*>(BottomTile)->Set_TextureIndex(1);
	//		BottomTrans->Scaled(_float3(2.f, 2.f, 2.f));
	//		BottomTrans->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90));
	//		BottomTrans->Set_State(CTransform::STATE_POSITION, _float3((j * 1.f), 0.f, 30 + (i * 1.f)));

	//	}
	//}

	//물 속 면_0
	/*{
		for (_uint i = 0; i < 1; ++i)
		{
			for (_uint j = 0; j < 60; ++j)
			{
				if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Side_Bottom0"), TEXT("Prototype_GameObject_Tile"))))
					return E_FAIL;
				CGameObject* FrontTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Side_Bottom0"), (i * 35) + j);
				CTransform* FrontTrans = (CTransform*)FrontTile->Get_Component(COM_TRANSFORM);

				static_cast<CTile*>(FrontTile)->Set_TextureIndex(1);
				FrontTrans->Scaled(_float3(2.f, 2.f, 2.f));
				FrontTrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180));
				FrontTrans->Set_State(CTransform::STATE_POSITION, _float3((j * 1.f), 0.f, 30 + (i * 1.f)));

			}
		}
	}*/

	// 물
	/*{
		for (_uint i = 0; i < 5; ++i)
		{
			for (_uint j = 0; j < 60; ++j)
			{
				if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Water"), TEXT("Prototype_GameObject_Water"))))
					return E_FAIL;

				CGameObject* Water = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Water"), (i * 60) + j);
				CTransform* WaterTrans = (CTransform*)Water->Get_Component(COM_TRANSFORM);
				WaterTrans->Set_State(CTransform::STATE_POSITION, _float3((j * 1.f), 0.f, 30 + (i * 1.f)));

			}
		}
	}*/

	//물 속 면_1
	//{
	//	for (_uint i = 0; i < 1; ++i)
	//	{
	//		for (_uint j = 0; j < 60; ++j)
	//		{
	//			if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Side_Bottom1"), TEXT("Prototype_GameObject_Tile"))))
	//				return E_FAIL;
	//			CGameObject* FrontTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Side_Bottom1"), (i * 35) + j);
	//			CTransform* FrontTrans = (CTransform*)FrontTile->Get_Component(COM_TRANSFORM);

	//			static_cast<CTile*>(FrontTile)->Set_TextureIndex(1);
	//			FrontTrans->Scaled(_float3(2.f, 2.f, 2.f));
	//			//FrontTrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180));
	//			FrontTrans->Set_State(CTransform::STATE_POSITION, _float3((j * 1.f), 0.f, 34 + (i * 1.f)));

	//		}
	//	}
	//}

	//// 아랫면_2
	//for (_uint i = 0; i < 25; ++i)
	//{
	//	for (_uint j = 0; j < 60; ++j)
	//	{
	//		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Bottom2"), TEXT("Prototype_GameObject_Tile"))))
	//			return E_FAIL;
	//		CGameObject* BottomTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Bottom2"), (i * 60) + j);
	//		CTransform* BottomTrans = (CTransform*)BottomTile->Get_Component(COM_TRANSFORM);
	//		static_cast<CTile*>(BottomTile)->Set_TextureIndex(1);
	//		BottomTrans->Scaled(_float3(2.f, 2.f, 2.f));
	//		BottomTrans->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90));
	//		BottomTrans->Set_State(CTransform::STATE_POSITION, _float3((j * 1.f), 1.f, 35 + (i * 1.f)));

	//	}
	//}

	//// 윗면
	//for (_uint i = 0; i < 60; ++i)
	//{
	//	for (_uint j = 0; j < 60; ++j)
	//	{
	//		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Top"), TEXT("Prototype_GameObject_Tile"))))
	//			return E_FAIL;
	//		CGameObject* TopTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Top"), (i * 60) + j);
	//		CTransform* TopTrans = (CTransform*)TopTile->Get_Component(COM_TRANSFORM);

	//		TopTrans->Scaled(_float3(2.f, 2.f, 2.f));
	//		TopTrans->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(270));
	//		TopTrans->Set_State(CTransform::STATE_POSITION, _float3((j * 1.f), 15.f, (i * 1.f)));

	//	}
	//}


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Bottom2"), TEXT("Prototype_GameObject_Tile"))))
			return E_FAIL;
	CGameObject* BottomTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Bottom2"));
	CTransform* BottomTrans = (CTransform*)BottomTile->Get_Component(COM_TRANSFORM);
	static_cast<CTile*>(BottomTile)->Set_TextureIndex(1);
	BottomTrans->Scaled(_float3(60.f, 60.f, 60.f));
	CBoxCollider* box = static_cast<CBoxCollider*>(BottomTile->Get_Component(COM_COLLIDER));
	box->Set_State(CBoxCollider::COLL_SIZE, _float3(60.f, 60.f, 60.f));
	box->Set_AdditionalPos(_float3(0.f,-30.f,0.f));
	BottomTrans->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	BottomTrans->Set_State(CTransform::STATE_POSITION, _float3(0.f,-1.f,0.f));

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Right"), TEXT("Prototype_GameObject_Tile"))))
		return E_FAIL;
	CGameObject* RightTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Right"));
	CTransform* RightTrans = (CTransform*)RightTile->Get_Component(COM_TRANSFORM);
	static_cast<CTile*>(RightTile)->Set_TextureIndex(1);
	RightTrans->Scaled(_float3(60.f, 60.f, 60.f));
	CBoxCollider* box2 = static_cast<CBoxCollider*>(RightTile->Get_Component(COM_COLLIDER));
	box2->Set_State(CBoxCollider::COLL_SIZE, _float3(60.f, 60.f, 60.f));
	RightTrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
	box2->Set_AdditionalPos(_float3(30.f, 0.f, 0.f));
	RightTrans->Set_State(CTransform::STATE_POSITION, _float3(15.f, 28.f, 0.f));
		

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

#include "stdafx.h"
#include "..\Public\Level_StageOne.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Camera_Player.h"
#include "CubeMonster.h"
#include "UI.h"
#include "Player.h"
#include "Tile.h"
#include "Tile_Collider.h"
#include "Door_left.h"
#include "Door_right.h"
#include "Level_Loading.h"
#include "Tile_Cube.h"
#include "ChangeLevel.h"
#include "Impact.h"
#include "Water.h"

CLevel_StageOne::CLevel_StageOne(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{

}

HRESULT CLevel_StageOne::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Map()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster_Map()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Event()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Second_Entrance()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Ball_Map()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Gravity_Map()))
		return E_FAIL;



	if (FAILED(Ready_Layer_Next_Stage()))
	{
		MSGBOX("Failed to Ready_Layer_Next_Stage in CLevel_StageOne");
		return E_FAIL;
	}

	SetWindowText(g_hWnd, TEXT("PORTAL_STAGE1"));

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	pInstance->SetMouseMode(false, g_hWnd);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CLevel_StageOne::Tick(_float fTimeDelta)
{
	if (0 > fTimeDelta)
		return 0;

	if (!m_setting)
	{
		CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
		p_instance->SetMouseMode(false, g_hWnd);
		RELEASE_INSTANCE(CGameInstance);
		m_setting = true;
	}

	if (0 > __super::Tick(fTimeDelta))
		return -1;

	if (!m_Open_1)
	{
		if (FAILED(Open_Exit()))
		{
			MSGBOX("Failed to Open_Exit in CLevel_StageOne");
			return E_FAIL;
		}
	}

	if (!m_Close_1 || !m_Open_2)
	{
		m_player->Get_Info().Pos = _float3(15.f, 1.f, 35.f);
		if (FAILED(Close_Exit_Open_Door2()))
		{
			MSGBOX("Failed to Close_Exit_Open_Door2 in CLevel_StageOne");
			return E_FAIL;
		}
	}

	if (m_Open_2 && !m_Open_3)
	{
		if (FAILED(Open_Exit2()))
		{
			MSGBOX("Failed to Open_Exit2 in CLevel_StageOne");
			return E_FAIL;
		}
	}




	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);

	if (p_instance->Get_Key_Down(DIK_0))
	{
		if (FAILED(p_instance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_STAGETWO))))
		{
			RELEASE_INSTANCE(CGameInstance);
			return -1;
		}
		RELEASE_INSTANCE(CGameInstance);
		return 0;
	}

	if (m_Change->Get_LevelOut())
	{

		if (FAILED(p_instance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_STAGETWO))))
		{
			RELEASE_INSTANCE(CGameInstance);
			return -1;
		}
		RELEASE_INSTANCE(CGameInstance);
		return 0;
	}

	RELEASE_INSTANCE(CGameInstance);


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

HRESULT CLevel_StageOne::Ready_Layer_Camera(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

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
	CameraDesc.iLevel = LEVEL_STAGEONE;

	if (FAILED(pGameInstance->Add_Camera_Object(CAM_PLAYER, MAIN_CAM, &CameraDesc)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_StageOne::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("SkyBox"), TEXT("Prototype_GameObject_Sky"))))
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
	desc2.Shader_Style = SHADER_SETCOLOR_BLEND;
	desc2.Shader_Control = &g_ControlShader;



	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("BackUI"), TEXT("Prototype_GameObject_BackUI"), &desc2)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	/*if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("CubeMonster"), TEXT("Prototype_GameObject_CubeMonster"))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	CCubeMonster* monster = static_cast<CCubeMonster*>(pGameInstance->Get_GameObject_End(LEVEL_STAGEONE, TEXT("CubeMonster")));
	monster->Set_InitPos(_float3(10.f, 10.f, 10.f));

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("CubeMonster"), TEXT("Prototype_GameObject_CubeMonster"))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}


	CCubeMonster* monster1 = static_cast<CCubeMonster*>(pGameInstance->Get_GameObject_End(LEVEL_STAGEONE, TEXT("CubeMonster")));
	monster1->Set_InitPos(_float3(15.f, 15.f, 10.f));


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("CubeMonster"), TEXT("Prototype_GameObject_CubeMonster"))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	CCubeMonster* monster2 = static_cast<CCubeMonster*>(pGameInstance->Get_GameObject_End(LEVEL_STAGEONE, TEXT("CubeMonster")));
	monster2->Set_InitPos(_float3(10.f, 10.f, 15.f));


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("CubeMonster"), TEXT("Prototype_GameObject_CubeMonster"))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}


	CCubeMonster* monster3 = static_cast<CCubeMonster*>(pGameInstance->Get_GameObject_End(LEVEL_STAGEONE, TEXT("CubeMonster")));
	monster3->Set_InitPos(_float3(10.f, 10.f, 20.f));*/


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_StageOne::Ready_Layer_Player(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	CPlayer::PLAYERINFO INFO;
	ZeroMemory(&INFO, sizeof(INFO));

	INFO.Pos = _float3(10.0f, 2.f, 2.f);
	INFO.Hp = 100;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, pLayerTag, TEXT("Prototype_GameObject_Player"), &INFO)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_player = static_cast<CPlayer*>(pGameInstance->Get_GameObject_End(LEVEL_STAGEONE, pLayerTag));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_StageOne::Ready_Layer_Map()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// 시작 바닥
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_Bot_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_Bot_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		transform->Set_State(CTransform::STATE_POSITION, _float3((iWaterZ * 1.9f), 1.f, 0.f));
		transform->Scaled(_float3(30.f, 20.f, 30.f));
		transform->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90));

		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		box->Set_State(CBoxCollider::COLL_SIZE, _float3(7.f, 3.f, 6.f));
		box->Set_AdditionalPos(_float3((iWaterZ * 0.5f), 0.5f, (-iWaterZ * 0.5f)));

	}


	// 입구 오른쪽면 충돌체
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_R_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_R_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		transform->Set_State(CTransform::STATE_POSITION, _float3(_float3((iWaterZ * 3.f), 0.f, -4.f)));
		transform->Scaled(_float3(10.f, 10.f, 15.f));
		transform->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90));
		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		box->Set_State(CBoxCollider::COLL_SIZE, _float3(1.f, 7.f, 6.f));
		box->Set_AdditionalPos(_float3(-0.5f, 5.f, 0.f));

	}

	// 입구 왼쪽면 충돌체
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_L_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_L_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		transform->Set_State(CTransform::STATE_POSITION, _float3((iWaterZ * 0.5f) + ((_float)iWaterZ) + 2.f, ((_float)iWaterZ) * 0.5f, -(iWaterZ * 0.5f) - 2.f));
		transform->Scaled(_float3(10.f, 10.f, 15.f));
		transform->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(270));

		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3(1.f, 5.f, 6.f));
		box->Set_AdditionalPos(_float3(1.f, 1.f, 0.f));
	}

	// 입구 뒷면 충돌체
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_B_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Enter_B_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		transform->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180));
		transform->Set_State(CTransform::STATE_POSITION, _float3((((_float)iWaterZ * 1.9f)), 1.f, -(_float)iWaterZ));
		transform->Scaled(_float3(10.f, 15.f, 10.f));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ + 1, (_float)iWaterZ, 0.1f));
		box->Set_AdditionalPos(_float3((iWaterZ * 0.5f), (iWaterZ * 0.5f), 0.f));

	}


	// 천장
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_T_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_T_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		transform->Set_State(CTransform::STATE_POSITION, _float3(0.f, (_float)iLineY - 1 - 4, 0.f));
		transform->Scaled(_float3(70.f, 60.f, 40.f));
		transform->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(270));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iLineX, 1.f, (_float)iLineZ));
		box->Set_AdditionalPos(_float3(iLineX * 0.5f, 0.5f, iLineZ * 0.5f));

	}


	// 뒷면
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_B_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_B_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		transform->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.f, 1.f));
		transform->Scaled(_float3(50.f, 15.f, 15.f));
		transform->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(270));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ * 2 + 6, (_float)iLineY + 1, 0.1f));
		box->Set_AdditionalPos(_float3((iWaterZ * 0.5f) + 1 + 5, iLineY * 0.5f, 0.f));

	}

	//뒷면
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_B1_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_B1_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		transform->Set_State(CTransform::STATE_POSITION, _float3((iWaterZ * 3) + 2.f + 7.f, 0.f, 1.f));
		transform->Scaled(_float3(50.f, 30.f, 30.f));
		transform->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ * 2 + 1 + 20.f, (_float)iLineY + 1, 0.5f));
		box->Set_AdditionalPos(_float3((iWaterZ * 0.5f), iLineY * 0.5f, 0.f));
	}


	// 정면 충돌체_0
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_F_Collider0"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_F_Collider0"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		transform->Set_State(CTransform::STATE_POSITION, _float3(5.f, 0.f, (_float)iLineZ - 1));
		transform->Scaled(_float3(10.f, 10.f, 10.f));

		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ * 2, (_float)iLineY + 1, 0.1f));
		box->Set_AdditionalPos(_float3(0.f, iLineY * 0.5f, 0.f));

	}

	//정면 충돌체_1
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_F_Collider1"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_F_Collider1"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		transform->Set_State(CTransform::STATE_POSITION, _float3((iWaterZ * 3) + 2.f + 3.f, 0.f, (_float)iLineZ - 1));
		transform->Scaled(_float3(10.f, 10.f, 10.f));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ * 2 + 1 + 3, (_float)iLineY + 1, 0.1f));
		box->Set_AdditionalPos(_float3((iWaterZ * 0.5f) - 2.f, iLineY * 0.5f, 0.f));
	}


	// 정면 충돌체_2
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_F_Collider2"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_F_Collider2"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		transform->Set_State(CTransform::STATE_POSITION, _float3((iWaterZ * 2.f) + 2.5f, 4.5f, (_float)iLineZ - 1));
		transform->Scaled(_float3(5.f, 1.f, 5.f));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ * 2, (_float)iLineY, 0.1f));
		box->Set_AdditionalPos(_float3((iWaterZ * 0.5f), iLineY * 0.5f, 0.f));
	}


	// 왼쪽면 충돌체
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_L_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_L_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		transform->Set_State(CTransform::STATE_POSITION, _float3(1.f, 0.f, 0.f));
		transform->Scaled(_float3(30.f, 1.f, 30.f));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3(0.f, (_float)iLineY, (_float)iLineZ));
		box->Set_AdditionalPos(_float3(-1.f, iLineY * 0.5f, iLineZ * 0.5f));

	}

	// 바닥면 충돌체_2
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_bot_Collider2"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_bot_Collider2"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		transform->Set_State(CTransform::STATE_POSITION, _float3(0.1f, 1.f, (iWaterZ * 3.f) + 4.f));
		transform->Scaled(_float3(50.f, 11.f, 50.f));
		transform->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90));

		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iLineX, 1.f, (_float)(iWaterZ * 2.f) + 1.f));
		box->Set_AdditionalPos(_float3(iLineX * 0.5f, -0.5f, 0));

	}

	// 오른쪽면
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_R_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_R_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		transform->Set_State(CTransform::STATE_POSITION, _float3((_float)iLineX - 1.f, 0.1f, 1.f));
		transform->Scaled(_float3(50.f, 30.f, 10.f));
		transform->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3(0.5f, (_float)iLineY, (_float)iLineZ));
		box->Set_AdditionalPos(_float3(0.f, iLineY * 0.5f, iLineZ * 0.5f));

	}

	// 바닥면 충돌체_0
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_bot_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_bot_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		transform->Set_State(CTransform::STATE_POSITION, _float3(0.1f, 0.5f, 0.1f));
		transform->Scaled(_float3(30.f, 1.f, 30.f));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iLineX, 1.f, (_float)(iWaterZ * 2.9)));
		box->Set_AdditionalPos(_float3(iLineX * 0.5f, 0.f, iWaterZ * 0.5f));

	}


	// 바닥면 충돌체_1 (물 밑 바닥)
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_bot_Collider1"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_bot_Collider1"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		transform->Set_State(CTransform::STATE_POSITION, _float3(0.1f, -2.f, (iWaterZ * 2.f)));
		transform->Scaled(_float3(30.f, 1.f, 30.f));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iLineX, 1.f, (_float)(iWaterZ)));
		box->Set_AdditionalPos(_float3(iLineX * 0.5f, 0.f, iWaterZ * 0.5f));

	}

	//물 옆 면_0
	{
		for (_int i = 0; i < 1; ++i)
		{
			for (_int j = 0; j < iLineX; ++j)
			{
				if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Side_Bottom0"), TEXT("Prototype_GameObject_Tile"))))
					return E_FAIL;
				CGameObject* FrontTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Side_Bottom0"), (i * iLineX) + j);
				CTransform* FrontTrans = (CTransform*)FrontTile->Get_Component(COM_TRANSFORM);

				static_cast<CTile*>(FrontTile)->Set_TextureIndex(1);
				FrontTrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180));
				FrontTrans->Set_State(CTransform::STATE_POSITION, _float3((j * 1.f), 0.5f, (iWaterZ * 2) - 0.5f));

			}
		}
	}

	// 물
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Water"), TEXT("Prototype_GameObject_Water"))))
			return E_FAIL;

		CGameObject* Water = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Water"), 0);
		CTransform* WaterTrans = (CTransform*)Water->Get_Component(COM_TRANSFORM);
		WaterTrans->Scaled(_float3(25.f, 2.f, 5.f));
		WaterTrans->Set_State(CTransform::STATE_POSITION, _float3((iWaterZ * 2.5f), -0.5f, (_float)iWaterZ * 2.4f));

		CBoxCollider* box = static_cast<CBoxCollider*>(Water->Get_Component(COM_COLLIDER));

		box->Set_State(CBoxCollider::COLL_SIZE, _float3(25.f, 1.f, 5.f));
	}

	//물 옆 면_1
	{
		for (_int i = 0; i < 1; ++i)
		{
			for (_int j = 0; j < iLineX; ++j)
			{
				if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Side_Bottom1"), TEXT("Prototype_GameObject_Tile"))))
					return E_FAIL;
				CGameObject* FrontTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Side_Bottom1"), (i * iLineX) + j);
				CTransform* FrontTrans = (CTransform*)FrontTile->Get_Component(COM_TRANSFORM);

				//FrontTrans->Scaled(_float3(2.f, 2.f, 2.f));
				static_cast<CTile*>(FrontTile)->Set_TextureIndex(1);
				FrontTrans->Set_State(CTransform::STATE_POSITION, _float3((j * 1.f), 0.5f, (iWaterZ * 3) - 0.5f));

			}
		}
	}




	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_StageOne::Ready_Layer_Event()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	// Start Left Door
	{

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Left_Door"), TEXT("Prototype_GameObject_Left_Door"))))
			return E_FAIL;

		CGameObject* Left_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Left_Door"));
		CTransform* Lefttrans = (CTransform*)Left_Door->Get_Component(COM_TRANSFORM);

		Lefttrans->Scaled(_float3(fDoorSizeX, fDoorSizeY, 1.f));
		Lefttrans->Set_State(CTransform::STATE_POSITION, _float3(10.5f, 2.5f, 0.7f));

		CBoxCollider* box = static_cast<CBoxCollider*>(Left_Door->Get_Component(COM_COLLIDER));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3(fDoorSizeX, fDoorSizeY, 1.f));
	}
	// Start Right Door
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Right_Door"), TEXT("Prototype_GameObject_Right_Door"))))
			return E_FAIL;

		CGameObject* Right_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Right_Door"));
		CTransform* Righttrans = (CTransform*)Right_Door->Get_Component(COM_TRANSFORM);

		Righttrans->Scaled(_float3(fDoorSizeX, fDoorSizeY, 1.f));
		Righttrans->Set_State(CTransform::STATE_POSITION, _float3(13.5f, 2.5f, 0.7f));

		CBoxCollider* box = static_cast<CBoxCollider*>(Right_Door->Get_Component(COM_COLLIDER));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3(fDoorSizeX, fDoorSizeY, 1.f));
	}

	// Exit Left Door
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Left_Exit"), TEXT("Prototype_GameObject_Left_Door"))))
			return E_FAIL;

		CGameObject* Left_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Left_Exit"));
		CTransform* Lefttrans = (CTransform*)Left_Door->Get_Component(COM_TRANSFORM);
		static_cast<CDoor_left*>(Left_Door)->Set_TextureIndex(1);
		Lefttrans->Scaled(_float3(fDoorSizeX, fDoorSizeY, 1.f));
		Lefttrans->Set_State(CTransform::STATE_POSITION, _float3(10.5f, 2.5f, (_float)iLineZ - 1.f));

		CBoxCollider* box = static_cast<CBoxCollider*>(Left_Door->Get_Component(COM_COLLIDER));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3(fDoorSizeX, fDoorSizeY, 1.f));
	}

	// Exit Right Door
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Right_Exit"), TEXT("Prototype_GameObject_Right_Door"))))
			return E_FAIL;

		CGameObject* Right_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Right_Exit"));
		CTransform* Righttrans = (CTransform*)Right_Door->Get_Component(COM_TRANSFORM);
		static_cast<CDoor_right*>(Right_Door)->Set_TextureIndex(1);
		Righttrans->Scaled(_float3(fDoorSizeX, fDoorSizeY, 1.f));
		Righttrans->Set_State(CTransform::STATE_POSITION, _float3(13.5f, 2.5f, ((_float)iLineZ) - 1.f));

		CBoxCollider* box = static_cast<CBoxCollider*>(Right_Door->Get_Component(COM_COLLIDER));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3(fDoorSizeX, fDoorSizeY, 1.f));
	}


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_StageOne::Ready_Layer_Second_Entrance()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// 두번째 입구 바닥 충돌체
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Enter_Bot_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Enter_Bot_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		transform->Scaled(_float3((_float)iWaterZ, (_float)iWaterZ, (_float)iWaterZ));
		transform->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90));
		transform->Set_State(CTransform::STATE_POSITION, _float3((iLineX) * 0.5f - 0.5f, 1.f, (_float)iLineZ + 2.f));

		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ, 0.1f, (_float)iWaterZ));
		//	box->Set_AdditionalPos(_float3((iWaterZ * 0.5f), 0.5f, (iWaterZ * 0.5f)));
	}

	// 두번째 입구 오른쪽면 충돌체
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Enter_R_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Enter_R_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		transform->Scaled(_float3((_float)iWaterZ, (_float)iWaterZ, (_float)iWaterZ));
		transform->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90));
		transform->Set_State(CTransform::STATE_POSITION, _float3(_float3((iLineX) * 0.5f + 2.f, (_float)iWaterZ * 0.5f, (_float)iLineZ + 2.f)));

		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		//box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ, (_float)iWaterZ, 0.5f));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3(0.1f, (_float)iWaterZ, (_float)iWaterZ));
	}

	// 두번째 입구 왼쪽면 충돌체
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Enter_L_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Enter_L_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		transform->Scaled(_float3((_float)iWaterZ, (_float)iWaterZ, (_float)iWaterZ));
		transform->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(270));
		transform->Set_State(CTransform::STATE_POSITION, _float3(_float3((iLineX) * 0.5f - 3.f, (_float)iWaterZ * 0.5f, (_float)iLineZ + 2.f)));

		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		//box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ, (_float)iWaterZ, 0.5f));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3(0.1f, (_float)iWaterZ, (_float)iWaterZ));
	}
	// 두번째 입구 천장 충돌체
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Enter_T_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Enter_T_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		transform->Scaled(_float3((_float)iWaterZ, (_float)iWaterZ + 1.f, (_float)iWaterZ));
		transform->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(270));
		transform->Set_State(CTransform::STATE_POSITION, _float3((iLineX) * 0.5f - 0.5f, (_float)iWaterZ - 1.f, (_float)iLineZ + 2.f));

		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ + 1.f, 0.1f, (_float)iWaterZ));
	}

	// Second entrance Right Door
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Left_Entrance"), TEXT("Prototype_GameObject_Left_Door"))))
			return E_FAIL;

		CGameObject* Left_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Left_Entrance"));
		CTransform* Lefttrans = (CTransform*)Left_Door->Get_Component(COM_TRANSFORM);
		Lefttrans->Scaled(_float3(fDoorSizeX, fDoorSizeY, 1.f));
		Lefttrans->Set_State(CTransform::STATE_POSITION, _float3(10.5f, 2.5f, (_float)iLineZ + (_float)iWaterZ));

		CBoxCollider* box = static_cast<CBoxCollider*>(Left_Door->Get_Component(COM_COLLIDER));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3(fDoorSizeX, fDoorSizeY, 1.f));
	}

	// Second entrance Right Door
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Right_Entrance"), TEXT("Prototype_GameObject_Right_Door"))))
			return E_FAIL;

		CGameObject* Right_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Right_Entrance"));
		CTransform* Righttrans = (CTransform*)Right_Door->Get_Component(COM_TRANSFORM);
		Righttrans->Scaled(_float3(fDoorSizeX, fDoorSizeY, 1.f));
		Righttrans->Set_State(CTransform::STATE_POSITION, _float3(13.5f, 2.5f, ((_float)iLineZ) + (_float)iWaterZ));

		CBoxCollider* box = static_cast<CBoxCollider*>(Right_Door->Get_Component(COM_COLLIDER));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3(fDoorSizeX, fDoorSizeY, 1.f));
	}



	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_ChangeLevel"), TEXT("Prototype_GameObject_ChangeLevel"))))
		return E_FAIL;

	m_Change = static_cast<CChangeLevel*>(pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_ChangeLevel")));

	CTransform* ExitTrans = static_cast<CTransform*>(m_Change->Get_Component(COM_TRANSFORM));
	ExitTrans->Scaled(_float3(1.f, 1.f, 10.f));
	ExitTrans->Set_State(CTransform::STATE_POSITION, _float3(81.f, 1.f, 73.f));

	CBoxCollider* ExitBox = static_cast<CBoxCollider*>(m_Change->Get_Component(COM_COLLIDER));
	ExitBox->Set_State(CBoxCollider::COLL_SIZE, _float3(1.f, 1.f, 10.f));

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_StageOne::Ready_Layer_Monster_Map()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// 두번째 맵 바닥 충돌체
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Bot_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Bot_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(2);
		transform->Scaled(_float3((_float)iLineX, (_float)iLineY * 2.f, (_float)iLineZ));
		transform->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90));
		transform->Set_State(CTransform::STATE_POSITION, _float3((_float)iLineX - iLineX * 0.5f, 1.f, (_float)iLineZ + (_float)iWaterZ * 3.f - 0.5f));

		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iLineX, 0.1f, (_float)iLineZ));
		//	box->Set_AdditionalPos(_float3((iWaterZ * 0.5f), 0.5f, (iWaterZ * 0.5f)));
	}

	// 두번째 맵 천장 충돌체
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Top_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Top_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(2);
		transform->Scaled(_float3((_float)iLineX, (_float)iLineY * 2.f, (_float)iLineZ));
		transform->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(270));
		transform->Set_State(CTransform::STATE_POSITION, _float3((_float)iLineX - iLineX * 0.5f, (_float)iLineY * 1.5f, (_float)iLineZ + (_float)iWaterZ * 3.f - 1.f));

		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iLineX, 0.1f, (_float)iLineZ));
		//	box->Set_AdditionalPos(_float3((iWaterZ * 0.5f), 0.5f, (iWaterZ * 0.5f)));
	}

	// 두번째 맵 왼쪽면 충돌체
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_L_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_L_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		transform->Scaled(_float3((_float)iLineX, (_float)iLineY * 1.5f, (_float)iLineZ));
		transform->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(270));
		transform->Set_State(CTransform::STATE_POSITION, _float3(3.f, (_float)iLineY * 0.8f, (_float)iLineY + (_float)iLineZ + 5.f));

		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		box->Set_State(CBoxCollider::COLL_SIZE, _float3(0.1f, (_float)iLineY * 1.5f, (_float)iLineZ));
	}

	// 두번째 맵 오른쪽면 충돌체
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_R_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_R_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		transform->Scaled(_float3((_float)iLineX, (_float)iLineY * 1.5f, (_float)iLineZ));
		transform->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90));
		transform->Set_State(CTransform::STATE_POSITION, _float3(_float3(_float(iLineX), (_float)iLineY * 0.8f, (_float)iLineY + (_float)iLineZ + 5.f)));

		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		box->Set_State(CBoxCollider::COLL_SIZE, _float3(0.1f, (_float)iLineY * 1.5f, (_float)iLineZ));
	}

	// 두번째 맵 후면 충돌체_0
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_B_Collider0"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_B_Collider0"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		transform->Scaled(_float3((_float)iLineX * 0.3f, (_float)iLineY * 1.5f, 1.f));
		transform->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180));
		transform->Set_State(CTransform::STATE_POSITION, _float3(5.f, (_float)iLineY * 0.8f, (_float)iLineZ + 5.f));


		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iLineX * 0.3f, (_float)iLineY * 1.5f, 0.1f));

	}

	// 두번째 맵 후면 충돌체_1
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_B_Collider1"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_B_Collider1"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		transform->Scaled(_float3((_float)iLineX * 0.3f, (_float)iLineY * 1.3f, 1.f));
		transform->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180));
		transform->Set_State(CTransform::STATE_POSITION, _float3((_float)iLineX * 0.5f, (_float)iLineY, (_float)iLineZ + 5.f));


		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iLineX * 1.3f, (_float)iLineY * 0.5f, 0.1f));

	}

	// 두번째 맵 후면 충돌체_2
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_B_Collider2"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_B_Collider2"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		transform->Scaled(_float3((_float)iLineX * 0.4f, (_float)iLineY * 1.5f, 1.f));
		transform->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180));
		transform->Set_State(CTransform::STATE_POSITION, _float3((_float)iLineX * 0.8f, (_float)iLineY * 0.8f, (_float)iLineZ + 5.f));


		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iLineX * 0.3f, (_float)iLineY * 1.5f, 0.1f));
	}

	// 두번째 맵 정면 충돌체_0
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_F_Collider0"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_F_Collider0"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		transform->Scaled(_float3((_float)iLineX * 0.4f, (_float)iLineY * 1.5f, 1.f));
		transform->Set_State(CTransform::STATE_POSITION, _float3(5.f, (_float)iLineY * 0.8f, (_float)iLineZ * 2.f - 1.f));


		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iLineX * 0.3f, (_float)iLineY * 1.5f, 0.1f));

	}

	// 두번째 맵 후면 충돌체_1
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_F_Collider1"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_F_Collider1"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		transform->Scaled(_float3((_float)iLineX * 0.3f, (_float)iLineY * 1.3f, 1.f));
		transform->Set_State(CTransform::STATE_POSITION, _float3((_float)iLineX * 0.5f, (_float)iLineY, (_float)iLineZ * 2.f - 1.f));


		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iLineX * 0.3f, (_float)iLineY * 1.3f, 0.1f));

	}

	// 두번째 맵 후면 충돌체_2
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_F_Collider2"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_F_Collider2"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		transform->Scaled(_float3((_float)iLineX * 0.4f, (_float)iLineY * 1.5f, 1.f));
		transform->Set_State(CTransform::STATE_POSITION, _float3((_float)iLineX * 0.8f, (_float)iLineY * 0.8f, (_float)iLineZ * 2.f - 1.f));


		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iLineX * 0.4f, (_float)iLineY * 1.5f, 0.1f));
	}

	// Second Exit Right Door
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Left_Exit"), TEXT("Prototype_GameObject_Left_Door"))))
			return E_FAIL;

		CGameObject* Left_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Left_Exit"));
		CTransform* Lefttrans = (CTransform*)Left_Door->Get_Component(COM_TRANSFORM);
		Lefttrans->Scaled(_float3(fDoorSizeX, fDoorSizeY, 1.f));
		Lefttrans->Set_State(CTransform::STATE_POSITION, _float3(10.5f, 2.5f, (_float)iLineZ * 2.f - 1.f));
		static_cast<CDoor_left*>(Left_Door)->Set_TextureIndex(1);

		CBoxCollider* box = static_cast<CBoxCollider*>(Left_Door->Get_Component(COM_COLLIDER));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3(fDoorSizeX, fDoorSizeY, 1.f));
	}

	// Second Exit Right Door
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Right_Exit"), TEXT("Prototype_GameObject_Right_Door"))))
			return E_FAIL;

		CGameObject* Right_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Right_Exit"));
		CTransform* Righttrans = (CTransform*)Right_Door->Get_Component(COM_TRANSFORM);
		Righttrans->Scaled(_float3(fDoorSizeX, fDoorSizeY, 1.f));
		Righttrans->Set_State(CTransform::STATE_POSITION, _float3(13.5f, 2.5f, (_float)iLineZ * 2.f - 1.f));
		static_cast<CDoor_left*>(Right_Door)->Set_TextureIndex(1);

		CBoxCollider* box = static_cast<CBoxCollider*>(Right_Door->Get_Component(COM_COLLIDER));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3(fDoorSizeX, fDoorSizeY, 1.f));
	}


	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Button_Block"), TEXT("Prototype_GameObject_Block"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Button_Block"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		transform->Scaled(_float3(5.f, 1.f, 5.f));
		transform->Set_State(CTransform::STATE_POSITION, _float3(5.f, 5.f, 47.f));


		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3(5.f, 1.f, 5.f));
		box->Set_CollStyle(CCollider::COLLSTYLE_ENTER);

	}

	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("CubeMonster"), TEXT("Prototype_GameObject_CubeMonster"))))
			return E_FAIL;

		CGameObject* Switch = pGameInstance->Get_GameObject_End(LEVEL_STAGEONE, TEXT("CubeMonster"));
		CTransform* trans = (CTransform*)Switch->Get_Component(COM_TRANSFORM);
		trans->Set_State(CTransform::STATE_POSITION, _float3(10.f, 2.f, 40.f));
		static_cast<CCubeMonster*>(Switch)->Set_InitPos(_float3(10.f, 2.f, 40.f));
	}

	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("CubeMonster"), TEXT("Prototype_GameObject_CubeMonster"))))
			return E_FAIL;

		CGameObject* Switch = pGameInstance->Get_GameObject_End(LEVEL_STAGEONE, TEXT("CubeMonster"));
		CTransform* trans = (CTransform*)Switch->Get_Component(COM_TRANSFORM);
		trans->Set_State(CTransform::STATE_POSITION, _float3(15.f, 5.f, 40.f));
		static_cast<CCubeMonster*>(Switch)->Set_InitPos(_float3(15.f, 5.f, 40.f));
	}


	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("CubeMonster"), TEXT("Prototype_GameObject_CubeMonster"))))
			return E_FAIL;

		CGameObject* Switch = pGameInstance->Get_GameObject_End(LEVEL_STAGEONE, TEXT("CubeMonster"));
		CTransform* trans = (CTransform*)Switch->Get_Component(COM_TRANSFORM);
		trans->Set_State(CTransform::STATE_POSITION, _float3(15.f, 2.f, 45.f));
		static_cast<CCubeMonster*>(Switch)->Set_InitPos(_float3(15.f, 2.f, 45.f));

	}


	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("CubeMonster"), TEXT("Prototype_GameObject_CubeMonster"))))
			return E_FAIL;

		CGameObject* Switch = pGameInstance->Get_GameObject_End(LEVEL_STAGEONE, TEXT("CubeMonster"));
		CTransform* trans = (CTransform*)Switch->Get_Component(COM_TRANSFORM);
		trans->Set_State(CTransform::STATE_POSITION, _float3(20.f, 2.f, 45.f));
		static_cast<CCubeMonster*>(Switch)->Set_InitPos(_float3(20.f, 2.f, 45.f));

	}



	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("CubeMonster"), TEXT("Prototype_GameObject_CubeMonster"))))
			return E_FAIL;

		CGameObject* Switch = pGameInstance->Get_GameObject_End(LEVEL_STAGEONE, TEXT("CubeMonster"));
		CTransform* trans = (CTransform*)Switch->Get_Component(COM_TRANSFORM);
		trans->Set_State(CTransform::STATE_POSITION, _float3(20.f, 2.f, 45.f));
		static_cast<CCubeMonster*>(Switch)->Set_InitPos(_float3(20.f, 2.f, 45.f));
	}



	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("CubeMonster"), TEXT("Prototype_GameObject_CubeMonster"))))
			return E_FAIL;

		CGameObject* Switch = pGameInstance->Get_GameObject_End(LEVEL_STAGEONE, TEXT("CubeMonster"));
		CTransform* trans = (CTransform*)Switch->Get_Component(COM_TRANSFORM);
		trans->Set_State(CTransform::STATE_POSITION, _float3(10.f, 4.f, 45.f));
		static_cast<CCubeMonster*>(Switch)->Set_InitPos(_float3(10.f, 4.f, 45.f));

	}




	// 몬스터를 다 잡으면 오픈 큐브 나오게 나중에 수정

	// Open Event용 충돌체 cube
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Open_Exit"), TEXT("Prototype_GameObject_Block"))))
			return E_FAIL;

		CGameObject* Switch = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Open_Exit"), 0);
		CTransform* trans = (CTransform*)Switch->Get_Component(COM_TRANSFORM);
		trans->Scaled(_float3(1.f, 1.f, 1.f));
		trans->Set_State(CTransform::STATE_POSITION, _float3(12.f, 1.f, ((_float)iLineZ) - 3.f));

		CBoxCollider* box = static_cast<CBoxCollider*>(Switch->Get_Component(COM_COLLIDER));
	}

	// Open Event용 충돌체 cube
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Exit2_Switch"), TEXT("Prototype_GameObject_Block"))))
			return E_FAIL;

		CGameObject* Switch = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Exit2_Switch"), 0);
		CTransform* trans = (CTransform*)Switch->Get_Component(COM_TRANSFORM);
		static_cast<CTile_Cube*>(Switch)->Set_TextureIndex(2);
		trans->Scaled(_float3(1.f, 0.7f, 1.f));
		trans->Set_State(CTransform::STATE_POSITION, _float3(5.f, 5.5f, 47.f));

		CBoxCollider* box = static_cast<CBoxCollider*>(Switch->Get_Component(COM_COLLIDER));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3(1.f, 0.7f, 1.f));
	}


	// Open Event용 충돌체 cube
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Door_Switch"), TEXT("Prototype_GameObject_Block"))))
			return E_FAIL;

		CGameObject* Switch = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Door_Switch"), 0);
		CTransform* trans = (CTransform*)Switch->Get_Component(COM_TRANSFORM);
		trans->Scaled(_float3(1.f, 1.f, 1.f));
		trans->Set_State(CTransform::STATE_POSITION, _float3(12.f, 1.f, ((_float)iLineZ) + 2.5f));

		CBoxCollider* box = static_cast<CBoxCollider*>(Switch->Get_Component(COM_COLLIDER));
	}


	m_EventCube1 = static_cast<CTile_Cube*>(pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Open_Exit"), 0));
	m_EventCube2 = static_cast<CTile_Cube*>(pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Door_Switch"), 0));
	m_EventCube3 = static_cast<CTile_Cube*>(pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Exit2_Switch"), 0));

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_StageOne::Ready_Layer_Next_Stage()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// 2번방이랑 에너지볼 맵 통로
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Exit_Bot_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Exit_Bot_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		transform->Scaled(_float3((_float)iWaterZ, (_float)iWaterZ + 15.f, (_float)iWaterZ));
		transform->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90));
		transform->Set_State(CTransform::STATE_POSITION, _float3((iLineX) * 0.5f - 0.5f, 1.f, (_float)iLineZ + 34.5f));

		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ, 2.f, (_float)iWaterZ + 15.f));
		box->Set_AdditionalPos(_float3(0.f, -1.f, 0.f));
	}

	// 2번방이랑 에너지볼 맵 통로 오른쪽면 충돌체
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Exit_R_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Exit_R_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		transform->Scaled(_float3((_float)iWaterZ + 15.f, (_float)iWaterZ, (_float)iWaterZ));
		transform->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90));
		//transform->Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(180));
		transform->Set_State(CTransform::STATE_POSITION, _float3(_float3((iLineX) * 0.5f + 2.f, (_float)iWaterZ * 0.5f, (_float)iLineZ + 34.5f)));

		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		//box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ, (_float)iWaterZ, 0.5f));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3(2.f, (_float)iWaterZ, (_float)iWaterZ + 15.f));
		box->Set_AdditionalPos(_float3(1.f, 0.f, 0.f));
	}

	//2번방이랑 에너지볼 맵 통로 왼쪽면 충돌체
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Exit_L_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Exit_L_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		transform->Scaled(_float3((_float)iWaterZ + 15.f, (_float)iWaterZ, (_float)iWaterZ));
		transform->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(-90));
		transform->Set_State(CTransform::STATE_POSITION, _float3(_float3((iLineX) * 0.5f - 3.f, (_float)iWaterZ * 0.5f, (_float)iLineZ + 34.5f)));

		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		//box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ, (_float)iWaterZ, 0.5f));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3(2.f, (_float)iWaterZ, (_float)iWaterZ + 15.f));
		box->Set_AdditionalPos(_float3(-1.f, 0.f, 0.f));
	}
	// 2번방이랑 에너지볼 맵 통로 천장 충돌체
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Exit_T_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Exit_T_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		transform->Scaled(_float3((_float)iWaterZ, (_float)iWaterZ + 15.f, (_float)iWaterZ));
		transform->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(270));
		transform->Set_State(CTransform::STATE_POSITION, _float3((iLineX) * 0.5f - 0.5f, (_float)iWaterZ, (_float)iLineZ + 34.5f));

		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ, 2.f, (_float)iWaterZ + 15.f));
		box->Set_AdditionalPos(_float3(0.f, 1.f, 0.f));
	}



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_StageOne::Ready_Layer_Ball_Map()
{

	_float3 BallMapPos = _float3(12.f, 0.f, 84.5f);
	_float3 GravityMapPos = _float3(55.f, -15.f, 92.f);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Ball_Bottom2"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* BallBottomTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Ball_Bottom2"));
	CTransform* BallBottomTrans = (CTransform*)BallBottomTile->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(BallBottomTile)->Set_TextureIndex(1);
	//실제 보이는 박스
	BallBottomTrans->Scaled(_float3(m_iBoxSize, m_iBoxSize + HalfBoxSize, m_iBoxSize));
	BallBottomTrans->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	BallBottomTrans->Set_State(CTransform::STATE_POSITION, _float3(BallMapPos.x, BallMapPos.y + 1.f, BallMapPos.z));
	CBoxCollider* Ballbox = static_cast<CBoxCollider*>(BallBottomTile->Get_Component(COM_COLLIDER));
	//충돌박스
	Ballbox->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iBoxSize, WallSize, m_iBoxSize + HalfBoxSize));
	Ballbox->Set_AdditionalPos(_float3(0.f, -HalfWallSize, 0.f));

	//천장
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Ball_Bottom3"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* BallBottomTileBottom1 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Ball_Bottom3"));
	CTransform* BallBottomTransBottom1 = (CTransform*)BallBottomTileBottom1->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(BallBottomTileBottom1)->Set_TextureIndex(1);
	//실제 보이는 박스
	BallBottomTransBottom1->Scaled(_float3(m_iBoxSize, m_iBoxSize + HalfBoxSize, m_iBoxSize));
	BallBottomTransBottom1->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(-90.f));
	BallBottomTransBottom1->Set_State(CTransform::STATE_POSITION, _float3(BallMapPos.x, BallMapPos.y + m_iBoxSize, BallMapPos.z));
	CBoxCollider* BallboxBottom1 = static_cast<CBoxCollider*>(BallBottomTileBottom1->Get_Component(COM_COLLIDER));
	//충돌박스
	BallboxBottom1->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iBoxSize, WallSize, m_iBoxSize + HalfBoxSize));
	BallboxBottom1->Set_AdditionalPos(_float3(0.f, HalfWallSize, 0.f));

	//왼쪽
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Ball_Left1"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* BallBottomTile1 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Ball_Left1"));
	CTransform* BallBottomTrans1 = (CTransform*)BallBottomTile1->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(BallBottomTile1)->Set_TextureIndex(1);
	//실제 보이는 박스
	BallBottomTrans1->Scaled(_float3(m_iBoxSize + HalfBoxSize, m_iBoxSize, m_iBoxSize));
	BallBottomTrans1->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(-90.f));
	BallBottomTrans1->Set_State(CTransform::STATE_POSITION, _float3(BallMapPos.x - HalfBoxSize, BallMapPos.y + HalfBoxSize, BallMapPos.z));;
	CBoxCollider* Ballbox1 = static_cast<CBoxCollider*>(BallBottomTile1->Get_Component(COM_COLLIDER));
	//충돌박스
	Ballbox1->Set_State(CBoxCollider::COLL_SIZE, _float3(WallSize, m_iBoxSize, m_iBoxSize + HalfBoxSize));
	Ballbox1->Set_AdditionalPos(_float3(-HalfWallSize, 0.f, 0.f));



	//오른쪽
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Ball_Right"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* BallBottomTile4 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Ball_Right"));
	CTransform* BallBottomTrans4 = (CTransform*)BallBottomTile4->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(BallBottomTile4)->Set_TextureIndex(1);
	//실제 보이는 박스
	BallBottomTrans4->Scaled(_float3(m_iBoxSize + HalfBoxSize, m_iBoxSize, m_iBoxSize));
	BallBottomTrans4->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
	BallBottomTrans4->Set_State(CTransform::STATE_POSITION, _float3(BallMapPos.x + HalfBoxSize, BallMapPos.y + HalfBoxSize, BallMapPos.z + 7.5f));;
	CBoxCollider* Ballbox4 = static_cast<CBoxCollider*>(BallBottomTile4->Get_Component(COM_COLLIDER));
	//충돌박스
	Ballbox4->Set_State(CBoxCollider::COLL_SIZE, _float3(WallSize, m_iBoxSize, m_iBoxSize + HalfBoxSize));
	Ballbox4->Set_AdditionalPos(_float3(HalfWallSize, 0.f, 0.f));



	//정면
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Ball_Front"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* BallBottomTile2 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Ball_Front"));
	CTransform* BallBottomTrans2 = (CTransform*)BallBottomTile2->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(BallBottomTile2)->Set_TextureIndex(1);
	//실제 보이는 박스
	BallBottomTrans2->Scaled(_float3(m_iBoxSize, m_iBoxSize, m_iBoxSize));
	BallBottomTrans2->Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(-90.f));
	BallBottomTrans2->Set_State(CTransform::STATE_POSITION, _float3(BallMapPos.x, BallMapPos.y + HalfBoxSize, BallMapPos.z + HalfBoxSize * 1.5f));
	CBoxCollider* Ballbox2 = static_cast<CBoxCollider*>(BallBottomTile2->Get_Component(COM_COLLIDER));
	//충돌박스
	Ballbox2->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iBoxSize, m_iBoxSize, WallSize));
	Ballbox2->Set_AdditionalPos(_float3(0.f, 0.f, HalfWallSize));



	//뒤에 왼

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Ball_Back2"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* BallBottomTile3 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Ball_Back2"));
	CTransform* BallBottomTrans3 = (CTransform*)BallBottomTile3->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(BallBottomTile3)->Set_TextureIndex(1);
	//실제 보이는 박스
	BallBottomTrans3->Scaled(_float3(m_iBoxSize, m_iBoxSize, m_iBoxSize));
	//BottomTrans3->Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(90.f));
	BallBottomTrans3->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180.f));
	BallBottomTrans3->Set_State(CTransform::STATE_POSITION, _float3(BallMapPos.x - 12.5f, BallMapPos.y + HalfBoxSize, BallMapPos.z - HalfBoxSize * 1.5f));
	CBoxCollider* Ballbox3 = static_cast<CBoxCollider*>(BallBottomTile3->Get_Component(COM_COLLIDER));
	//충돌박스
	Ballbox3->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iBoxSize, m_iBoxSize, WallSize));
	Ballbox3->Set_AdditionalPos(_float3(0.f, 0.f, -HalfWallSize));

	//뒤에 오른

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Ball_Back1"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* BallBottomTile7 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Ball_Back1"));
	CTransform* BallBottomTrans7 = (CTransform*)BallBottomTile7->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(BallBottomTile7)->Set_TextureIndex(1);
	//실제 보이는 박스
	BallBottomTrans7->Scaled(_float3(m_iBoxSize, m_iBoxSize, m_iBoxSize));
	//BottomTrans7->Rotation(_float3(0.f, 0.f, 1.f), D7DXToRadian(90.f));
	BallBottomTrans7->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180.f));
	BallBottomTrans7->Set_State(CTransform::STATE_POSITION, _float3(BallMapPos.x + 12.5f, BallMapPos.y + HalfBoxSize, BallMapPos.z - HalfBoxSize * 1.5f - 0.05f));
	CBoxCollider* Ballbox7 = static_cast<CBoxCollider*>(BallBottomTile7->Get_Component(COM_COLLIDER));
	//충돌박스
	Ballbox7->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iBoxSize, m_iBoxSize, WallSize));
	Ballbox7->Set_AdditionalPos(_float3(0.f, 0.f, -HalfWallSize - 2.f));

	//뒤에 중앙

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Ball_Back3"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* BallBottomTile8 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Ball_Back3"));
	CTransform* BallBottomTrans8 = (CTransform*)BallBottomTile8->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(BallBottomTile8)->Set_TextureIndex(1);
	//실제 보이는 박스
	BallBottomTrans8->Scaled(_float3(m_iBoxSize + 2.f, m_iBoxSize, 12.5f));
	//BottomTrans8->Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(90.f));
	BallBottomTrans8->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180.f));
	BallBottomTrans8->Set_State(CTransform::STATE_POSITION, _float3(BallMapPos.x, BallMapPos.y + HalfBoxSize + 5.f, BallMapPos.z - HalfBoxSize * 1.5f - 0.1f));
	CBoxCollider* Ballbox8 = static_cast<CBoxCollider*>(BallBottomTile8->Get_Component(COM_COLLIDER));
	//충돌박스
	Ballbox7->Set_State(CBoxCollider::COLL_SIZE, _float3(WallSize, m_iBoxSize, 12.5f));
	Ballbox8->Set_AdditionalPos(_float3(0.f, 0.f, -HalfWallSize));


	//그 아래있는 문
	// 통로 출구 왼
	{

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Passage_Left1_Exit"), TEXT("Prototype_GameObject_Left_Door"))))
			return E_FAIL;

		CGameObject* PassageLeft1Exit_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Passage_Left1_Exit"));
		CTransform* PassageLeft1Exittrans = (CTransform*)PassageLeft1Exit_Door->Get_Component(COM_TRANSFORM);

		PassageLeft1Exittrans->Scaled(_float3(fDoorSizeX + 1.f, fDoorSizeY + 1.f, 1.f));
		PassageLeft1Exittrans->Set_State(CTransform::STATE_POSITION, _float3(BallMapPos.x - 2.f, BallMapPos.y + 3.f, BallMapPos.z - HalfBoxSize * 1.5f - 0.1f));
		CBoxCollider* PassageLeft1Exitbox = static_cast<CBoxCollider*>(PassageLeft1Exit_Door->Get_Component(COM_COLLIDER));
		PassageLeft1Exitbox->Set_State(CBoxCollider::COLL_SIZE, _float3(1.f, fDoorSizeY, fDoorSizeX));
	}
	// 통로 출구 오른
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Passage_Right1_Exit"), TEXT("Prototype_GameObject_Right_Door"))))
			return E_FAIL;

		CGameObject* PassageRight1Exit_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Passage_Right1_Exit"));
		CTransform* PassageRight1Exittrans = (CTransform*)PassageRight1Exit_Door->Get_Component(COM_TRANSFORM);

		PassageRight1Exittrans->Scaled(_float3(fDoorSizeX + 1.f, fDoorSizeY + 1.f, 1.f));
		PassageRight1Exittrans->Set_State(CTransform::STATE_POSITION, _float3(BallMapPos.x + 2.f, BallMapPos.y + 3.f, BallMapPos.z - HalfBoxSize * 1.5f - 0.1f));
		CBoxCollider* PassageRight1Exitbox = static_cast<CBoxCollider*>(PassageRight1Exit_Door->Get_Component(COM_COLLIDER));
		PassageRight1Exitbox->Set_State(CBoxCollider::COLL_SIZE, _float3(1.f, fDoorSizeY, fDoorSizeX));
	}

	//중간벽

	//왼쪽
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Middle_Left"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* MiddleTile1 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Middle_Left"));
	CTransform* MiddleTrans1 = (CTransform*)MiddleTile1->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(MiddleTile1)->Set_TextureIndex(1);
	//실제 보이는 박스
	MiddleTrans1->Scaled(_float3((m_iBoxSize + HalfBoxSize) * 0.5f, m_iBoxSize, m_iBoxSize));
	MiddleTrans1->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
	MiddleTrans1->Set_State(CTransform::STATE_POSITION, _float3(BallMapPos.x - HalfBoxSize * 0.25f, BallMapPos.y + HalfBoxSize, BallMapPos.z + HalfBoxSize * 0.75f));;
	CBoxCollider* Middlebox1 = static_cast<CBoxCollider*>(MiddleTile1->Get_Component(COM_COLLIDER));
	//충돌박스
	Middlebox1->Set_State(CBoxCollider::COLL_SIZE, _float3(WallSize, m_iBoxSize, m_iBoxSize - HalfBoxSize + HalfBoxSize * 0.5f));
	Middlebox1->Set_AdditionalPos(_float3(HalfWallSize, 0.f, 0.f));



	//오른
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Middle_Right"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* MiddleTile2 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Middle_Right"));
	CTransform* MiddleTrans2 = (CTransform*)MiddleTile2->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(MiddleTile2)->Set_TextureIndex(1);
	//실제 보이는 박스
	MiddleTrans2->Scaled(_float3((m_iBoxSize + HalfBoxSize) * 0.5f, m_iBoxSize, m_iBoxSize));
	MiddleTrans2->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(-90.f));
	MiddleTrans2->Set_State(CTransform::STATE_POSITION, _float3(BallMapPos.x + HalfBoxSize * 0.25f, BallMapPos.y + HalfBoxSize, BallMapPos.z + HalfBoxSize * 0.75f));;



	//정면
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Middle_Back"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* MiddleTile3 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Middle_Back"));
	CTransform* MiddleTrans3 = (CTransform*)MiddleTile3->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(MiddleTile3)->Set_TextureIndex(1);
	//실제 보이는 박스
	MiddleTrans3->Scaled(_float3(m_iBoxSize, (m_iBoxSize - HalfBoxSize) * 0.5f, m_iBoxSize));
	MiddleTrans3->Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(-90.f));
	MiddleTrans3->Set_State(CTransform::STATE_POSITION, _float3(BallMapPos.x, BallMapPos.y + HalfBoxSize, BallMapPos.z));


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Middle_UnPortal"), TEXT("Prototype_GameObject_UnPortal"))))
		return E_FAIL;

	CGameObject* MiddleunPortal = pGameInstance->Get_GameObject_End(LEVEL_STAGEONE, TEXT("Layer_Middle_UnPortal"));
	CTransform* MiddleunPortalTr = static_cast<CTransform*>(MiddleunPortal->Get_Component(COM_TRANSFORM));

	MiddleunPortalTr->Set_State(CTransform::STATE_POSITION, _float3(BallMapPos.x, BallMapPos.y + HalfBoxSize, BallMapPos.z + 1.1f));
	MiddleunPortalTr->Scaled(_float3(m_iBoxSize, m_iBoxSize, 2.f));
	CBoxCollider* MiddleunPortalBox = static_cast<CBoxCollider*>(MiddleunPortal->Get_Component(COM_COLLIDER));
	MiddleunPortalBox->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iBoxSize, m_iBoxSize, 2.f));


	////볼맵이랑 중력맵 통로



	//통로 바닥
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Passage_Botttm"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* PassageTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Passage_Botttm"));
	CTransform* PassageTrans = (CTransform*)PassageTile->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(PassageTile)->Set_TextureIndex(1);
	//실제 보이는 박스
	PassageTrans->Scaled(_float3(m_iGravityBoxSize - 12.f, m_iGravityBoxSize * 0.25f - 0.1f, m_iGravityBoxSize));
	PassageTrans->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	PassageTrans->Set_State(CTransform::STATE_POSITION, _float3((GravityMapPos.x + BallMapPos.x) / 2 - 2.5f, GravityMapPos.y + m_iGravityBoxSize * 0.5f, GravityMapPos.z - m_iGravityBoxSize * 0.625f));
	CBoxCollider* Passagebox = static_cast<CBoxCollider*>(PassageTile->Get_Component(COM_COLLIDER));
	//충돌박스
	Passagebox->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iGravityBoxSize - 12.f, GravityWallSize, m_iGravityBoxSize * 0.25f - 0.1f));
	Passagebox->Set_AdditionalPos(_float3(0.f, -HalfGravityWallSize, 0.f));


	//통로 천장
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Passage_Up"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* PassageTileUp = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Passage_Up"));
	CTransform* PassageTransUp = (CTransform*)PassageTileUp->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(PassageTileUp)->Set_TextureIndex(1);
	//실제 보이는 박스
	PassageTransUp->Scaled(_float3(m_iGravityBoxSize - 12.f, m_iGravityBoxSize * 0.25f + 1.f, m_iGravityBoxSize));
	PassageTransUp->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(-90.f));
	PassageTransUp->Set_State(CTransform::STATE_POSITION, _float3((GravityMapPos.x + BallMapPos.x) / 2 - 2.5f, GravityMapPos.y + m_iGravityBoxSize, GravityMapPos.z - m_iGravityBoxSize * 0.625f - 0.1f));
	CBoxCollider* PassageboxUp = static_cast<CBoxCollider*>(PassageTileUp->Get_Component(COM_COLLIDER));
	//충돌박스
	PassageboxUp->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iGravityBoxSize - 12.f, GravityWallSize, m_iGravityBoxSize * 0.25f - 0.1f));
	PassageboxUp->Set_AdditionalPos(_float3(0.f, HalfGravityWallSize, 0.f));


	// 통로 뒤에
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Passage_Back"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* PassageBackTile3 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Passage_Back"));
	CTransform* PassageBackTrans3 = (CTransform*)PassageBackTile3->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(PassageBackTile3)->Set_TextureIndex(1);
	//실제 보이는 박스
	PassageBackTrans3->Scaled(_float3(m_iGravityBoxSize - 12.f, m_iGravityBoxSize, m_iGravityBoxSize));
	//BottomTrans3->Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(90.f));
	PassageBackTrans3->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180.f));
	PassageBackTrans3->Set_State(CTransform::STATE_POSITION, _float3((GravityMapPos.x + BallMapPos.x) / 2 - 2.5f, GravityMapPos.y + GravityhalfBoxSize, GravityMapPos.z - GravityhalfBoxSize * 1.5f));
	CBoxCollider* PassageBackbox3 = static_cast<CBoxCollider*>(PassageBackTile3->Get_Component(COM_COLLIDER));
	//충돌박스
	PassageBackbox3->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iGravityBoxSize, m_iGravityBoxSize, GravityWallSize));
	PassageBackbox3->Set_AdditionalPos(_float3(0.f, 0.f, -HalfGravityWallSize));


	// 통로 앞에
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Passage_Front"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* PassageFrontTile3 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Passage_Front"));
	CTransform* PassageFrontTrans3 = (CTransform*)PassageFrontTile3->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(PassageFrontTile3)->Set_TextureIndex(1);
	//실제 보이는 박스
	PassageFrontTrans3->Scaled(_float3(m_iGravityBoxSize, m_iGravityBoxSize - 12.f, m_iGravityBoxSize));
	PassageFrontTrans3->Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(-90.f));
	PassageFrontTrans3->Set_State(CTransform::STATE_POSITION, _float3((GravityMapPos.x + BallMapPos.x) / 2 - 2.5f, GravityMapPos.y + GravityhalfBoxSize, GravityMapPos.z - GravityhalfBoxSize * 1.5f + m_iGravityBoxSize * 0.25f));
	CBoxCollider* PassageFrontbox3 = static_cast<CBoxCollider*>(PassageFrontTile3->Get_Component(COM_COLLIDER));
	//충돌박스
	PassageFrontbox3->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iGravityBoxSize - 12.f, m_iGravityBoxSize, GravityWallSize));
	PassageFrontbox3->Set_AdditionalPos(_float3(0.f, 0.f, HalfGravityWallSize));
	RELEASE_INSTANCE(CGameInstance);



	//통로쪽 머리위
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Middle5_Right"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* Middle5BottomTile4 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Middle5_Right"));
	CTransform* Middle5BottomTrans4 = (CTransform*)Middle5BottomTile4->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(Middle5BottomTile4)->Set_TextureIndex(1);
	//실제 보이는 박스
	Middle5BottomTrans4->Scaled(_float3(m_iGravityBoxSize * 0.25f, m_iGravityBoxSize - 12.f, m_iGravityBoxSize));
	Middle5BottomTrans4->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
	Middle5BottomTrans4->Set_State(CTransform::STATE_POSITION, _float3((GravityMapPos.x + BallMapPos.x) / 2 - 11.5f, GravityMapPos.y + m_iGravityBoxSize * 0.5f + 13.f, GravityMapPos.z - m_iGravityBoxSize * 0.625f));
	CBoxCollider* Middle5box4 = static_cast<CBoxCollider*>(Middle5BottomTile4->Get_Component(COM_COLLIDER));
	//충돌박스
	Middle5box4->Set_State(CBoxCollider::COLL_SIZE, _float3(WallSize, m_iGravityBoxSize - 12.f, m_iGravityBoxSize * 0.25f));
	Middle5box4->Set_AdditionalPos(_float3(HalfWallSize, 0.f, 0.f));


	//통로쪽 머리위
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Middle6_Right"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* Middle6BottomTile4 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Middle6_Right"));
	CTransform* Middle6BottomTrans4 = (CTransform*)Middle6BottomTile4->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(Middle6BottomTile4)->Set_TextureIndex(1);
	//실제 보이는 박스
	Middle6BottomTrans4->Scaled(_float3(m_iGravityBoxSize * 0.25f, m_iGravityBoxSize - 12.f, m_iGravityBoxSize));
	Middle6BottomTrans4->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(-90.f));
	Middle6BottomTrans4->Set_State(CTransform::STATE_POSITION, _float3((GravityMapPos.x + BallMapPos.x) / 2 - 11.5f, GravityMapPos.y + m_iGravityBoxSize * 0.5f + 13.f, GravityMapPos.z - m_iGravityBoxSize * 0.625f));




	//출구쪽 머리위
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Middle7_Right"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* Middle7BottomTile4 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Middle7_Right"));
	CTransform* Middle7BottomTrans4 = (CTransform*)Middle7BottomTile4->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(Middle7BottomTile4)->Set_TextureIndex(1);
	//실제 보이는 박스
	Middle7BottomTrans4->Scaled(_float3(m_iGravityBoxSize * 0.25f, m_iGravityBoxSize - 12.f, m_iGravityBoxSize));
	Middle7BottomTrans4->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
	Middle7BottomTrans4->Set_State(CTransform::STATE_POSITION, _float3((GravityMapPos.x + BallMapPos.x) / 2 - 11.5f + 18.f, GravityMapPos.y + m_iGravityBoxSize * 0.5f + 13.f, GravityMapPos.z - m_iGravityBoxSize * 0.625f));
	CBoxCollider* Middle7box4 = static_cast<CBoxCollider*>(Middle7BottomTile4->Get_Component(COM_COLLIDER));
	//충돌박스
	Middle7box4->Set_State(CBoxCollider::COLL_SIZE, _float3(WallSize, m_iGravityBoxSize - 12.f, m_iGravityBoxSize * 0.25f));
	Middle7box4->Set_AdditionalPos(_float3(-HalfWallSize, 0.f, 0.f));



	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Middle8_Right"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* Middle8BottomTile4 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Middle8_Right"));
	CTransform* Middle8BottomTrans4 = (CTransform*)Middle8BottomTile4->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(Middle8BottomTile4)->Set_TextureIndex(1);
	//실제 보이는 박스
	Middle8BottomTrans4->Scaled(_float3(m_iGravityBoxSize * 0.25f, m_iGravityBoxSize - 12.f, m_iGravityBoxSize));
	Middle8BottomTrans4->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(-90.f));
	Middle8BottomTrans4->Set_State(CTransform::STATE_POSITION, _float3((GravityMapPos.x + BallMapPos.x) / 2 - 11.5f + 18.f, GravityMapPos.y + m_iGravityBoxSize * 0.5f + 13.f, GravityMapPos.z - m_iGravityBoxSize * 0.625f));
	// 통로 입구 왼
	{

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Passage_Left_Door"), TEXT("Prototype_GameObject_Left_Door"))))
			return E_FAIL;

		CGameObject* PassageLeft_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Passage_Left_Door"));
		CTransform* PassageLefttrans = (CTransform*)PassageLeft_Door->Get_Component(COM_TRANSFORM);

		PassageLefttrans->Scaled(_float3(fDoorSizeX + 1.f, fDoorSizeY + 1.f, 1.f));
		PassageLefttrans->Set_State(CTransform::STATE_POSITION, _float3(22.f, 2.f, 75.f));
		PassageLefttrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
		CBoxCollider* PassageLeftbox = static_cast<CBoxCollider*>(PassageLeft_Door->Get_Component(COM_COLLIDER));
		PassageLeftbox->Set_State(CBoxCollider::COLL_SIZE, _float3(1.f, fDoorSizeY, fDoorSizeX));
	}
	// 통로 입구 오른
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Passage_Right_Door"), TEXT("Prototype_GameObject_Right_Door"))))
			return E_FAIL;

		CGameObject* PassageRight_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Passage_Right_Door"));
		CTransform* PassageRighttrans = (CTransform*)PassageRight_Door->Get_Component(COM_TRANSFORM);

		PassageRighttrans->Scaled(_float3(fDoorSizeX + 1.f, fDoorSizeY + 1.f, 1.f));
		PassageRighttrans->Set_State(CTransform::STATE_POSITION, _float3(22.f, 2.f, 71.f));
		PassageRighttrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
		CBoxCollider* PassageRightbox = static_cast<CBoxCollider*>(PassageRight_Door->Get_Component(COM_COLLIDER));
		PassageRightbox->Set_State(CBoxCollider::COLL_SIZE, _float3(1.f, fDoorSizeY, fDoorSizeX));
	}


	// 통로 출구 왼
	{

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Passage_Left_Exit"), TEXT("Prototype_GameObject_Left_Door"))))
			return E_FAIL;

		CGameObject* PassageLeftExit_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Passage_Left_Exit"));
		CTransform* PassageLeftExittrans = (CTransform*)PassageLeftExit_Door->Get_Component(COM_TRANSFORM);

		PassageLeftExittrans->Scaled(_float3(fDoorSizeX + 1.f, fDoorSizeY + 1.f, 1.f));
		PassageLeftExittrans->Set_State(CTransform::STATE_POSITION, _float3(40.f, 2.f, 75.f));
		PassageLeftExittrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
		CBoxCollider* PassageLeftExitbox = static_cast<CBoxCollider*>(PassageLeftExit_Door->Get_Component(COM_COLLIDER));
		PassageLeftExitbox->Set_State(CBoxCollider::COLL_SIZE, _float3(1.f, fDoorSizeY, fDoorSizeX));
	}
	// 통로 출구 오른
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Passage_Right_Exit"), TEXT("Prototype_GameObject_Right_Door"))))
			return E_FAIL;

		CGameObject* PassageRightExit_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Passage_Right_Exit"));
		CTransform* PassageRightExittrans = (CTransform*)PassageRightExit_Door->Get_Component(COM_TRANSFORM);

		PassageRightExittrans->Scaled(_float3(fDoorSizeX + 1.f, fDoorSizeY + 1.f, 1.f));
		PassageRightExittrans->Set_State(CTransform::STATE_POSITION, _float3(40.f, 2.f, 71.f));
		PassageRightExittrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
		CBoxCollider* PassageRightExitbox = static_cast<CBoxCollider*>(PassageRightExit_Door->Get_Component(COM_COLLIDER));
		PassageRightExitbox->Set_State(CBoxCollider::COLL_SIZE, _float3(1.f, fDoorSizeY, fDoorSizeX));
	}
	return S_OK;
}

HRESULT CLevel_StageOne::Ready_Layer_Gravity_Map()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	_float3 BallMapPos = _float3(12.f, 0.f, 84.5f);
	_float3 GravityMapPos = _float3(55.f, -15.f, 92.f);


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Bottom2"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* GravityBottomTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Bottom2"));
	CTransform* GravityBottomTrans = (CTransform*)GravityBottomTile->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(GravityBottomTile)->Set_TextureIndex(1);
	//실제 보이는 박스
	GravityBottomTrans->Scaled(_float3(m_iGravityBoxSize, m_iGravityBoxSize + GravityhalfBoxSize, m_iGravityBoxSize));
	GravityBottomTrans->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	GravityBottomTrans->Set_State(CTransform::STATE_POSITION, _float3(GravityMapPos.x, GravityMapPos.y, GravityMapPos.z));
	CBoxCollider* Gravitybox = static_cast<CBoxCollider*>(GravityBottomTile->Get_Component(COM_COLLIDER));
	//충돌박스
	Gravitybox->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iGravityBoxSize, GravityWallSize, m_iGravityBoxSize + GravityhalfBoxSize));
	Gravitybox->Set_AdditionalPos(_float3(0.f, -HalfGravityWallSize, 0.f));

	//천장
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Bottom3"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* GravityBottomTileBottom1 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Bottom3"));
	CTransform* GravityBottomTransBottom1 = (CTransform*)GravityBottomTileBottom1->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(GravityBottomTileBottom1)->Set_TextureIndex(1);
	//실제 보이는 박스
	GravityBottomTransBottom1->Scaled(_float3(m_iGravityBoxSize, m_iGravityBoxSize + GravityhalfBoxSize, m_iGravityBoxSize));
	GravityBottomTransBottom1->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(-90.f));
	GravityBottomTransBottom1->Set_State(CTransform::STATE_POSITION, _float3(GravityMapPos.x, GravityMapPos.y + m_iGravityBoxSize, GravityMapPos.z));
	CBoxCollider* GravityboxBottom1 = static_cast<CBoxCollider*>(GravityBottomTileBottom1->Get_Component(COM_COLLIDER));
	//충돌박스
	GravityboxBottom1->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iGravityBoxSize, GravityWallSize, m_iGravityBoxSize + GravityhalfBoxSize));
	GravityboxBottom1->Set_AdditionalPos(_float3(0.f, HalfGravityWallSize, 0.f));

	//왼쪽
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Left1"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* GravityBottomTile1 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Left1"));
	CTransform* GravityBottomTrans1 = (CTransform*)GravityBottomTile1->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(GravityBottomTile1)->Set_TextureIndex(1);
	//실제 보이는 박스
	GravityBottomTrans1->Scaled(_float3(m_iGravityBoxSize + GravityhalfBoxSize, m_iGravityBoxSize, m_iGravityBoxSize));
	GravityBottomTrans1->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(-90.f));
	GravityBottomTrans1->Set_State(CTransform::STATE_POSITION, _float3(GravityMapPos.x - GravityhalfBoxSize, GravityMapPos.y + GravityhalfBoxSize, GravityMapPos.z + 7.5f));;
	CBoxCollider* Gravitybox1 = static_cast<CBoxCollider*>(GravityBottomTile1->Get_Component(COM_COLLIDER));
	//충돌박스
	Gravitybox1->Set_State(CBoxCollider::COLL_SIZE, _float3(GravityWallSize, m_iGravityBoxSize, m_iGravityBoxSize + GravityhalfBoxSize));
	Gravitybox1->Set_AdditionalPos(_float3(-HalfGravityWallSize, 0.f, 0.f));



	//오른쪽
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Right"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* GravityBottomTile4 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Right"));
	CTransform* GravityBottomTrans4 = (CTransform*)GravityBottomTile4->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(GravityBottomTile4)->Set_TextureIndex(1);
	//실제 보이는 박스
	GravityBottomTrans4->Scaled(_float3(m_iGravityBoxSize + GravityhalfBoxSize, m_iGravityBoxSize, m_iGravityBoxSize));
	GravityBottomTrans4->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
	GravityBottomTrans4->Set_State(CTransform::STATE_POSITION, _float3(GravityMapPos.x + GravityhalfBoxSize, GravityMapPos.y + GravityhalfBoxSize, GravityMapPos.z + 7.5f));;
	CBoxCollider* Gravitybox4 = static_cast<CBoxCollider*>(GravityBottomTile4->Get_Component(COM_COLLIDER));
	//충돌박스
	Gravitybox4->Set_State(CBoxCollider::COLL_SIZE, _float3(GravityWallSize, m_iGravityBoxSize, m_iGravityBoxSize + GravityhalfBoxSize));
	Gravitybox4->Set_AdditionalPos(_float3(HalfGravityWallSize, 0.f, 0.f));



	//정면
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Front"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* GravityBottomTile2 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Front"));
	CTransform* GravityBottomTrans2 = (CTransform*)GravityBottomTile2->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(GravityBottomTile2)->Set_TextureIndex(1);
	//실제 보이는 박스
	GravityBottomTrans2->Scaled(_float3(m_iGravityBoxSize, m_iGravityBoxSize, m_iGravityBoxSize));
	GravityBottomTrans2->Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(-90.f));
	GravityBottomTrans2->Set_State(CTransform::STATE_POSITION, _float3(GravityMapPos.x, GravityMapPos.y + GravityhalfBoxSize, GravityMapPos.z + GravityhalfBoxSize * 1.5f));
	CBoxCollider* Gravitybox2 = static_cast<CBoxCollider*>(GravityBottomTile2->Get_Component(COM_COLLIDER));
	//충돌박스
	Gravitybox2->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iGravityBoxSize, m_iGravityBoxSize, GravityWallSize * 2));
	Gravitybox2->Set_AdditionalPos(_float3(0.f, 0.f, GravityWallSize));// HalfGravityWallSize));



	//뒤에
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Back"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* GravityBottomTile3 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Back"));
	CTransform* GravityBottomTrans3 = (CTransform*)GravityBottomTile3->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(GravityBottomTile3)->Set_TextureIndex(1);
	//실제 보이는 박스
	GravityBottomTrans3->Scaled(_float3(m_iGravityBoxSize, m_iGravityBoxSize, m_iGravityBoxSize));
	//BottomTrans3->Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(90.f));
	GravityBottomTrans3->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180.f));
	GravityBottomTrans3->Set_State(CTransform::STATE_POSITION, _float3(GravityMapPos.x, GravityMapPos.y + GravityhalfBoxSize, GravityMapPos.z - GravityhalfBoxSize * 1.5f));
	CBoxCollider* Gravitybox3 = static_cast<CBoxCollider*>(GravityBottomTile3->Get_Component(COM_COLLIDER));
	//충돌박스
	Gravitybox3->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iGravityBoxSize, m_iGravityBoxSize, GravityWallSize));
	Gravitybox3->Set_AdditionalPos(_float3(0.f, 0.f, -HalfGravityWallSize));



	//물
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_UnPortal"), TEXT("Prototype_GameObject_UnPortal"))))
		return E_FAIL;

	CGameObject* GravityunPortal = pGameInstance->Get_GameObject_End(LEVEL_STAGEONE, TEXT("Layer_Gravity_UnPortal"));
	CTransform* GravityunPortalTr = static_cast<CTransform*>(GravityunPortal->Get_Component(COM_TRANSFORM));

	GravityunPortalTr->Set_State(CTransform::STATE_POSITION, _float3(GravityMapPos.x, GravityMapPos.y - GravityMapPos.y, GravityMapPos.z));
	GravityunPortalTr->Scaled(_float3(m_iGravityBoxSize, m_iGravityBoxSize, 2.f));
	CBoxCollider* GravityunPortalBox = static_cast<CBoxCollider*>(GravityunPortal->Get_Component(COM_COLLIDER));
	GravityunPortalBox->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iGravityBoxSize, m_iGravityBoxSize, 2.f));


	//중간 바닥
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Player_Bottom"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* GravityPlayerTileBottom = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Player_Bottom"));
	CTransform* GravityPlayerTransBottom = (CTransform*)GravityPlayerTileBottom->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(GravityPlayerTileBottom)->Set_TextureIndex(1);
	//실제 보이는 박스
	GravityPlayerTransBottom->Scaled(_float3(m_iGravityBoxSize, m_iGravityBoxSize * 0.25f, m_iGravityBoxSize));
	GravityPlayerTransBottom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	GravityPlayerTransBottom->Set_State(CTransform::STATE_POSITION, _float3(GravityMapPos.x, GravityMapPos.y + m_iGravityBoxSize * 0.5f, GravityMapPos.z - m_iGravityBoxSize * 0.625f));
	CBoxCollider* GravityPlayerboxBottom = static_cast<CBoxCollider*>(GravityPlayerTileBottom->Get_Component(COM_COLLIDER));
	//충돌박스
	GravityPlayerboxBottom->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iGravityBoxSize, GravityWallSize, m_iGravityBoxSize * 0.25f));
	GravityPlayerboxBottom->Set_AdditionalPos(_float3(0.f, -HalfGravityWallSize, 0.f));


	//중간 바닥 벽
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Player_Wall"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* GravityPlayerTileWall = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Player_Wall"));
	CTransform* GravityBottomWallTrans = (CTransform*)GravityPlayerTileWall->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(GravityPlayerTileWall)->Set_TextureIndex(1);
	//실제 보이는 박스
	GravityBottomWallTrans->Scaled(_float3(m_iGravityBoxSize, m_iGravityBoxSize * 0.5f, m_iGravityBoxSize));
	GravityBottomWallTrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180.f));
	GravityBottomWallTrans->Set_State(CTransform::STATE_POSITION, _float3(GravityMapPos.x, GravityMapPos.y + m_iGravityBoxSize * 0.25f, GravityMapPos.z - GravityhalfBoxSize));
	CBoxCollider* GravityWallbox = static_cast<CBoxCollider*>(GravityPlayerTileWall->Get_Component(COM_COLLIDER));
	//충돌박스
	GravityWallbox->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iGravityBoxSize * 1.5f, m_iGravityBoxSize * 0.5f, GravityWallSize));
	GravityWallbox->Set_AdditionalPos(_float3(0.f, 0.f, -HalfGravityWallSize));





	//중간 바닥 반대
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Player_Bottom1"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* GravityPlayerTileBottom1 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Player_Bottom1"));
	CTransform* GravityPlayerTransBottom1 = (CTransform*)GravityPlayerTileBottom1->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(GravityPlayerTileBottom1)->Set_TextureIndex(1);
	//실제 보이는 박스
	GravityPlayerTransBottom1->Scaled(_float3(m_iGravityBoxSize, m_iGravityBoxSize * 0.25f, m_iGravityBoxSize));
	GravityPlayerTransBottom1->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	GravityPlayerTransBottom1->Set_State(CTransform::STATE_POSITION, _float3(GravityMapPos.x, GravityMapPos.y + m_iGravityBoxSize * 0.5f, GravityMapPos.z + m_iGravityBoxSize * 0.625f));
	CBoxCollider* GravityPlayerboxBottom1 = static_cast<CBoxCollider*>(GravityPlayerTileBottom1->Get_Component(COM_COLLIDER));
	//충돌박스
	GravityPlayerboxBottom1->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iGravityBoxSize, GravityWallSize, m_iGravityBoxSize * 0.25f));
	GravityPlayerboxBottom1->Set_AdditionalPos(_float3(0.f, -HalfGravityWallSize, 0.f));

	//중간 바닥 반대 벽
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Player_Wall1"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* GravityPlayerTileWall1 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Player_Wall1"));
	CTransform* GravityBottomWall1Trans = (CTransform*)GravityPlayerTileWall1->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(GravityPlayerTileWall1)->Set_TextureIndex(1);
	//실제 보이는 박스
	GravityBottomWall1Trans->Scaled(_float3(m_iGravityBoxSize, m_iGravityBoxSize * 0.5f, m_iGravityBoxSize));
	//GravityBottomWall1Trans->Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(-90.f));
	GravityBottomWall1Trans->Set_State(CTransform::STATE_POSITION, _float3(GravityMapPos.x, GravityMapPos.y + m_iGravityBoxSize * 0.25f, GravityMapPos.z + GravityhalfBoxSize));
	CBoxCollider* GravityWall1box = static_cast<CBoxCollider*>(GravityPlayerTileWall1->Get_Component(COM_COLLIDER));
	//충돌박스
	GravityWall1box->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iGravityBoxSize * 1.5f, m_iGravityBoxSize * 0.5f, GravityWallSize));
	GravityWall1box->Set_AdditionalPos(_float3(0.f, 0.f, HalfGravityWallSize));

	//물
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Water"), TEXT("Prototype_GameObject_Water"))))
			return E_FAIL;

		CGameObject* GravityWater = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Water"), 0);
		CTransform* GravityWaterTrans = (CTransform*)GravityWater->Get_Component(COM_TRANSFORM);
		GravityWaterTrans->Scaled(_float3(m_iGravityBoxSize - 0.1f, 3.f - 0.1f, m_iGravityBoxSize - 0.1f));
		GravityWaterTrans->Set_State(CTransform::STATE_POSITION, _float3(GravityMapPos.x, GravityMapPos.y, GravityMapPos.z));

		CBoxCollider* box = static_cast<CBoxCollider*>(GravityWater->Get_Component(COM_COLLIDER));

		box->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iGravityBoxSize, 2.f, m_iGravityBoxSize));
	}



	//물 위 바닥
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Water_Bottom"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* GravityWaterBottmTileBottom = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Water_Bottom"));
	CTransform* GravityWaterBottmTransBottom = (CTransform*)GravityWaterBottmTileBottom->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(GravityWaterBottmTileBottom)->Set_TextureIndex(1);
	//실제 보이는 박스
	GravityWaterBottmTransBottom->Scaled(_float3(m_iGravityBoxSize * 0.25f, m_iGravityBoxSize * 0.25f, m_iGravityBoxSize * 0.25f));
	GravityWaterBottmTransBottom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	GravityWaterBottmTransBottom->Set_State(CTransform::STATE_POSITION, _float3(GravityMapPos.x, GravityMapPos.y + 2.f, GravityMapPos.z - m_iGravityBoxSize * 0.5f));
	CBoxCollider* GravityWaterBottmboxBottom = static_cast<CBoxCollider*>(GravityWaterBottmTileBottom->Get_Component(COM_COLLIDER));
	//충돌박스
	GravityWaterBottmboxBottom->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iGravityBoxSize * 0.25f, 0.5f, m_iGravityBoxSize * 0.25f));
	GravityWaterBottmboxBottom->Set_AdditionalPos(_float3(0.f, 0.f, 0.f));

	//물 위 바닥 반대
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Water_Bottom1"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* GravityWaterBottm1TileBottom = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Gravity_Water_Bottom1"));
	CTransform* GravityWaterBottm1TransBottom = (CTransform*)GravityWaterBottm1TileBottom->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(GravityWaterBottm1TileBottom)->Set_TextureIndex(1);
	//실제 보이는 박스
	GravityWaterBottm1TransBottom->Scaled(_float3(m_iGravityBoxSize * 0.25f, m_iGravityBoxSize * 0.25f, m_iGravityBoxSize * 0.25f));
	GravityWaterBottm1TransBottom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	GravityWaterBottm1TransBottom->Set_State(CTransform::STATE_POSITION, _float3(GravityMapPos.x, GravityMapPos.y + 2.f, GravityMapPos.z + m_iGravityBoxSize * 0.5f));
	CBoxCollider* GravityWaterBottm1boxBottom = static_cast<CBoxCollider*>(GravityWaterBottm1TileBottom->Get_Component(COM_COLLIDER));
	//충돌박스
	GravityWaterBottm1boxBottom->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iGravityBoxSize * 0.25f, 0.5f, m_iGravityBoxSize * 0.25f));
	GravityWaterBottm1boxBottom->Set_AdditionalPos(_float3(0.f, 0.f, 0.f));




	////중력맵 출구
	//
	//
	//통로 바닥
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_PassageGravity_Botttm"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* PassageGravityTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_PassageGravity_Botttm"));
	CTransform* PassageGravityTrans = (CTransform*)PassageGravityTile->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(PassageGravityTile)->Set_TextureIndex(1);
	//실제 보이는 박스
	PassageGravityTrans->Scaled(_float3(m_iGravityBoxSize - 12.f, m_iGravityBoxSize * 0.25f - 0.1f, m_iGravityBoxSize));
	PassageGravityTrans->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	PassageGravityTrans->Set_State(CTransform::STATE_POSITION, _float3((GravityMapPos.x + BallMapPos.x) / 2 - 2.5f + 18.f + m_iGravityBoxSize, GravityMapPos.y + m_iGravityBoxSize * 0.5f, GravityMapPos.z - m_iGravityBoxSize * 0.625f));
	CBoxCollider* PassageGravitybox = static_cast<CBoxCollider*>(PassageGravityTile->Get_Component(COM_COLLIDER));
	//충돌박스
	PassageGravitybox->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iGravityBoxSize - 12.f, GravityWallSize, m_iGravityBoxSize * 0.25f - 0.1f));
	PassageGravitybox->Set_AdditionalPos(_float3(0.f, -HalfGravityWallSize, 0.f));


	//통로 천장
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_PassageGravity_Up"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* PassageGravityTileUp = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_PassageGravity_Up"));
	CTransform* PassageGravityTransUp = (CTransform*)PassageGravityTileUp->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(PassageGravityTileUp)->Set_TextureIndex(1);
	//실제 보이는 박스
	PassageGravityTransUp->Scaled(_float3(m_iGravityBoxSize - 12.f, m_iGravityBoxSize * 0.25f + 1.f, m_iGravityBoxSize));
	PassageGravityTransUp->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(-90.f));
	PassageGravityTransUp->Set_State(CTransform::STATE_POSITION, _float3((GravityMapPos.x + BallMapPos.x) / 2 - 2.5f + 18.f + m_iGravityBoxSize, GravityMapPos.y + m_iGravityBoxSize, GravityMapPos.z - m_iGravityBoxSize * 0.625f - 0.1f));
	CBoxCollider* PassageGravityboxUp = static_cast<CBoxCollider*>(PassageGravityTileUp->Get_Component(COM_COLLIDER));
	//충돌박스
	PassageGravityboxUp->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iGravityBoxSize - 12.f, GravityWallSize, m_iGravityBoxSize * 0.25f - 0.1f));
	PassageGravityboxUp->Set_AdditionalPos(_float3(0.f, HalfGravityWallSize, 0.f));


	// 통로 뒤에
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_PassageGravity_Back"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* PassageGravityBackTile3 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_PassageGravity_Back"));
	CTransform* PassageGravityBackTrans3 = (CTransform*)PassageGravityBackTile3->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(PassageGravityBackTile3)->Set_TextureIndex(1);
	//실제 보이는 박스
	PassageGravityBackTrans3->Scaled(_float3(m_iGravityBoxSize - 12.f, m_iGravityBoxSize, m_iGravityBoxSize));
	//BottomTrans3->Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(90.f));
	PassageGravityBackTrans3->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180.f));
	PassageGravityBackTrans3->Set_State(CTransform::STATE_POSITION, _float3((GravityMapPos.x + BallMapPos.x) / 2 - 2.5f + 18.f + m_iGravityBoxSize, GravityMapPos.y + GravityhalfBoxSize, GravityMapPos.z - GravityhalfBoxSize * 1.5f));
	CBoxCollider* PassageGravityBackbox3 = static_cast<CBoxCollider*>(PassageGravityBackTile3->Get_Component(COM_COLLIDER));
	//충돌박스
	PassageGravityBackbox3->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iGravityBoxSize, m_iGravityBoxSize, GravityWallSize));
	PassageGravityBackbox3->Set_AdditionalPos(_float3(0.f, 0.f, -HalfGravityWallSize));


	// 통로 앞에
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_PassageGravity_Front"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* PassageGravityFrontTile3 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_PassageGravity_Front"));
	CTransform* PassageGravityFrontTrans3 = (CTransform*)PassageGravityFrontTile3->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(PassageGravityFrontTile3)->Set_TextureIndex(1);
	//실제 보이는 박스
	PassageGravityFrontTrans3->Scaled(_float3(m_iGravityBoxSize, m_iGravityBoxSize - 12.f, m_iGravityBoxSize));
	PassageGravityFrontTrans3->Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(-90.f));
	PassageGravityFrontTrans3->Set_State(CTransform::STATE_POSITION, _float3((GravityMapPos.x + BallMapPos.x) / 2 - 2.5f + 18.f + m_iGravityBoxSize, GravityMapPos.y + GravityhalfBoxSize, GravityMapPos.z - GravityhalfBoxSize * 1.5f + m_iGravityBoxSize * 0.25f));
	CBoxCollider* PassageGravityFrontbox3 = static_cast<CBoxCollider*>(PassageGravityFrontTile3->Get_Component(COM_COLLIDER));
	//충돌박스
	PassageGravityFrontbox3->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iGravityBoxSize - 12.f, m_iGravityBoxSize, GravityWallSize));
	PassageGravityFrontbox3->Set_AdditionalPos(_float3(0.f, 0.f, HalfGravityWallSize));




	//통로쪽 머리위
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_MiddleGravity5_Right"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* MiddleGravity5BottomTile4 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_MiddleGravity5_Right"));
	CTransform* MiddleGravity5BottomTrans4 = (CTransform*)MiddleGravity5BottomTile4->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(MiddleGravity5BottomTile4)->Set_TextureIndex(1);
	//실제 보이는 박스
	MiddleGravity5BottomTrans4->Scaled(_float3(m_iGravityBoxSize * 0.25f, m_iGravityBoxSize - 12.f, m_iGravityBoxSize));
	MiddleGravity5BottomTrans4->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
	MiddleGravity5BottomTrans4->Set_State(CTransform::STATE_POSITION, _float3((GravityMapPos.x + BallMapPos.x) / 2 - 11.5f + m_iGravityBoxSize + 18.f, GravityMapPos.y + m_iGravityBoxSize * 0.5f + 13.f, GravityMapPos.z - m_iGravityBoxSize * 0.625f));
	CBoxCollider* MiddleGravity5box4 = static_cast<CBoxCollider*>(MiddleGravity5BottomTile4->Get_Component(COM_COLLIDER));
	//충돌박스
	MiddleGravity5box4->Set_State(CBoxCollider::COLL_SIZE, _float3(WallSize, m_iGravityBoxSize - 12.f, m_iGravityBoxSize * 0.25f));
	MiddleGravity5box4->Set_AdditionalPos(_float3(HalfWallSize, 0.f, 0.f));


	//통로쪽 머리위
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_MiddleGravity6_Right"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* MiddleGravity6BottomTile4 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_MiddleGravity6_Right"));
	CTransform* MiddleGravity6BottomTrans4 = (CTransform*)MiddleGravity6BottomTile4->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(MiddleGravity6BottomTile4)->Set_TextureIndex(1);
	//실제 보이는 박스
	MiddleGravity6BottomTrans4->Scaled(_float3(m_iGravityBoxSize * 0.25f, m_iGravityBoxSize - 12.f, m_iGravityBoxSize));
	MiddleGravity6BottomTrans4->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(-90.f));
	MiddleGravity6BottomTrans4->Set_State(CTransform::STATE_POSITION, _float3((GravityMapPos.x + BallMapPos.x) / 2 - 11.5f + m_iGravityBoxSize + 18.f, GravityMapPos.y + m_iGravityBoxSize * 0.5f + 13.f, GravityMapPos.z - m_iGravityBoxSize * 0.625f));


	// 통로 입구 왼
	{

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_passageGravity_Left_Door"), TEXT("Prototype_GameObject_Left_Door"))))
			return E_FAIL;

		CGameObject* passageGravityLeft_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_passageGravity_Left_Door"));
		CTransform* passageGravityLefttrans = (CTransform*)passageGravityLeft_Door->Get_Component(COM_TRANSFORM);

		passageGravityLefttrans->Scaled(_float3(fDoorSizeX + 1.f, fDoorSizeY + 1.f, 1.f));
		passageGravityLefttrans->Set_State(CTransform::STATE_POSITION, _float3(40.f + m_iGravityBoxSize, 2.f, 75.f));
		passageGravityLefttrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
		CBoxCollider* passageGravityLeftbox = static_cast<CBoxCollider*>(passageGravityLeft_Door->Get_Component(COM_COLLIDER));
		passageGravityLeftbox->Set_State(CBoxCollider::COLL_SIZE, _float3(1.f, fDoorSizeY, fDoorSizeX));
	}
	// 통로 입구 오른
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_passageGravity_Right_Door"), TEXT("Prototype_GameObject_Right_Door"))))
			return E_FAIL;

		CGameObject* passageGravityRight_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_passageGravity_Right_Door"));
		CTransform* passageGravityRighttrans = (CTransform*)passageGravityRight_Door->Get_Component(COM_TRANSFORM);

		passageGravityRighttrans->Scaled(_float3(fDoorSizeX + 1.f, fDoorSizeY + 1.f, 1.f));
		passageGravityRighttrans->Set_State(CTransform::STATE_POSITION, _float3(40.f + m_iGravityBoxSize, 2.f, 71.f));
		passageGravityRighttrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
		CBoxCollider* passageGravityRightbox = static_cast<CBoxCollider*>(passageGravityRight_Door->Get_Component(COM_COLLIDER));
		passageGravityRightbox->Set_State(CBoxCollider::COLL_SIZE, _float3(1.f, fDoorSizeY, fDoorSizeX));
	}
	//출구(맵이동)
	// Start Left Door
	{

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_LeftGravity_Door"), TEXT("Prototype_GameObject_Left_Door"))))
			return E_FAIL;

		CGameObject* LeftGravity_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_LeftGravity_Door"));
		CTransform* LeftGravitytrans = (CTransform*)LeftGravity_Door->Get_Component(COM_TRANSFORM);

		LeftGravitytrans->Scaled(_float3(20.f, 20.f, 1.f));
		LeftGravitytrans->Set_State(CTransform::STATE_POSITION, _float3(83.f, 10.f, 83.f));
		LeftGravitytrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
		CBoxCollider* LeftGravitybox = static_cast<CBoxCollider*>(LeftGravity_Door->Get_Component(COM_COLLIDER));
		LeftGravitybox->Set_State(CBoxCollider::COLL_SIZE, _float3(20.f, 20.f, 1.f));
	}
	// Start Right Door
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_RightGravity_Door"), TEXT("Prototype_GameObject_Right_Door"))))
			return E_FAIL;

		CGameObject* RightGravity_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_RightGravity_Door"));
		CTransform* RightGravitytrans = (CTransform*)RightGravity_Door->Get_Component(COM_TRANSFORM);

		RightGravitytrans->Scaled(_float3(20.f, 20.f, 1.f));
		RightGravitytrans->Set_State(CTransform::STATE_POSITION, _float3(83.f, 10.f, 63.f));
		RightGravitytrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
		CBoxCollider* RightGravitybox = static_cast<CBoxCollider*>(RightGravity_Door->Get_Component(COM_COLLIDER));
		RightGravitybox->Set_State(CBoxCollider::COLL_SIZE, _float3(20.f, 20.f, 1.f));
	}





	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_StageOne::Open_Exit()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_Open_1 = m_EventCube1->Open_Event(LEVEL_STAGEONE, TEXT("Layer_Left_Exit"), TEXT("Layer_Right_Exit"));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_StageOne::Close_Exit_Open_Door2()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_Close_1 = m_EventCube2->Close_Event(LEVEL_STAGEONE, TEXT("Layer_Left_Exit"), TEXT("Layer_Right_Exit"));
	m_Open_2 = m_EventCube2->Open_Event(LEVEL_STAGEONE, TEXT("Layer_Second_Left_Entrance"), TEXT("Layer_Second_Right_Entrance"));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_StageOne::Open_Exit2()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_Open_3 = m_EventCube3->Open_Block_Event(LEVEL_STAGEONE, TEXT("Layer_Second_Left_Exit"), TEXT("Layer_Second_Right_Exit"));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_StageOne* CLevel_StageOne::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_StageOne* pInstance = new CLevel_StageOne(pGraphic_Device);

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

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

	if (FAILED(Ready_Layer_Next_Stage()))
	{
		MSGBOX("Failed to Ready_Layer_Next_Stage in CLevel_StageOne");
		return E_FAIL;
	}

	SetWindowText(g_hWnd, TEXT("PORTAL_STAGE1"));

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	pInstance->SetMouseMode(false,g_hWnd);
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

	if(0 > __super::Tick(fTimeDelta))
		return -1;

	if(!m_Open_1)
	{
		if (FAILED(Open_Exit()))
		{
			MSGBOX("Failed to Open_Exit in CLevel_StageOne");
			return E_FAIL;
		}
	}

	if(!m_Close_1 || !m_Open_2)
	{
		m_player->Get_Info().Pos = _float3(15.f,1.f,35.f);
		if (FAILED(Close_Exit_Open_Door2()))
		{
			MSGBOX("Failed to Close_Exit_Open_Door2 in CLevel_StageOne");
			return E_FAIL;
		}
	}

	if(m_Open_2 && !m_Open_3)
	{
		if (FAILED(Open_Exit2()))
		{
			MSGBOX("Failed to Open_Exit2 in CLevel_StageOne");
			return E_FAIL;
		}
	}

	
	

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);

	if(p_instance->Get_Key_Down(DIK_0))
	{
		if (FAILED(p_instance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_STAGETWO))))
		{
			RELEASE_INSTANCE(CGameInstance);
			return -1;
		}
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

HRESULT CLevel_StageOne::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


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
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
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
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
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
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
		transform->Set_State(CTransform::STATE_POSITION, _float3((iWaterZ * 0.5f) + ((_float)iWaterZ) + 2.f, ((_float)iWaterZ) * 0.5f, -(iWaterZ * 0.5f)- 2.f));
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
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
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
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		transform->Set_State(CTransform::STATE_POSITION, _float3(0.f, (_float)iLineY - 1 - 4, 0.f));
		transform->Scaled(_float3(70.f, 60.f, 40.f));
		transform->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(270));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iLineX, 1.f, (_float)iLineZ));
		box->Set_AdditionalPos(_float3(iLineX * 0.5f,  0.5f, iLineZ * 0.5f));

	}


	// 뒷면
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_B_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_B_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
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
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
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
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
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
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
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
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
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
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
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
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
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
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
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
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
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
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
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
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
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
	ExitTrans->Scaled(_float3(1.f, 1.f, 1.f));
	ExitTrans->Set_State(CTransform::STATE_POSITION, _float3(11.f, 1.f, 51.f));

	CBoxCollider* ExitBox = static_cast<CBoxCollider*>(m_Change->Get_Component(COM_COLLIDER));
	ExitBox->Set_State(CBoxCollider::COLL_SIZE, _float3(3.f, 1.f, 3.f));

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

	// 두번째 Exit 바닥 충돌체
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Exit_Bot_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Exit_Bot_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		transform->Scaled(_float3((_float)iWaterZ, (_float)iWaterZ, (_float)iWaterZ));
		transform->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90));
		transform->Set_State(CTransform::STATE_POSITION, _float3((iLineX) * 0.5f - 0.5f, 1.f, (_float)iLineZ * 2.f + 2.f));

		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ, 0.1f, (_float)iWaterZ));
	}

	// 두번째 Exit 오른쪽면 충돌체
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Exit_R_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Exit_R_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		transform->Scaled(_float3((_float)iWaterZ, (_float)iWaterZ, (_float)iWaterZ));
		transform->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90));
		transform->Set_State(CTransform::STATE_POSITION, _float3(_float3((iLineX) * 0.5f + 2.f, (_float)iWaterZ * 0.5f, (_float)iLineZ * 2.f + 2.f)));

		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		//box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ, (_float)iWaterZ, 0.5f));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3(0.1f, (_float)iWaterZ, (_float)iWaterZ));
	}

	// 두번째 Exit 왼쪽면 충돌체
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Exit_L_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Exit_L_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		transform->Scaled(_float3((_float)iWaterZ, (_float)iWaterZ, (_float)iWaterZ));
		transform->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(270));
		transform->Set_State(CTransform::STATE_POSITION, _float3(_float3((iLineX) * 0.5f - 3.f, (_float)iWaterZ * 0.5f, (_float)iLineZ * 2.f + 2.f)));

		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		//box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ, (_float)iWaterZ, 0.5f));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3(0.1f, (_float)iWaterZ, (_float)iWaterZ));
	}
	// 두번째 Exit 천장 충돌체
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Exit_T_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Exit_T_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(0);
		transform->Scaled(_float3((_float)iWaterZ, (_float)iWaterZ + 1.f, (_float)iWaterZ));
		transform->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(270));
		transform->Set_State(CTransform::STATE_POSITION, _float3((iLineX) * 0.5f - 0.5f, (_float)iWaterZ - 1.f, (_float)iLineZ * 2.f + 2.f));

		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ + 1.f, 0.1f, (_float)iWaterZ));
	}

	// 두번째 Exit 정면 충돌체
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Exit_F_Collider"), TEXT("Prototype_GameObject_TileCollider"))))
			return E_FAIL;

		CGameObject* tileCollider = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Second_Exit_F_Collider"), 0);
		// tile의 위치
		CTransform* transform = (CTransform*)tileCollider->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(tileCollider)->Set_TextureIndex(1);
		transform->Scaled(_float3((_float)iWaterZ, (_float)iWaterZ + 1.f, (_float)iWaterZ));
		transform->Set_State(CTransform::STATE_POSITION, _float3((iLineX) * 0.5f - 0.5f, (_float)iWaterZ - 1.f, (_float)iLineZ * 2.f + 5.f -2.f) );

		// collider의 위치
		CBoxCollider* box = static_cast<CBoxCollider*>(tileCollider->Get_Component(COM_COLLIDER));

		box->Set_State(CBoxCollider::COLL_SIZE, _float3((_float)iWaterZ + 1.f, (_float)iWaterZ, 1.f));
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

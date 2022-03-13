#include "stdafx.h"
#include "Level_StageTwo.h"

#include "BazierBullet.h"
#include "Boss.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Door_left.h"
#include "Door_right.h"
#include "UI.h"
#include "Player.h"
#include "Tile_Collider.h"
#include "Tile_Cube.h"
#include "Transform.h"
#include "Player.h"

CLevel_StageTwo::CLevel_StageTwo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{

}

HRESULT CLevel_StageTwo::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;


	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Map()))
		return E_FAIL;

	if (FAILED(Ready_Layer_JumpMap()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("PORTAL_STAGE2"));

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	pInstance->PlayBGM(TEXT("Boss_Bgm.mp3"));
	pInstance->SetMouseMode(false, g_hWnd);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CLevel_StageTwo::Tick(_float fTimeDelta)
{
	if (!m_setting)
	{
		CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
		p_instance->SetMouseMode(false, g_hWnd);
		RELEASE_INSTANCE(CGameInstance);
		m_setting = true;
	}

	if (0 > __super::Tick(fTimeDelta))
		return -1;

	if (!m_Open_2)
	{
		if (FAILED(Open_Exit()))
		{
			MSGBOX("Failed to Open_Exit in CLevel_StageOne");
			return E_FAIL;
		}
	}

	if (!m_Save)
	{
		if (FAILED(Save_Point()))
		{
			MSGBOX("Failed to Open_Exit in CLevel_StageOne");
			return E_FAIL;
		}
	}


	if (m_Save && m_BossSpone == false)
	{
		m_BossSpone = true;
		CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
		if (FAILED(p_instance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_Wall"), TEXT("Prototype_GameObject_TileCollider"))))
			return -1;
		CGameObject* BottomTileFront2 = p_instance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_Wall"));
		CTransform* BottomTransFront2 = (CTransform*)BottomTileFront2->Get_Component(COM_TRANSFORM);
		static_cast<CTileCollider*>(BottomTileFront2)->Set_TextureIndex(1);
		//실제 보이는 박스
		BottomTransFront2->Scaled(_float3(m_iJumpBoxSize, HalfBoxSize - 20.f, HalfBoxSize));
		BottomTransFront2->Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(-90.f));
		BottomTransFront2->Set_State(CTransform::STATE_POSITION, _float3(0.f, m_iBoxSize + m_iJumpBoxSize * 0.5f, HalfBoxSize));;
		CBoxCollider* boxFront2 = static_cast<CBoxCollider*>(BottomTileFront2->Get_Component(COM_COLLIDER));
		//충돌박스
		boxFront2->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iJumpBoxSize + m_iJumpBoxSize * 0.5f, HalfBoxSize - 10, WallSize));
		boxFront2->Set_AdditionalPos(_float3(0.f, 0.f, HalfWallSize));
		boxFront2->Set_Collider();
		m_TileList.push_back(static_cast<CTileCollider*>(BottomTileFront2));

		m_pPlayer->Set_Shake(1.f, 2.f);

		p_instance->Play_Sound(TEXT("Boss_Spawn.wav"), CSoundMgr::EFFECT3, 1.f);

		RELEASE_INSTANCE(CGameInstance);
	}

	if(m_Changed && !m_Spawning)
	{
		static_cast<CBoxCollider*>(m_EventCube_Save_Exit->Get_Component(COM_COLLIDER))->Set_Dead(false);

		if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
			return E_FAIL;
		m_pPlayer->Set_Shake(1.f, 2.f);

		CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
		p_instance->StopSound(CSoundMgr::EFFECT3);
		RELEASE_INSTANCE(CGameInstance);
		m_Spawning = true;
	}

	if(!m_ChangedTile)
	{
		Spawn_Boss_Tile(fTimeDelta);
	}

	if (!m_ChangedTile2)
	{
		Change_Boss_Tile(fTimeDelta);
	}

	if(m_pBoss && m_pBoss->Get_HP() <= 0)
	{
		m_SoundTimer += fTimeDelta;
		CGameInstance* p_instance = GET_INSTANCE(CGameInstance);

		if (m_SoundTimer < 1.5f)
		{
			m_pPlayer->Set_Shake(1.0f, 2.f);
			p_instance->BGMVolumeDown(fTimeDelta * 0.5f);
			g_ControlTime -= fTimeDelta * 0.6f;
			g_ControlShader += fTimeDelta * 0.3f;
		}
		else
		{
			p_instance->StopAll();
			g_ControlShader = 1.f;
			g_ControlTime = 0.2f;
		}
		RELEASE_INSTANCE(CGameInstance);

	}

	m_setting = true;

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

HRESULT CLevel_StageTwo::Ready_Layer_Camera(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* 사본객체를 생성ㅎ나다. */
	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CameraDesc));

	CameraDesc.vEye = _float3(0.f, 0.f, -7.f);
	CameraDesc.vAt = _float3(0.f, 0.f, -10.f);
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
	desc2.Shader_Style = SHADER_SETCOLOR_BLEND;
	desc2.Shader_Control = &g_ControlShader;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("BackUI"), TEXT("Prototype_GameObject_BackUI"), &desc2)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_StageTwo::Ready_Layer_Player(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer::PLAYERINFO info;
	info.Hp = 100;
	info.Pos = _float3(0.f, 85.f, 80.f);


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, pLayerTag, TEXT("Prototype_GameObject_Player"), &info)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}


	m_pPlayer = static_cast<CPlayer*>(pGameInstance->Get_GameObject_End(LEVEL_STAGETWO, pLayerTag));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_StageTwo::Ready_Layer_Monster(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Boss"), TEXT("Prototype_GameObject_Boss"))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_pBoss = static_cast<CBoss*>(pGameInstance->Get_GameObject_End(LEVEL_STAGETWO, TEXT("Boss")));

	m_pBoss->Set_InitPos(_float3(0.f, 10.f, 0.f));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_StageTwo::Ready_Layer_Map()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//문


	// Start Left Door
	{

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Left_Door"), TEXT("Prototype_GameObject_Left_Door"))))
			return E_FAIL;

		CGameObject* Left_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Left_Door"));
		CTransform* Lefttrans = (CTransform*)Left_Door->Get_Component(COM_TRANSFORM);

		Lefttrans->Scaled(_float3(fDoorSizeX, fDoorSizeY, 1.f));
		Lefttrans->Set_State(CTransform::STATE_POSITION, _float3(-10.f, 90.f, 85.f));

		CBoxCollider* box = static_cast<CBoxCollider*>(Left_Door->Get_Component(COM_COLLIDER));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3(fDoorSizeX, fDoorSizeY, 1.f));
		box->Set_Collider();

	}
	// Start Right Door
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Right_Door"), TEXT("Prototype_GameObject_Right_Door"))))
			return E_FAIL;

		CGameObject* Right_Door = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Right_Door"));
		CTransform* Righttrans = (CTransform*)Right_Door->Get_Component(COM_TRANSFORM);

		Righttrans->Scaled(_float3(fDoorSizeX, fDoorSizeY, 1.f));
		Righttrans->Set_State(CTransform::STATE_POSITION, _float3(10.f, 90.f, 85.f));

		CBoxCollider* box = static_cast<CBoxCollider*>(Right_Door->Get_Component(COM_COLLIDER));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3(fDoorSizeX, fDoorSizeY, 1.f));
		box->Set_Collider();

	}


	// Exit Left Door
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_Left_Exit"), TEXT("Prototype_GameObject_Left_Door"))))
			return E_FAIL;

		CGameObject* Left_Door = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_Left_Exit"));
		CTransform* Lefttrans = (CTransform*)Left_Door->Get_Component(COM_TRANSFORM);
		static_cast<CDoor_left*>(Left_Door)->Set_TextureIndex(0);
		Lefttrans->Scaled(_float3(fDoorSizeX, fDoorSizeY, 1.f));
		Lefttrans->Set_State(CTransform::STATE_POSITION, _float3(-10.f, 90.f, 50.f));

		CBoxCollider* box = static_cast<CBoxCollider*>(Left_Door->Get_Component(COM_COLLIDER));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3(fDoorSizeX, fDoorSizeY, 1.f));
		box->Set_Collider();

	}

	// Exit Right Door
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_Right_Exit"), TEXT("Prototype_GameObject_Right_Door"))))
			return E_FAIL;

		CGameObject* Right_Door = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_Right_Exit"));
		CTransform* Righttrans = (CTransform*)Right_Door->Get_Component(COM_TRANSFORM);
		static_cast<CDoor_right*>(Right_Door)->Set_TextureIndex(0);
		Righttrans->Scaled(_float3(fDoorSizeX, fDoorSizeY, 1.f));
		Righttrans->Set_State(CTransform::STATE_POSITION, _float3(10.f, 90.f, 50.f));

		CBoxCollider* box = static_cast<CBoxCollider*>(Right_Door->Get_Component(COM_COLLIDER));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3(fDoorSizeX, fDoorSizeY, 1.f));
		box->Set_Collider();

	}

	//문여는 큐브
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_Open_Exit"), TEXT("Prototype_GameObject_Block"))))
			return E_FAIL;

		CGameObject* Switch = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_Open_Exit"), 0);
		CTransform* trans = (CTransform*)Switch->Get_Component(COM_TRANSFORM);
		trans->Scaled(_float3(1.f, 1.f, 1.f));
		trans->Set_State(CTransform::STATE_POSITION, _float3(0.f, 81.f, 55.f));
		CBoxCollider* box = static_cast<CBoxCollider*>(Switch->Get_Component(COM_COLLIDER));
		box->Set_State(CBoxCollider::COLL_SIZE,_float3(100.f,100.f,2.f));
		box->Set_Collider();

	}

	//문 닫으면 서 세이브 포인트
	{
		if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_Save_Exit"), TEXT("Prototype_GameObject_Block"))))
			return E_FAIL;

		CGameObject* Switch = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_Save_Exit"), 0);
		CTransform* trans = (CTransform*)Switch->Get_Component(COM_TRANSFORM);
		trans->Scaled(_float3(1.f, 1.f, 1.f));
		trans->Set_State(CTransform::STATE_POSITION, _float3(0.f, 1.f, 0.f));

		CBoxCollider* box = static_cast<CBoxCollider*>(Switch->Get_Component(COM_COLLIDER));
		box->Set_State(CBoxCollider::COLL_SIZE, _float3(15.f, 15.f, 15.f));
		box->Set_Collider();
	}

	m_EventCube_Save_Exit = static_cast<CTile_Cube*>(pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_Save_Exit"), 0));

	m_EventCube_Save_Exit->Set_TextureIndex(4);

	m_EventCubeTr = static_cast<CTransform*>(m_EventCube_Save_Exit->Get_Component(COM_TRANSFORM));

	m_EventCube_Open = static_cast<CTile_Cube*>(pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_Open_Exit"), 0));


	/////





	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_Bottom2"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* BottomTile = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_Bottom2"));
	CTransform* BottomTrans = (CTransform*)BottomTile->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(BottomTile)->Set_TextureIndex(1);
	//실제 보이는 박스
	BottomTrans->Scaled(_float3(m_iBoxSize, m_iBoxSize, m_iBoxSize));
	BottomTrans->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	BottomTrans->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.f, 0.f));
	CBoxCollider* box = static_cast<CBoxCollider*>(BottomTile->Get_Component(COM_COLLIDER));
	//충돌박스
	box->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iBoxSize, WallSize, m_iBoxSize));
	box->Set_AdditionalPos(_float3(0.f, -HalfWallSize, 0.f));
	box->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(BottomTile));



	//천장
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_Bottom3"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* BottomTileBottom1 = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_Bottom3"));
	CTransform* BottomTransBottom1 = (CTransform*)BottomTileBottom1->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(BottomTileBottom1)->Set_TextureIndex(1);
	//실제 보이는 박스
	BottomTransBottom1->Scaled(_float3(m_iBoxSize, m_iBoxSize, m_iBoxSize));
	BottomTransBottom1->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(-90.f));
	BottomTransBottom1->Set_State(CTransform::STATE_POSITION, _float3(0.f, m_iBoxSize + m_iJumpBoxSize, 0.f));
	CBoxCollider* boxBottom1 = static_cast<CBoxCollider*>(BottomTileBottom1->Get_Component(COM_COLLIDER));
	//충돌박스
	boxBottom1->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iBoxSize, WallSize, m_iBoxSize));
	boxBottom1->Set_AdditionalPos(_float3(0.f, HalfWallSize, 0.f));
	boxBottom1->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(BottomTileBottom1));


	//왼쪽
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_Left1"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* BottomTile1 = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_Left1"));
	CTransform* BottomTrans1 = (CTransform*)BottomTile1->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(BottomTile1)->Set_TextureIndex(1);
	//실제 보이는 박스
	BottomTrans1->Scaled(_float3(m_iBoxSize, m_iBoxSize, m_iBoxSize));
	BottomTrans1->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(-90.f));
	BottomTrans1->Set_State(CTransform::STATE_POSITION, _float3(-HalfBoxSize, HalfBoxSize, 0.f));;
	CBoxCollider* box1 = static_cast<CBoxCollider*>(BottomTile1->Get_Component(COM_COLLIDER));
	//충돌박스
	box1->Set_State(CBoxCollider::COLL_SIZE, _float3(WallSize, m_iBoxSize, m_iBoxSize));
	box1->Set_AdditionalPos(_float3(-HalfWallSize, 0.f, 0.f));
	box1->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(BottomTile1));

	//왼쪽 ↑
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_Left11"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* BottomTile11 = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_Left11"));
	CTransform* BottomTrans11 = (CTransform*)BottomTile11->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(BottomTile11)->Set_TextureIndex(1);
	//실제 보이는 박스
	BottomTrans11->Scaled(_float3(m_iBoxSize, m_iJumpBoxSize, m_iJumpBoxSize));
	BottomTrans11->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(-90.f));
	BottomTrans11->Set_State(CTransform::STATE_POSITION, _float3(-HalfBoxSize, m_iBoxSize + m_iJumpBoxSize * 0.5f, 0.f));;
	CBoxCollider* box11 = static_cast<CBoxCollider*>(BottomTile11->Get_Component(COM_COLLIDER));
	//충돌박스
	box11->Set_State(CBoxCollider::COLL_SIZE, _float3(WallSize, m_iBoxSize, m_iBoxSize));
	box11->Set_AdditionalPos(_float3(-HalfWallSize, 0.f, 0.f));
	box11->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(BottomTile11));

	//오른쪽
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_Right"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* BottomTile4 = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_Right"));
	CTransform* BottomTrans4 = (CTransform*)BottomTile4->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(BottomTile4)->Set_TextureIndex(1);
	//실제 보이는 박스
	BottomTrans4->Scaled(_float3(m_iBoxSize, m_iBoxSize, m_iBoxSize));
	BottomTrans4->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
	BottomTrans4->Set_State(CTransform::STATE_POSITION, _float3(HalfBoxSize, HalfBoxSize, 0.f));;
	CBoxCollider* box4 = static_cast<CBoxCollider*>(BottomTile4->Get_Component(COM_COLLIDER));
	//충돌박스
	box4->Set_State(CBoxCollider::COLL_SIZE, _float3(WallSize, m_iBoxSize, m_iBoxSize));
	box4->Set_AdditionalPos(_float3(HalfWallSize, 0.f, 0.f));
	box4->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(BottomTile4));

	//오른쪽 ↑
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_Right11"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* BottomTile41 = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_Right11"));
	CTransform* BottomTrans41 = (CTransform*)BottomTile41->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(BottomTile41)->Set_TextureIndex(1);
	//실제 보이는 박스
	BottomTrans41->Scaled(_float3(m_iBoxSize, m_iJumpBoxSize, m_iJumpBoxSize));
	BottomTrans41->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
	BottomTrans41->Set_State(CTransform::STATE_POSITION, _float3(HalfBoxSize, m_iBoxSize + m_iJumpBoxSize * 0.5f, 0.f));;
	CBoxCollider* box41 = static_cast<CBoxCollider*>(BottomTile41->Get_Component(COM_COLLIDER));
	//충돌박스
	box41->Set_State(CBoxCollider::COLL_SIZE, _float3(WallSize, m_iBoxSize, m_iBoxSize));
	box41->Set_AdditionalPos(_float3(HalfWallSize, 0.f, 0.f));
	box41->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(BottomTile41));

	//정면
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_Front"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* BottomTile2 = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_Front"));
	CTransform* BottomTrans2 = (CTransform*)BottomTile2->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(BottomTile2)->Set_TextureIndex(1);
	//실제 보이는 박스
	BottomTrans2->Scaled(_float3(m_iBoxSize, m_iBoxSize, m_iBoxSize));
	BottomTrans2->Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(-90.f));
	BottomTrans2->Set_State(CTransform::STATE_POSITION, _float3(0.f, HalfBoxSize, HalfBoxSize));;
	CBoxCollider* box2 = static_cast<CBoxCollider*>(BottomTile2->Get_Component(COM_COLLIDER));
	//충돌박스
	box2->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iBoxSize, m_iBoxSize, WallSize));
	box2->Set_AdditionalPos(_float3(0.f, 0.f, HalfWallSize));
	box2->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(BottomTile2));

	//정면↑ →
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_Front1"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* BottomTileFront2 = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_Front1"));
	CTransform* BottomTransFront2 = (CTransform*)BottomTileFront2->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(BottomTileFront2)->Set_TextureIndex(1);
	//실제 보이는 박스
	BottomTransFront2->Scaled(_float3(m_iJumpBoxSize, HalfBoxSize - 10, HalfBoxSize - 10));
	BottomTransFront2->Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(-90.f));
	BottomTransFront2->Set_State(CTransform::STATE_POSITION, _float3(25.f, m_iBoxSize + m_iJumpBoxSize * 0.5f, HalfBoxSize));;
	CBoxCollider* boxFront2 = static_cast<CBoxCollider*>(BottomTileFront2->Get_Component(COM_COLLIDER));
	//충돌박스
	boxFront2->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iJumpBoxSize + m_iJumpBoxSize * 0.5f, HalfBoxSize - 10, WallSize));
	boxFront2->Set_AdditionalPos(_float3(0.f, 0.f, HalfWallSize));
	boxFront2->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(BottomTileFront2));

	//정면↑ ←
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_Front2"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* BottomTileFront3 = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_Front2"));
	CTransform* BottomTransFront3 = (CTransform*)BottomTileFront3->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(BottomTileFront3)->Set_TextureIndex(1);
	//실제 보이는 박스
	BottomTransFront3->Scaled(_float3(m_iJumpBoxSize, HalfBoxSize - 10, HalfBoxSize - 10));
	BottomTransFront3->Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(-90.f));
	BottomTransFront3->Set_State(CTransform::STATE_POSITION, _float3(-25.f, m_iBoxSize + m_iJumpBoxSize * 0.5f, HalfBoxSize));;
	CBoxCollider* boxFront3 = static_cast<CBoxCollider*>(BottomTileFront3->Get_Component(COM_COLLIDER));
	//충돌박스
	boxFront3->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iJumpBoxSize + m_iJumpBoxSize * 0.5f, HalfBoxSize - 10, WallSize));
	boxFront3->Set_AdditionalPos(_float3(0.f, 0.f, HalfWallSize));
	boxFront3->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(BottomTileFront3));

	//뒤에
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_Back"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* BottomTile3 = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_Back"));
	CTransform* BottomTrans3 = (CTransform*)BottomTile3->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(BottomTile3)->Set_TextureIndex(1);
	//실제 보이는 박스
	BottomTrans3->Scaled(_float3(m_iBoxSize, m_iBoxSize, m_iBoxSize));
	//BottomTrans3->Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(90.f));
	BottomTrans3->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180.f));
	BottomTrans3->Set_State(CTransform::STATE_POSITION, _float3(0.f, HalfBoxSize, -HalfBoxSize));;
	CBoxCollider* box3 = static_cast<CBoxCollider*>(BottomTile3->Get_Component(COM_COLLIDER));
	//충돌박스
	box3->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iBoxSize, m_iBoxSize, WallSize));
	box3->Set_AdditionalPos(_float3(0.f, 0.f, -HalfWallSize));
	box3->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(BottomTile3));


	//뒤에  ↑
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_Back2"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* BottomTile33 = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_Back2"));
	CTransform* BottomTrans33 = (CTransform*)BottomTile33->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(BottomTile33)->Set_TextureIndex(1);
	//실제 보이는 박스
	BottomTrans33->Scaled(_float3(m_iBoxSize, m_iJumpBoxSize, m_iJumpBoxSize));
	//BottomTrans3->Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(90.f));
	BottomTrans33->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180.f));
	BottomTrans33->Set_State(CTransform::STATE_POSITION, _float3(0.f, m_iBoxSize + m_iJumpBoxSize * 0.5f, -HalfBoxSize));;
	CBoxCollider* box33 = static_cast<CBoxCollider*>(BottomTile33->Get_Component(COM_COLLIDER));
	//충돌박스
	box33->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iBoxSize, m_iBoxSize, WallSize));
	box33->Set_AdditionalPos(_float3(0.f, 0.f, -HalfWallSize));
	box33->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(BottomTile33));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_StageTwo::Ready_Layer_JumpMap()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	//아래
	//아래

	_float3 JumpMapCenterPos = _float3(0.f, 60.f, 65.f);


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpBottom2"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* JumpBottomTile = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpBottom2"));
	CTransform* JumpBottomTrans = (CTransform*)JumpBottomTile->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(JumpBottomTile)->Set_TextureIndex(1);
	//실제 보이는 박스
	JumpBottomTrans->Scaled(_float3(m_iJumpBoxSize, JumpHalfBoxSize, m_iJumpBoxSize));
	JumpBottomTrans->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	JumpBottomTrans->Set_State(CTransform::STATE_POSITION, _float3(JumpMapCenterPos.x, JumpMapCenterPos.y, JumpMapCenterPos.z));
	CBoxCollider* Jumpbox = static_cast<CBoxCollider*>(JumpBottomTile->Get_Component(COM_COLLIDER));
	//충돌박스
	Jumpbox->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iJumpBoxSize, JumpWallSize, m_iJumpBoxSize));
	JumpBottomTrans->Scaled(_float3(m_iJumpBoxSize, JumpHalfBoxSize, m_iJumpBoxSize));
	Jumpbox->Set_AdditionalPos(_float3(0.f, -JumpHalfWallSize, 0.f));
	Jumpbox->Set_Collider();
	//천장
	m_TileList.push_back(static_cast<CTileCollider*>(JumpBottomTile));

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpTop"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* JumpTopTile = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpTop"));
	CTransform* JumpTopTrans = (CTransform*)JumpTopTile->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(JumpTopTile)->Set_TextureIndex(1);
	//실제 보이는 박스
	JumpTopTrans->Scaled(_float3(m_iJumpBoxSize, JumpHalfBoxSize, m_iJumpBoxSize));
	JumpTopTrans->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(-90.f));
	JumpTopTrans->Set_State(CTransform::STATE_POSITION, _float3(JumpMapCenterPos.x, JumpMapCenterPos.y + m_iJumpBoxSize * 2.f, JumpMapCenterPos.z));
	CBoxCollider* JumpTopbox = static_cast<CBoxCollider*>(JumpTopTile->Get_Component(COM_COLLIDER));
	//충돌박스
	JumpTopbox->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iJumpBoxSize, JumpWallSize, m_iJumpBoxSize));
	JumpTopbox->Set_AdditionalPos(_float3(0.f, JumpHalfWallSize, 0.f));
	JumpTopbox->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(JumpTopTile));


	//왼쪽
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpLeft1"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* JumpBottomTile1 = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpLeft1"));
	CTransform* JumpBottomTrans1 = (CTransform*)JumpBottomTile1->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(JumpBottomTile1)->Set_TextureIndex(1);
	//실제 보이는 박스
	JumpBottomTrans1->Scaled(_float3(JumpHalfBoxSize, m_iJumpBoxSize, m_iJumpBoxSize));
	JumpBottomTrans1->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(-90.f));
	JumpBottomTrans1->Set_State(CTransform::STATE_POSITION, _float3(JumpMapCenterPos.x - JumpHalfBoxSize, JumpMapCenterPos.y + JumpHalfBoxSize, JumpMapCenterPos.z));
	CBoxCollider* Jumpbox1 = static_cast<CBoxCollider*>(JumpBottomTile1->Get_Component(COM_COLLIDER));
	//충돌박스
	Jumpbox1->Set_State(CBoxCollider::COLL_SIZE, _float3(JumpWallSize, m_iJumpBoxSize, m_iJumpBoxSize));
	Jumpbox1->Set_AdditionalPos(_float3(0.f - JumpHalfWallSize, 0.f, 0.f));
	Jumpbox1->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(JumpBottomTile1));

	//왼쪽 ↑
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpLeft2"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* JumpBottomTileLeft2 = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpLeft2"));
	CTransform* JumpBottomTransLeft2 = (CTransform*)JumpBottomTileLeft2->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(JumpBottomTileLeft2)->Set_TextureIndex(1);
	//실제 보이는 박스
	JumpBottomTransLeft2->Scaled(_float3(JumpHalfBoxSize, m_iJumpBoxSize, m_iJumpBoxSize));
	JumpBottomTransLeft2->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(-90.f));
	JumpBottomTransLeft2->Set_State(CTransform::STATE_POSITION, _float3(JumpMapCenterPos.x - JumpHalfBoxSize, JumpMapCenterPos.y + JumpHalfBoxSize + m_iJumpBoxSize, JumpMapCenterPos.z));
	CBoxCollider* JumpboxLeft2 = static_cast<CBoxCollider*>(JumpBottomTileLeft2->Get_Component(COM_COLLIDER));
	//충돌박스
	JumpboxLeft2->Set_State(CBoxCollider::COLL_SIZE, _float3(JumpWallSize, m_iJumpBoxSize, m_iJumpBoxSize));
	JumpboxLeft2->Set_AdditionalPos(_float3(0.f - JumpHalfWallSize, 0.f, 0.f));
	JumpboxLeft2->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(JumpBottomTileLeft2));

	//왼쪽 ↑ →
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpLeft3"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* JumpBottomTileLeft3 = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpLeft3"));
	CTransform* JumpBottomTransLeft3 = (CTransform*)JumpBottomTileLeft3->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(JumpBottomTileLeft3)->Set_TextureIndex(1);
	//실제 보이는 박스
	JumpBottomTransLeft3->Scaled(_float3(m_iJumpBoxSize, m_iJumpBoxSize, m_iJumpBoxSize));
	JumpBottomTransLeft3->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(-90.f));
	JumpBottomTransLeft3->Set_State(CTransform::STATE_POSITION, _float3(JumpMapCenterPos.x - JumpHalfBoxSize, JumpMapCenterPos.y + JumpHalfBoxSize + m_iJumpBoxSize, JumpMapCenterPos.z + m_iJumpBoxSize - JumpHalfBoxSize * 0.5f));
	CBoxCollider* JumpboxLeft3 = static_cast<CBoxCollider*>(JumpBottomTileLeft3->Get_Component(COM_COLLIDER));
	//충돌박스
	JumpboxLeft3->Set_State(CBoxCollider::COLL_SIZE, _float3(JumpWallSize, m_iJumpBoxSize, m_iJumpBoxSize));
	JumpboxLeft3->Set_AdditionalPos(_float3(0.f - JumpHalfWallSize, 0.f, 0.f));
	JumpboxLeft3->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(JumpBottomTileLeft3));

	//왼쪽 ↑ ←
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpLeft4"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* JumpBottomTileLeft4 = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpLeft4"));
	CTransform* JumpBottomTransLeft4 = (CTransform*)JumpBottomTileLeft4->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(JumpBottomTileLeft4)->Set_TextureIndex(1);
	//실제 보이는 박스
	JumpBottomTransLeft4->Scaled(_float3(m_iJumpBoxSize, m_iJumpBoxSize, m_iJumpBoxSize));
	JumpBottomTransLeft4->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(-90.f));
	JumpBottomTransLeft4->Set_State(CTransform::STATE_POSITION, _float3(JumpMapCenterPos.x - JumpHalfBoxSize, JumpMapCenterPos.y + JumpHalfBoxSize + m_iJumpBoxSize, JumpMapCenterPos.z - m_iJumpBoxSize + JumpHalfBoxSize * 0.5f));
	CBoxCollider* JumpboxLeft4 = static_cast<CBoxCollider*>(JumpBottomTileLeft4->Get_Component(COM_COLLIDER));
	//충돌박스
	JumpboxLeft4->Set_State(CBoxCollider::COLL_SIZE, _float3(JumpWallSize, m_iJumpBoxSize, m_iJumpBoxSize));
	JumpboxLeft4->Set_AdditionalPos(_float3(0.f - JumpHalfWallSize, 0.f, 0.f));
	JumpboxLeft4->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(JumpBottomTileLeft4));

	//오른쪽 ↑
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpRight1"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* JumpBottomTileRight1 = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpRight1"));
	CTransform* JumpBottomTransRight1 = (CTransform*)JumpBottomTileRight1->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(JumpBottomTileRight1)->Set_TextureIndex(1);
	//실제 보이는 박스
	JumpBottomTransRight1->Scaled(_float3(JumpHalfBoxSize, m_iJumpBoxSize, m_iJumpBoxSize));
	JumpBottomTransRight1->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
	JumpBottomTransRight1->Set_State(CTransform::STATE_POSITION, _float3(JumpMapCenterPos.x + JumpHalfBoxSize, JumpMapCenterPos.y + JumpHalfBoxSize + m_iJumpBoxSize, JumpMapCenterPos.z));
	CBoxCollider* JumpboxRight1 = static_cast<CBoxCollider*>(JumpBottomTileRight1->Get_Component(COM_COLLIDER));
	//충돌박스
	JumpboxRight1->Set_State(CBoxCollider::COLL_SIZE, _float3(JumpWallSize, m_iJumpBoxSize, m_iJumpBoxSize));
	JumpboxRight1->Set_AdditionalPos(_float3(0.f + JumpHalfWallSize, 0.f, 0.f));
	JumpboxRight1->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(JumpBottomTileRight1));

	//오른쪽 ↑ →
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpRight2"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* JumpBottomTileRight2 = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpRight2"));
	CTransform* JumpBottomTransRight2 = (CTransform*)JumpBottomTileRight2->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(JumpBottomTileRight2)->Set_TextureIndex(1);
	//실제 보이는 박스
	JumpBottomTransRight2->Scaled(_float3(m_iJumpBoxSize, m_iJumpBoxSize, m_iJumpBoxSize));
	JumpBottomTransRight2->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
	JumpBottomTransRight2->Set_State(CTransform::STATE_POSITION, _float3(JumpMapCenterPos.x + JumpHalfBoxSize, JumpMapCenterPos.y + JumpHalfBoxSize + m_iJumpBoxSize, JumpMapCenterPos.z + m_iJumpBoxSize - JumpHalfBoxSize * 0.5f));
	CBoxCollider* JumpboxRight2 = static_cast<CBoxCollider*>(JumpBottomTileRight2->Get_Component(COM_COLLIDER));
	//충돌박스
	JumpboxRight2->Set_State(CBoxCollider::COLL_SIZE, _float3(JumpWallSize, m_iJumpBoxSize, m_iJumpBoxSize));
	JumpboxRight2->Set_AdditionalPos(_float3(0.f + JumpHalfWallSize, 0.f, 0.f));
	JumpboxRight2->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(JumpBottomTileRight2));

	//오른쪽 ↑ ←
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpRight3"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* JumpBottomTileRight3 = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpRight3"));
	CTransform* JumpBottomTransRight3 = (CTransform*)JumpBottomTileRight3->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(JumpBottomTileRight3)->Set_TextureIndex(1);
	//실제 보이는 박스
	JumpBottomTransRight3->Scaled(_float3(m_iJumpBoxSize, m_iJumpBoxSize, m_iJumpBoxSize));
	JumpBottomTransRight3->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
	JumpBottomTransRight3->Set_State(CTransform::STATE_POSITION, _float3(JumpMapCenterPos.x + JumpHalfBoxSize, JumpMapCenterPos.y + JumpHalfBoxSize + m_iJumpBoxSize, JumpMapCenterPos.z - m_iJumpBoxSize + JumpHalfBoxSize * 0.5f));
	CBoxCollider* JumpboxRight3 = static_cast<CBoxCollider*>(JumpBottomTileRight3->Get_Component(COM_COLLIDER));
	//충돌박스
	JumpboxRight3->Set_State(CBoxCollider::COLL_SIZE, _float3(JumpWallSize, m_iJumpBoxSize, m_iJumpBoxSize));
	JumpboxRight3->Set_AdditionalPos(_float3(0.f + JumpHalfWallSize, 0.f, 0.f));
	JumpboxRight3->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(JumpBottomTileRight3));


	//오른쪽
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpRight"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* JumpBottomTile4 = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpRight"));
	CTransform* JumpBottomTrans4 = (CTransform*)JumpBottomTile4->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(JumpBottomTile4)->Set_TextureIndex(1);
	//실제 보이는 박스
	JumpBottomTrans4->Scaled(_float3(JumpHalfBoxSize, m_iJumpBoxSize, m_iJumpBoxSize));
	JumpBottomTrans4->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
	JumpBottomTrans4->Set_State(CTransform::STATE_POSITION, _float3(JumpMapCenterPos.x + JumpHalfBoxSize, JumpMapCenterPos.y + JumpHalfBoxSize, JumpMapCenterPos.z));
	CBoxCollider* Jumpbox4 = static_cast<CBoxCollider*>(JumpBottomTile4->Get_Component(COM_COLLIDER));
	//충돌박스
	Jumpbox4->Set_State(CBoxCollider::COLL_SIZE, _float3(JumpWallSize, m_iJumpBoxSize, m_iJumpBoxSize));
	Jumpbox4->Set_AdditionalPos(_float3(0.f + JumpHalfWallSize, 0.f, 0.f));
	Jumpbox4->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(JumpBottomTile4));


	//정면
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpFront"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* JumpBottomTile2 = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpFront"));
	CTransform* JumpBottomTrans2 = (CTransform*)JumpBottomTile2->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(JumpBottomTile2)->Set_TextureIndex(1);
	//실제 보이는 박스
	JumpBottomTrans2->Scaled(_float3(m_iJumpBoxSize, m_iJumpBoxSize, m_iJumpBoxSize));
	JumpBottomTrans2->Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(-90.f));
	JumpBottomTrans2->Set_State(CTransform::STATE_POSITION, _float3(JumpMapCenterPos.x, JumpMapCenterPos.y + JumpHalfBoxSize, JumpMapCenterPos.z + JumpHalfBoxSize * 0.5f));
	CBoxCollider* Jumpbox2 = static_cast<CBoxCollider*>(JumpBottomTile2->Get_Component(COM_COLLIDER));
	//충돌박스
	Jumpbox2->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iJumpBoxSize, m_iJumpBoxSize, JumpWallSize));
	Jumpbox2->Set_AdditionalPos(_float3(0.f, 0.f, JumpHalfWallSize));
	Jumpbox2->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(JumpBottomTile2));

	//뒤에
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpBack"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* JumpBottomTile3 = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpBack"));
	CTransform* JumpBottomTrans3 = (CTransform*)JumpBottomTile3->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(JumpBottomTile3)->Set_TextureIndex(1);
	//실제 보이는 박스
	JumpBottomTrans3->Scaled(_float3(m_iJumpBoxSize, m_iJumpBoxSize, m_iJumpBoxSize));
	//JumpBottomTrans3->Rotation(_float3(0.f, 0.f, 1.f), D3DXToRadian(90.f));
	JumpBottomTrans3->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180.f));
	JumpBottomTrans3->Set_State(CTransform::STATE_POSITION, _float3(JumpMapCenterPos.x, JumpMapCenterPos.y + JumpHalfBoxSize, JumpMapCenterPos.z - JumpHalfBoxSize * 0.5f));
	CBoxCollider* Jumpbox3 = static_cast<CBoxCollider*>(JumpBottomTile3->Get_Component(COM_COLLIDER));
	//충돌박스
	Jumpbox3->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iJumpBoxSize, m_iJumpBoxSize, JumpWallSize));
	Jumpbox3->Set_AdditionalPos(_float3(0.f, 0.f, JumpHalfWallSize));
	Jumpbox3->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(JumpBottomTile3));


	//바닥 + 위
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpBottom3"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* JumpBottomTile5 = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpBottom3"));
	CTransform* JumpBottomTrans5 = (CTransform*)JumpBottomTile5->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(JumpBottomTile5)->Set_TextureIndex(1);
	//실제 보이는 박스
	JumpBottomTrans5->Scaled(_float3(m_iJumpBoxSize, m_iJumpBoxSize, m_iJumpBoxSize));
	JumpBottomTrans5->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	JumpBottomTrans5->Set_State(CTransform::STATE_POSITION, _float3(JumpMapCenterPos.x, JumpMapCenterPos.y + m_iJumpBoxSize, JumpMapCenterPos.z - (JumpHalfBoxSize + JumpHalfBoxSize * 0.5f)));
	CBoxCollider* Jumpbox5 = static_cast<CBoxCollider*>(JumpBottomTile5->Get_Component(COM_COLLIDER));
	//충돌박스
	Jumpbox5->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iJumpBoxSize, JumpWallSize, m_iJumpBoxSize));
	Jumpbox5->Set_AdditionalPos(_float3(0.f, -JumpHalfWallSize, 0.f));
	Jumpbox5->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(JumpBottomTile5));

	//바닥 + 위2
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpBottom4"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* JumpBottomTile6 = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpBottom4"));
	CTransform* JumpBottomTrans6 = (CTransform*)JumpBottomTile6->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(JumpBottomTile6)->Set_TextureIndex(1);
	//실제 보이는 박스
	JumpBottomTrans6->Scaled(_float3(m_iJumpBoxSize, m_iJumpBoxSize, m_iJumpBoxSize));
	JumpBottomTrans6->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	JumpBottomTrans6->Set_State(CTransform::STATE_POSITION, _float3(JumpMapCenterPos.x, JumpMapCenterPos.y + m_iJumpBoxSize, JumpMapCenterPos.z + JumpHalfBoxSize + JumpHalfBoxSize * 0.5f));
	CBoxCollider* Jumpbox6 = static_cast<CBoxCollider*>(JumpBottomTile6->Get_Component(COM_COLLIDER));
	//충돌박스
	Jumpbox6->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iJumpBoxSize, JumpWallSize, m_iJumpBoxSize));
	Jumpbox6->Set_AdditionalPos(_float3(0.f, -JumpHalfWallSize, 0.f));
	Jumpbox6->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(JumpBottomTile6));

	//천장(바닥 + 위 반대)
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpTop3"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* JumpTopTile5 = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpTop3"));
	CTransform* JumpTopTrans5 = (CTransform*)JumpTopTile5->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(JumpTopTile5)->Set_TextureIndex(1);
	//실제 보이는 박스
	JumpTopTrans5->Scaled(_float3(m_iJumpBoxSize, m_iJumpBoxSize, m_iJumpBoxSize));
	JumpTopTrans5->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(-90.f));
	JumpTopTrans5->Set_State(CTransform::STATE_POSITION, _float3(JumpMapCenterPos.x, JumpMapCenterPos.y + m_iJumpBoxSize * 2.f, JumpMapCenterPos.z - (JumpHalfBoxSize + JumpHalfBoxSize * 0.5f)));
	CBoxCollider* JumpTopbox5 = static_cast<CBoxCollider*>(JumpTopTile5->Get_Component(COM_COLLIDER));
	//충돌박스
	JumpTopbox5->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iJumpBoxSize, JumpWallSize, m_iJumpBoxSize));
	JumpTopbox5->Set_AdditionalPos(_float3(0.f, JumpHalfWallSize, 0.f));
	JumpTopbox5->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(JumpTopTile5));


	//천장(바닥 + 위2 반대)
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpTop4"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* JumpTopTile6 = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_JumpTop4"));
	CTransform* JumpTopTrans6 = (CTransform*)JumpTopTile6->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(JumpTopTile6)->Set_TextureIndex(1);
	//실제 보이는 박스
	JumpTopTrans6->Scaled(_float3(m_iJumpBoxSize, m_iJumpBoxSize, m_iJumpBoxSize));
	JumpTopTrans6->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(-90.f));
	JumpTopTrans6->Set_State(CTransform::STATE_POSITION, _float3(JumpMapCenterPos.x, JumpMapCenterPos.y + m_iJumpBoxSize * 2.f, JumpMapCenterPos.z + JumpHalfBoxSize + JumpHalfBoxSize * 0.5f));
	CBoxCollider* JumpTopbox6 = static_cast<CBoxCollider*>(JumpTopTile6->Get_Component(COM_COLLIDER));
	//충돌박스
	JumpTopbox6->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iJumpBoxSize, JumpWallSize, m_iJumpBoxSize));
	JumpTopbox6->Set_AdditionalPos(_float3(0.f, JumpHalfWallSize, 0.f));
	JumpTopbox6->Set_Collider();
	m_TileList.push_back(static_cast<CTileCollider*>(JumpTopTile6));


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_UnPortal"), TEXT("Prototype_GameObject_UnPortal"))))
		return E_FAIL;

	CGameObject* unPortal = pGameInstance->Get_GameObject_End(LEVEL_STAGETWO, TEXT("Layer_UnPortal"));
	CTransform* unPortalTr = static_cast<CTransform*>(unPortal->Get_Component(COM_TRANSFORM));

	unPortalTr->Set_State(CTransform::STATE_POSITION, _float3(0.f, 80.f, 62.f));
	unPortalTr->Scaled(_float3(30.f, 80.f, 2.f));
	CBoxCollider* unPortalBox = static_cast<CBoxCollider*>(unPortal->Get_Component(COM_COLLIDER));
	unPortalBox->Set_State(CBoxCollider::COLL_SIZE, _float3(30.f, 80.f, 2.f));
	unPortalBox->Set_Collider();


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

HRESULT CLevel_StageTwo::Save_Point()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CGameObject* Switch = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_Save_Exit"));
	CTransform* TransPos = static_cast<CTransform*>(Switch->Get_Component(COM_TRANSFORM));
	m_Save = static_cast<CTile_Cube*>(Switch)->Save_Point();

	if (m_Save)
	{
		m_Open_2 = true;
		CPlayer* Player = static_cast<CPlayer*>(pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_Player")));
		Player->Reset_PlayerPos(TransPos->Get_State(CTransform::STATE_POSITION));

	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_StageTwo::Close_Exit()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CGameObject* Switch = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_Save_Exit"));
	static_cast<CTile_Cube*>(Switch)->Close_Event(g_CurrLevel, TEXT("Layer_Left_Exit"), TEXT("Layer_Right_Exit"));
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CLevel_StageTwo::Spawn_Boss_Tile(_float fTimeDelta)
{
	if (!m_BossSpone)
	{
		m_BlowTimer += fTimeDelta;

		m_EventCubeTr->Set_State(CTransform::STATE_POSITION, _float3(0.f, sinf(D3DXToRadian(m_BlowTimer * 20.f)) + 2.f, 0.f));
		
		return;
	}

	m_Gradiant = false;
	m_EventCubeTr->Turn(_float3(1.f, 1.f, 0.f), fTimeDelta * 10.f);
	m_EventCubeTr->Set_State(CTransform::STATE_POSITION, _float3(0.f, m_EventCubeTr->Get_State(CTransform::STATE_POSITION).y + fTimeDelta * 3.f,0.f));

	if(!m_GradianChangeTile)
	{
		m_Timer += fTimeDelta;
		for(auto& i : m_TileList)
		{
			i->Set_Color(_float4(m_Timer, m_Timer, m_Timer, 0.f));
		}

		m_EventCube_Save_Exit->Set_Color(_float4(m_Timer, m_Timer, m_Timer, 0.f));
	
		if(m_Timer >= 1.f)
		{
			m_GradianChangeTile = true;
			m_Timer = 0.f;
			m_EventCube_Save_Exit->Set_Vaild(false);
			for (auto& i : m_TileList)
			{
				i->Set_TextureIndex(6);
			}
		}
	}
	else if(m_GradianChangeTile)
	{
		m_Timer += fTimeDelta;
		m_Changed = true;
		for (auto& i : m_TileList)
		{
			i->Set_Color(_float4(1.f - m_Timer, 1.f - m_Timer, 1.f - m_Timer, 0.f));
		}

		if (m_Timer >= 1.f)
		{
			m_ChangedTile = true;
			m_GradianChangeTile = false;
			m_Timer = 0.f;
			m_Gradiant = true;

			for (auto& i : m_TileList)
			{
				i->Set_Color(_float4(0.f, 0.f, 0.f, 0.f));
			}

			m_pPlayer->Set_UIColor(_float4(1.f, 1.f, 1.f, 0.f));

		}
	}


}

void CLevel_StageTwo::Change_Boss_Tile(_float fTimeDelta)
{
	if (!m_Spawning || m_pBoss->Get_Phase() != CBoss::BOSS_PHASETWO)
		return;

	m_Gradiant = false;

	if (!m_GradianChangeTile)
	{
		m_Timer += fTimeDelta;
		for (auto& i : m_TileList)
		{
			i->Set_Color(_float4(m_Timer, m_Timer, m_Timer, 0.f));
		}

		if (m_Timer >= 1.f)
		{
			m_GradianChangeTile = true;
			m_Timer = 0.f;
			for (auto& i : m_TileList)
			{
				i->Set_TextureIndex(7);
			}
		}
	}
	else if (m_GradianChangeTile)
	{
		m_Timer += fTimeDelta;

		for (auto& i : m_TileList)
		{
			i->Set_Color(_float4(1.f - m_Timer, 1.f - m_Timer, 1.f - m_Timer, 0.f));
		}

		if (m_Timer >= 1.f)
		{
			m_ChangedTile2 = true;
			m_Timer = 0.f;
			m_Gradiant = true;

			for (auto& i : m_TileList)
			{
				i->Set_Color(_float4(0.f, 0.f, 0.f, 0.f));
			}
		}
	}


}

void CLevel_StageTwo::Gradiant(_float fTimeDelta)
{
	if (!m_Gradiant)
		return;

	m_GradiantTime = m_GrowColor ? m_GradiantTime + fTimeDelta : m_GradiantTime - fTimeDelta;

	if(m_GradiantTime >= 0.3f || m_GradiantTime <= 0.f)
	{
		m_GrowColor = !m_GrowColor;
	}

	for(auto& i : m_TileList)
	{
		i->Set_Color(_float4(m_GradiantTime, m_GradiantTime, m_GradiantTime, 0.f));
	}
}

CLevel_StageTwo* CLevel_StageTwo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_StageTwo* pInstance = new CLevel_StageTwo(pGraphic_Device);

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

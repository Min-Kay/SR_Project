#include "stdafx.h"
#include "..\Public\Level_StageTwo.h"

#include "BazierBullet.h"
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

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("PORTAL_STAGE2"));

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	pInstance->SetMouseMode(false,g_hWnd);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CLevel_StageTwo::Tick(_float fTimeDelta)
{
	if(!m_setting)
	{
		CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
		p_instance->SetMouseMode(false, g_hWnd);
		RELEASE_INSTANCE(CGameInstance);
		m_setting = true; 
	}

	if(0 > __super::Tick(fTimeDelta))
		return -1;

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


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_StageTwo::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer::PLAYERINFO info;
	info.Hp = 100;
	info.Pos = _float3(0.f,10.f,0.f);


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, pLayerTag, TEXT("Prototype_GameObject_Player"),&info)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

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

	CTransform* tr = static_cast<CTransform*>(pGameInstance->Get_GameObject_End(LEVEL_STAGETWO, TEXT("Boss"))->Get_Component(COM_TRANSFORM));
	tr->Set_State(CTransform::STATE_POSITION, _float3(10.f, 10.f, 10.f));


	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Bazier"), TEXT("Prototype_GameObject_BazierBullet"))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	CBazierBullet* bazier = static_cast<CBazierBullet*>(pGameInstance->Get_GameObject_End(LEVEL_STAGETWO, TEXT("Bazier")));

	bazier->Set_Pos(_float3(0.f, 10.f, 0.f), _float3(-5.f, 20.f, 0.f), _float3(-15.f, 0.f, 0.f));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_StageTwo::Ready_Layer_Map()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);



	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGETWO, TEXT("Layer_Bottom2"), TEXT("Prototype_GameObject_TileCollider"))))
		return E_FAIL;
	CGameObject* BottomTile = pGameInstance->Get_GameObject(LEVEL_STAGETWO, TEXT("Layer_Bottom2"));
	CTransform* BottomTrans = (CTransform*)BottomTile->Get_Component(COM_TRANSFORM);
	static_cast<CTileCollider*>(BottomTile)->Set_TextureIndex(1);
	//실제 보이는 박스
	BottomTrans->Scaled(_float3(m_iBoxSize, m_iBoxSize, m_iBoxSize));
	BottomTrans->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	BottomTrans->Set_State(CTransform::STATE_POSITION, _float3(0.f, -10.f, 0.f));
	CBoxCollider* box = static_cast<CBoxCollider*>(BottomTile->Get_Component(COM_COLLIDER));
	//충돌박스
	box->Set_State(CBoxCollider::COLL_SIZE, _float3(m_iBoxSize, WallSize, m_iBoxSize));
	box->Set_AdditionalPos(_float3(0.f, -HalfWallSize, 0.f));

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

	//if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Bottom2"), TEXT("Prototype_GameObject_TileCollider"))))
	//		return E_FAIL;
	//CGameObject* BottomTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Bottom2"));
	//CTransform* BottomTrans = (CTransform*)BottomTile->Get_Component(COM_TRANSFORM);
	//static_cast<CTileCollider*>(BottomTile)->Set_TextureIndex(1);
	//BottomTrans->Scaled(_float3(60.f, 60.f, 60.f));
	//CBoxCollider* box = static_cast<CBoxCollider*>(BottomTile->Get_Component(COM_COLLIDER));
	//box->Set_State(CBoxCollider::COLL_SIZE, _float3(60.f, 60.f, 60.f));
	//box->Set_AdditionalPos(_float3(0.f,-30.f,0.f));
	//BottomTrans->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	//BottomTrans->Set_State(CTransform::STATE_POSITION, _float3(0.f,-1.f,0.f));


	//if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Right"), TEXT("Prototype_GameObject_TileCollider"))))
	//	return E_FAIL;
	//CGameObject* RightTile = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Right"));
	//CTransform* RightTrans = (CTransform*)RightTile->Get_Component(COM_TRANSFORM);
	//static_cast<CTileCollider*>(RightTile)->Set_TextureIndex(1);
	//RightTrans->Scaled(_float3(60.f, 60.f, 60.f));
	//CBoxCollider* box2 = static_cast<CBoxCollider*>(RightTile->Get_Component(COM_COLLIDER));
	//box2->Set_State(CBoxCollider::COLL_SIZE, _float3(60.f, 60.f, 60.f));
	//RightTrans->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
	//box2->Set_AdditionalPos(_float3(30.f, 0.f, 0.f));
	//RightTrans->Set_State(CTransform::STATE_POSITION, _float3(15.f, 0.f, 0.f));


	//if (FAILED(pGameInstance->Add_GameObject(LEVEL_STAGEONE, TEXT("Layer_Bottom1"), TEXT("Prototype_GameObject_TileCollider"))))
	//	return E_FAIL;
	//CGameObject* BottomTile1 = pGameInstance->Get_GameObject(LEVEL_STAGEONE, TEXT("Layer_Bottom1"));
	//CTransform* BottomTrans1 = (CTransform*)BottomTile1->Get_Component(COM_TRANSFORM);
	//static_cast<CTileCollider*>(BottomTile1)->Set_TextureIndex(1);
	//BottomTrans1->Scaled(_float3(20.f, 20.f, 20.f));
	//CBoxCollider* box3 = static_cast<CBoxCollider*>(BottomTile1->Get_Component(COM_COLLIDER));
	//box3->Set_State(CBoxCollider::COLL_SIZE, _float3(20.f, 1.f, 20.f));
	//box3->Set_AdditionalPos(_float3(0.f, 0.f, 0.f));
	//BottomTrans1->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	//BottomTrans1->Set_State(CTransform::STATE_POSITION, _float3(0.f, 25.f, 0.f));

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

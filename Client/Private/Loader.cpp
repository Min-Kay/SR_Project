#include "stdafx.h"
#include "Loader.h"
#include "GameInstance.h"

#include "Player.h"
#include "Portal.h"
#include "BackGround.h"
#include "UI.h"
#include "Sky.h"
#include "Cam_Portal.h"
#include "Camera_Dynamic.h"
#include "Camera_Player.h"
#include "PortalControl.h"
#include "UI_BackUI.h"
#include "Button_Continue.h"
#include "Button_Exit.h"
#include "Terrain.h"


CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

unsigned int APIENTRY ThreadFunction(void* pArg)
{	
	/* CLoader객체안에 로딩에 필요한 데이터들을 담아놨는데 그걸 써야혀? */
	/* CLoader객체의 주소를 가지고있으면 좋을것같다. */
	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CS());

	switch (pLoader->Get_LevelID())
	{
	case LEVEL_GAMEPLAY:
		pLoader->Loading_ForGamePlay();
		break;
	case LEVEL_BOSS:
		pLoader->Loading_ForBoss();
		break;
	}	

	LeaveCriticalSection(&pLoader->Get_CS());	

	return 0;
}



HRESULT CLoader::NativeConstruct(LEVEL eNextLevel)
{
	InitializeCriticalSection(&m_CS);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunction, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	m_eLevel = eNextLevel;

	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlay()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* 원형컴포넌트를 생성한다. */
#pragma region PROTOTYPE_COMPONENT


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PROTO_TERRAIN, CVIBuffer_Terrain::Create(m_pGraphic_Device, 129, 129))))
	{
		MSGBOX("Failed to create terrain in Loader");
		return E_FAIL;
	}

	/* For.Prototype_Component_VIBuffer_Portal */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, PROTO_PORTAL, CVIBuffer_Portal::Create(m_pGraphic_Device))))
		return E_FAIL;

		/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, PROTO_CUBE, CVIBuffer_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Camera */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Camera"), CTexture::Create(m_pGraphic_Device, g_iWinCX, g_iWinCY))))
		return E_FAIL;



	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Terrain/Grass_%d.tga"), 2))))
	{
		MSGBOX("Failed to create terrain_texture in Loader");
		return E_FAIL;
	}

	/* For.Prototype_Component_Portal_Orange */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Portal_Orange"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Portal_Orange.png")/*129, 129*/))))
		return E_FAIL;

	/* For.Prototype_Component_Portal_Blue */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Portal_Blue"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Portal_Blue.png")/*129, 129*/))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Default */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Default"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Default.jpg")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Player */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Player/AKIHA_AKI00_00%d.png"), 12))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Crosshair"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Crosshair.png")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sky"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../../Resources/Textures/SkyBox/Sky_%d.dds"),4))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BackUI"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Default/BackUI.png")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Continue"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Text/Continue_%d.png"),2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Exit"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Text/Exit_%d.png"), 2))))
		return E_FAIL;

#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT
	/* 원형객체를 생성한다. */


	if (FAILED(pGameInstance->Add_Camera_Prototype(CAM_DYNAMIC, CCamera_Dynamic::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Camera_Prototype(CAM_PORTAL, CCam_Portal::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Camera_Prototype(CAM_PLAYER, CCamera_Player::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Player */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"), CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PortalCtrl"), CPortalControl::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Portal"), CPortal::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"), CSky::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"), CBackGround::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_BackUI */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackUI"), CUI_BackUI::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_BackUI */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Continue"), CButton_Continue::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_BackUI */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Exit"), CButton_Exit::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"), CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma endregion

	RELEASE_INSTANCE(CGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForBoss()
{
	for (_uint i = 0; i < 9999999999; ++i)
		int a = 10;

	m_isFinished = true;
	
	return S_OK;
}

CLoader * CLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CLoader*	pInstance = new CLoader(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(eLevel)))
	{
		MSGBOX("Failed to Creating CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteObject(m_hThread);

	DeleteCriticalSection(&m_CS);

	Safe_Release(m_pGraphic_Device);


}

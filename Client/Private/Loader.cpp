#include "stdafx.h"
#include "Loader.h"

#include "Arm.h"
#include "AttackRange.h"
#include "GameInstance.h"

#include "Player.h"
#include "Portal.h"
#include "Ball.h"
#include "BazierBullet.h"
#include "Boss.h"
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
#include "Gun.h"
#include "Effect.h"
#include "Door_left.h"
#include "Door_Right.h"
#include "Tile.h"
#include "Tile_Cube.h"
#include "Water.h"
#include "CubeMonster.h"
#include "Effect_Alert.h"
#include "Impact.h"
#include "Tile_Collider.h"
#include "CubeBullet.h"
#include "ChangeLevel.h"
#include "CompanionCube.h"
#include "Dispenser.h"
#include "Laser.h"

#include "Level.h"
#include "Level_StageOne.h"
#include "Minimy.h"
#include "Missile.h"
#include "Targeting.h"
#include "Unportal.h"
#include "Shield.h"
#include "Shield_Effect.h"
#include "Sunflower.h"


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
	case LEVEL_STAGEONE:
		pLoader->Loading_ForStageOne();
		break;
	case LEVEL_STAGETWO:
		pLoader->Loading_ForStageTwo();
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

HRESULT CLoader::Loading_ForStageOne()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* 원형컴포넌트를 생성한다. */
#pragma region PROTOTYPE_COMPONENT

	/* For.Prototype_Component_Portal_Orange */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Portal_Orange"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Portal_Orange.png")/*129, 129*/))))
		return E_FAIL;

	/* For.Prototype_Component_Portal_Blue */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Portal_Blue"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Portal_Blue.png")/*129, 129*/))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Crosshair"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Crosshair.png")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BackUI"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Default/BackUI.png")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Continue"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Text/Continue_%d.png"),2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Exit"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Text/Exit_%d.png"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Ball"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Enemy/Enemy_Att/%d.png"), 27))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Portal_Orange_UI"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Gun UI/Portal_Orange_%d.png"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Portal_Blue_UI"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Gun UI/Portal_Blue_%d.png"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Portal_Gun_UI"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Player/Portal_Gun.png")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Gun_Reload"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Gun/Reload/Reload_%d.png"),76))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Gun_Muzzle"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Gun/Effect/MuzzleFlash_%d.png"), 4))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Gun_BulletHole"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Gun/Effect/bulletHoles_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Tile*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tile"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Terrain/Block_%d.png"), 8))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_Water*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Water"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../../Resources/Textures/Water/Water_%d.dds"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Tile_Cube*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Block"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../../Resources/Textures/Block/Block_%d.dds"), 5))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Tile_Cube*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CubeMonster"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../../Resources/Textures/Portal/Enemy/Enemy.dds")))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_Tile_Cube*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC , TEXT("Prototype_Component_Texture_Alert"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Enemy/Alert.png")))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_Door_left*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Door_Left"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../../Resources/Textures/Door/Portal_door_left_%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Door_right*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Door_Right"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../../Resources/Textures/Door/Portal_door_right_%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Door_right*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ChangeLevel"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../../Resources/Textures/Portal/Heal.dds")))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_Door_right*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CubeAttack"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../../Resources/Textures/Portal/Enemy/Enemy_Att.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Font*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Font"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Number/%d.png"), 10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Font_Slash*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Slash"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Number/Slash.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Font_Slash*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UnPortal"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../../Resources/Textures/Block/UnPortal.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Font_Slash*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Impact"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Impact.png")))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Cube"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../../Resources/Textures/Portal/Player/Cube.dds")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dispenser"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../../Resources/Textures/Portal/Dispenser.dds")))))
		return E_FAIL;


#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT
	/* 원형객체를 생성한다. */


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
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Gun"), CGun::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Tile */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tile"), CTile::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Left_Door*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Left_Door"), CDoor_left::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Right_Door*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Right_Door"), CDoor_right::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Water*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Water"), CWater::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Tile_Cube*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Block"), CTile_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CubeMonster"), CCubeMonster::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TileCollider"), CTileCollider::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Impact"), CImpact::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Alert"), CEffect_Alert::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ball"), CBall::Create(m_pGraphic_Device))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CubeBullet"), CCubeBullet::Create(m_pGraphic_Device))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChangeLevel"), CChangeLevel::Create(m_pGraphic_Device))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UnPortal"), CUnportal::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Dispenser"), CDispenser::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma endregion

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_PlayerUI"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Player/ItemHp.png")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Gunicon"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Player/bullet.png")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_PlayerHit"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Player/BloodScreen_alpha_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CompanionCube"), CCompanionCube::Create(m_pGraphic_Device))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	m_isFinished = true;

	return S_OK;
}


HRESULT CLoader::Loading_ForStageTwo()
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);

	// ** Component **

	if (FAILED(p_instance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../../Resources/Textures/Portal/Enemy/Boss_%d.dds"),6))))
		return E_FAIL;

	if (FAILED(p_instance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Arm"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../../Resources/Textures/Portal/Enemy/Boss_Hand_%d.dds"), 4))))
		return E_FAIL;

	if (FAILED(p_instance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_AttackRange"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Enemy/Range.png")))))
		return E_FAIL;

	if (FAILED(p_instance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Missile"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../../Resources/Textures/Portal/Enemy/Boss_Missile.dds")))))
		return E_FAIL;

	if (FAILED(p_instance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Shield"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../../Resources/Textures/Portal/Enemy/Boss_Shield.dds")))))
		return E_FAIL;

	if (FAILED(p_instance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Targeting"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Effect/Tageting_%d.png"), 13))))
		return E_FAIL;

	if (FAILED(p_instance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Shield_Effect"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../../Resources/Textures/Portal/Enemy/Boss_Shield_Effect.dds")))))
		return E_FAIL;

	if (FAILED(p_instance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Minimy"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../../Resources/Textures/Portal/Enemy/Minimy.dds")))))
		return E_FAIL;

	if (FAILED(p_instance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Laser"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../../Resources/Textures/Portal/Enemy/Boss_Laser.dds")))))
		return E_FAIL;

	if (FAILED(p_instance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sunflower"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../../Resources/Textures/Portal/Enemy/Boss_Sunflower.dds")))))
		return E_FAIL;


	if (FAILED(p_instance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_BossHP_Black"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Enemy/Back_HP.png")))))
		return E_FAIL;

	if (FAILED(p_instance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_BossHPBar"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Enemy/Boss_HPBar_%d.png"),2))))
		return E_FAIL;


	if (FAILED(p_instance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_BossHP"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Enemy/Boss_Hp/Boss_Hp_%d.png"), 68))))
		return E_FAIL;

	if (FAILED(p_instance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_BossShield"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Enemy/Boss_Shield/Boss_Shield_%d.png"), 68))))
		return E_FAIL;

	/////////////////

	////////////

	// ** GameObject **

	if (FAILED(p_instance->Add_Prototype(TEXT("Prototype_GameObject_Boss"), CBoss::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(p_instance->Add_Prototype(TEXT("Prototype_GameObject_Arm"), CArm::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(p_instance->Add_Prototype(TEXT("Prototype_GameObject_Missile"), CMissile::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype Game Object_AttackRange*/
	if (FAILED(p_instance->Add_Prototype(TEXT("Prototype_GameObject_AttackRange"), CAttackRange::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(p_instance->Add_Prototype(TEXT("Prototype_GameObject_Targeting"), CTargeting::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(p_instance->Add_Prototype(TEXT("Prototype_GameObject_Shield"), CShield::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(p_instance->Add_Prototype(TEXT("Prototype_GameObject_Shield_Effect"), CShield_Effect::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(p_instance->Add_Prototype(TEXT("Prototype_GameObject_Minimy"), CMinimy::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(p_instance->Add_Prototype(TEXT("Prototype_GameObject_Laser"), CLaser::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(p_instance->Add_Prototype(TEXT("Prototype_GameObject_Sunflower"), CSunflower::Create(m_pGraphic_Device))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

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

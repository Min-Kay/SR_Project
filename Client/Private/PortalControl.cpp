#include "stdafx.h"
#include "PortalControl.h"
#include "Portal.h"
#include "GameInstance.h"
#include "Cam_Portal.h"
#include "Level_GamePlay.h"

CPortalControl* CPortalControl::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	CPortalControl* pInstance = new CPortalControl(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
    return pInstance;
}

CGameObject* CPortalControl::Clone(void* pArg)
{
	CPortalControl* pInstance = new CPortalControl(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CPortalControl::Free()
{
	Safe_Release(m_pPortal_Orange);
	Safe_Release(m_pPortal_Blue);
	Safe_Release(m_pPlayerTransform);
	__super::Free();
}

CPortalControl::CPortalControl(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CPortalControl::CPortalControl(const CPortalControl& rhs)
	:CGameObject(rhs)
{
}

HRESULT CPortalControl::NativeConstruct_Prototype()
{
    return S_OK;
}

HRESULT CPortalControl::NativeConstruct(void* pArg)
{

	if (FAILED(SetUp_UI()))
		return E_FAIL;

    return S_OK;
}

_int CPortalControl::Tick(_float fTimeDelta)
{
	if (!m_Vaild)
		return 0;

	Animate_Gun(fTimeDelta);

    return _int();
}

_int CPortalControl::LateTick(_float fTimeDelta)
{
	if (!m_Vaild)
		return 0;

	if (nullptr != m_pPortal_Orange && nullptr != m_pPortal_Blue)
	{
		if(nullptr == m_pPortal_Orange->Get_Link_Portal())
			m_pPortal_Orange->Link_Portal(m_pPortal_Blue);
		if (nullptr == m_pPortal_Blue->Get_Link_Portal())
			m_pPortal_Blue->Link_Portal(m_pPortal_Orange);

		if (nullptr != m_pPlayerTransform)
		{
			if (nullptr != m_pPortal_Orange->Get_Link_Portal() && nullptr != m_pPortal_Blue->Get_Link_Portal())
			{
				m_pPortal_Blue->Set_Cam_Angle(m_pPlayerTransform);
				m_pPortal_Orange->Set_Cam_Angle(m_pPlayerTransform);
			}
		}
	}

    return _int();
}

HRESULT CPortalControl::Render()
{
	if (!m_Vaild)
		return S_OK;

    return S_OK;
}

HRESULT CPortalControl::Spawn_Portal(_uint iLevelIndex, CTransform* _tr, PortalColor iIndex)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (iIndex == PORTAL_ORANGE)
	{
		if (nullptr != m_pPortal_Orange)
		{
			if (nullptr != m_pPortal_Blue)
			{
				m_pPortal_Blue->Link_Portal(nullptr);
			}
			m_pPortal_Orange->Link_Portal(nullptr);

			pGameInstance->Release_GameObject(iLevelIndex, TEXT("Portal_Orange"), m_pPortal_Orange);
			m_pPortal_Orange = nullptr;
		}

		CPortal::PORTALDESC portalDesc;
		portalDesc.iLevel = iLevelIndex;
		portalDesc.iPortalColor = 0;
		portalDesc.vAxisY = _float3(0.f, 1.f, 0.f);

		_float3 nor = _tr->Get_State(CTransform::STATE_LOOK);
		D3DXVec3Normalize(&nor,&nor);
		portalDesc.vEye = _tr->Get_State(CTransform::STATE_POSITION) + nor * 2.f;
		portalDesc.vAt = _tr->Get_State(CTransform::STATE_POSITION) + nor * 2.1f;
		portalDesc.portalCam = TEXT("Portal_Orange");

		if (FAILED(pGameInstance->Add_GameObject(iLevelIndex, TEXT("Portal_Orange"), TEXT("Prototype_GameObject_Portal"), &portalDesc)))
		{
			RELEASE_INSTANCE(CGameInstance);
			return E_FAIL;
		}

		m_pPortal_Orange = static_cast<CPortal*>(pGameInstance->Get_GameObject(iLevelIndex, TEXT("Portal_Orange"), 0));

		m_pPortal_Orange_UI->Set_CurrFrameIndex(1);
		pGameInstance->StopSound(CSoundMgr::EFFECT);
		pGameInstance->Play_Sound(TEXT("Portal_Orange_Fire.mp3"), CSoundMgr::EFFECT, 1.f);
	}
	else if (iIndex == PORTAL_BLUE)
	{
		if (nullptr != m_pPortal_Blue)
		{
			if (nullptr != m_pPortal_Orange)
			{
				m_pPortal_Orange->Link_Portal(nullptr);
			}
			m_pPortal_Blue->Link_Portal(nullptr);

			pGameInstance->Release_GameObject(iLevelIndex, TEXT("Portal_Blue"), m_pPortal_Blue);
			m_pPortal_Blue = nullptr;
		}

		CPortal::PORTALDESC portalDesc2;
		portalDesc2.iLevel = iLevelIndex;
		portalDesc2.iPortalColor = 1;
		portalDesc2.vAxisY = _float3(0.f, 1.f, 0.f);
		_float3 nor = _tr->Get_State(CTransform::STATE_LOOK);
		D3DXVec3Normalize(&nor, &nor);
		portalDesc2.vEye = _tr->Get_State(CTransform::STATE_POSITION) + nor * 2.f;
		portalDesc2.vAt = _tr->Get_State(CTransform::STATE_POSITION) + nor * 2.1f;

		portalDesc2.portalCam = TEXT("Portal_Blue");

		if (FAILED(pGameInstance->Add_GameObject(iLevelIndex, TEXT("Portal_Blue"), TEXT("Prototype_GameObject_Portal"), &portalDesc2)))
		{
			RELEASE_INSTANCE(CGameInstance);
			return E_FAIL;
		}

		m_pPortal_Blue = static_cast<CPortal*>(pGameInstance->Get_GameObject(iLevelIndex, TEXT("Portal_Blue"), 0));
		m_pPortal_Blue_UI->Set_CurrFrameIndex(1);
		pGameInstance->StopSound(CSoundMgr::EFFECT);
		pGameInstance->Play_Sound(TEXT("Portal_Blue_Fire.mp3"), CSoundMgr::EFFECT, 1.f);

	}
	RELEASE_INSTANCE(CGameInstance);
   
    return S_OK;
}

HRESULT CPortalControl::Erase_Portal(_uint iLevelIndex)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (nullptr != m_pPortal_Blue)
	{
		if (nullptr != m_pPortal_Orange)
		{
			m_pPortal_Orange->Link_Portal(nullptr);
		}
		m_pPortal_Blue->Link_Portal(nullptr);

		pGameInstance->Release_GameObject(iLevelIndex, TEXT("Portal_Blue"), m_pPortal_Blue);
		m_pPortal_Blue = nullptr;
	}

	if (nullptr != m_pPortal_Orange)
	{
		if (nullptr != m_pPortal_Blue)
		{
			m_pPortal_Blue->Link_Portal(nullptr);
		}
		m_pPortal_Orange->Link_Portal(nullptr);

		pGameInstance->Release_GameObject(iLevelIndex, TEXT("Portal_Orange"), m_pPortal_Orange);
		m_pPortal_Orange = nullptr;
	}

	m_pPortal_Blue_UI->Set_CurrFrameIndex(0);
	m_pPortal_Orange_UI->Set_CurrFrameIndex(0);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CPortalControl::Set_Player(CTransform* pPlayer)
{
	m_pPlayerTransform = pPlayer;	
	Safe_AddRef(m_pPlayerTransform);
}

HRESULT CPortalControl::SetUp_UI()
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	CUI::UIDESC desc, desc2, desc3;
	ZeroMemory(&desc, sizeof(desc));
	ZeroMemory(&desc2, sizeof(desc2));
	ZeroMemory(&desc3, sizeof(desc3));

	desc.WinCX = g_iWinCX;
	desc.WinCY = g_iWinCY;
	desc.Layer = 2;
	desc.FrameCount = 2;
	desc.Alpha = CUI::ALPHA_BLEND;
	desc.PosX = g_iWinCX * 0.47f;
	desc.PosY = g_iWinCY * 0.46f;
	desc.SizeX = 70.f;
	desc.SizeY = 70.f;
	desc.Style = CUI::STYLE_FIX;
	desc.Texture = TEXT("Prototype_Component_Texture_Portal_Orange_UI");


	if (FAILED(p_instance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Portal_Orange_UI"), PROTO_UI, &desc)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_pPortal_Orange_UI = static_cast<CUI*>(p_instance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Portal_Orange_UI")));

	desc2.WinCX = g_iWinCX;
	desc2.WinCY = g_iWinCY;
	desc2.Layer = 2;
	desc2.FrameCount = 2;
	desc2.Alpha = CUI::ALPHA_BLEND;
	desc2.PosX = g_iWinCX * 0.53f;
	desc2.PosY = g_iWinCY * 0.54f;
	desc2.SizeX = 70.f;
	desc2.SizeY = 70.f;
	desc2.Style = CUI::STYLE_FIX;
	desc2.Texture = TEXT("Prototype_Component_Texture_Portal_Blue_UI");


	if (FAILED(p_instance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Portal_Blue_UI"), PROTO_UI, &desc2)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_pPortal_Blue_UI = static_cast<CUI*>(p_instance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Portal_Blue_UI")));


	desc3.WinCX = g_iWinCX;
	desc3.WinCY = g_iWinCY;
		
	desc3.Layer = 2;
	desc3.Alpha = CUI::ALPHA_BLEND;
	desc3.PosX = g_iWinCX * 0.90f;
	desc3.PosY = g_iWinCY * 0.82f;
	desc3.SizeX = 500.f;
	desc3.SizeY = 350.f;
	desc3.Style = CUI::STYLE_FIX;
	desc3.Texture = TEXT("Prototype_Component_Texture_Portal_Gun_UI");


	m_fGun_fx = desc3.PosX;
	m_fGun_fy = desc3.PosY;


	if (FAILED(p_instance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Portal_Gun_UI"), PROTO_UI, &desc3)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_pPortal_Gun_UI = static_cast<CUI*>(p_instance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Portal_Gun_UI")));

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CPortalControl::Animate_Gun(_float fTimeDelta)
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	if(p_instance->Get_Key_Press(DIK_W) || p_instance->Get_Key_Press(DIK_D) || p_instance->Get_Key_Press(DIK_A) || p_instance->Get_Key_Press(DIK_S))
	{
		m_fFrWalk += fTimeDelta * 400.f; 
	}
	else
	{
		m_fFrWalk -= fTimeDelta * 400.f;
	}

	if (m_fFrWalk >= 180.f || m_fFrWalk <= 0.f)
		m_fFrWalk = 0.f;

	if (p_instance->Get_Mouse_Down(CInput_Device::MBS_RBUTTON) || p_instance->Get_Mouse_Down(CInput_Device::MBS_LBUTTON))
	{
		m_fFrShoot += fTimeDelta * 700.f;
	}
	else
	{
		m_fFrShoot -= fTimeDelta * 200.f;
	}

	if (m_fFrShoot >= 180.f || m_fFrShoot <= 0.f)
		m_fFrShoot = 0.f;


	m_pPortal_Gun_UI->Set_Pos(m_fGun_fx + sinf(D3DXToRadian(m_fFrShoot)) * m_fGun_fx * 0.3f, m_fGun_fy + sinf(D3DXToRadian(m_fFrShoot)) * m_fGun_fy * 0.3f + sinf(D3DXToRadian(m_fFrWalk)) * m_fGun_fy * 0.1f);

	RELEASE_INSTANCE(CGameInstance);
}

void CPortalControl::Set_Vaild(_bool _bool)
{
	m_Vaild = _bool;
	m_pPortal_Orange_UI->Set_Vaild(m_Vaild);
	m_pPortal_Blue_UI->Set_Vaild(m_Vaild);
	m_pPortal_Gun_UI->Set_Vaild(m_Vaild);
}

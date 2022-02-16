#include "stdafx.h"
#include "PortalControl.h"
#include "Portal.h"
#include "GameInstance.h"
#include "Cam_Portal.h"

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
    return S_OK;
}

_int CPortalControl::Tick(_float fTimeDelta)
{
    return _int();
}

_int CPortalControl::LateTick(_float fTimeDelta)
{
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
				m_pPortal_Blue->Get_Cam_Portal()->Set_Cam_Angle(m_pPlayerTransform);
				m_pPortal_Orange->Get_Cam_Portal()->Set_Cam_Angle(m_pPlayerTransform);
			}
		}
	}

    return _int();
}

HRESULT CPortalControl::Render()
{
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

			pGameInstance->Release_GameObject(LEVEL_GAMEPLAY, TEXT("Portal_Orange"), m_pPortal_Orange);
			m_pPortal_Orange = nullptr;
		}

		CPortal::PORTALDESC portalDesc;
		portalDesc.iLevel = iLevelIndex;
		portalDesc.iPortalColor = 0;
		portalDesc.vAxisY = _float3(0.f, 1.f, 0.f);
		portalDesc.vEye = _tr->Get_State(CTransform::STATE_POSITION) + _tr->Get_State(CTransform::STATE_LOOK) * 2.f;
		portalDesc.vAt = _tr->Get_State(CTransform::STATE_POSITION) + _tr->Get_State(CTransform::STATE_LOOK) * 3.f;
		portalDesc.portalCam = TEXT("Portal_Orange");

		if (FAILED(pGameInstance->Add_GameObject(iLevelIndex, TEXT("Portal_Orange"), TEXT("Prototype_GameObject_Portal"), &portalDesc)))
		{
			RELEASE_INSTANCE(CGameInstance);
			return E_FAIL;
		}

		m_pPortal_Orange = static_cast<CPortal*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Portal_Orange"), 0));
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

			pGameInstance->Release_GameObject(LEVEL_GAMEPLAY, TEXT("Portal_Blue"), m_pPortal_Blue);
			m_pPortal_Blue = nullptr;
		}

		CPortal::PORTALDESC portalDesc2;
		portalDesc2.iLevel = iLevelIndex;
		portalDesc2.iPortalColor = 1;
		portalDesc2.vAxisY = _float3(0.f, 1.f, 0.f);
		portalDesc2.vEye = _tr->Get_State(CTransform::STATE_POSITION) + _tr->Get_State(CTransform::STATE_LOOK) * 2.f;
		portalDesc2.vAt = _tr->Get_State(CTransform::STATE_POSITION) + _tr->Get_State(CTransform::STATE_LOOK) * 3.f;

		portalDesc2.portalCam = TEXT("Portal_Blue");

		if (FAILED(pGameInstance->Add_GameObject(iLevelIndex, TEXT("Portal_Blue"), TEXT("Prototype_GameObject_Portal"), &portalDesc2)))
		{
			RELEASE_INSTANCE(CGameInstance);
			return E_FAIL;
		}

		m_pPortal_Blue = static_cast<CPortal*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Portal_Blue"), 0));
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

		pGameInstance->Release_GameObject(LEVEL_GAMEPLAY, TEXT("Portal_Blue"), m_pPortal_Blue);
		m_pPortal_Blue = nullptr;
	}

	if (nullptr != m_pPortal_Orange)
	{
		if (nullptr != m_pPortal_Blue)
		{
			m_pPortal_Blue->Link_Portal(nullptr);
		}
		m_pPortal_Orange->Link_Portal(nullptr);

		pGameInstance->Release_GameObject(LEVEL_GAMEPLAY, TEXT("Portal_Orange"), m_pPortal_Orange);
		m_pPortal_Orange = nullptr;
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CPortalControl::Set_Player(CTransform* pPlayer)
{
	m_pPlayerTransform = pPlayer;	
	Safe_AddRef(m_pPlayerTransform);
}

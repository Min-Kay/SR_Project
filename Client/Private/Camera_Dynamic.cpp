#include "stdafx.h"
#include "..\Public\Camera_Dynamic.h"
#include "GameInstance.h"
#include "PortalControl.h"

CCamera_Dynamic::CCamera_Dynamic(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{
}

CCamera_Dynamic::CCamera_Dynamic(const CCamera_Dynamic & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Dynamic::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Dynamic::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CCamera_Dynamic::Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	if (pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
	{
		m_pTransform->Go_Straight(fTimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
	{
		m_pTransform->Go_BackWard(fTimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
	{
		m_pTransform->Go_Left(fTimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
	{
		m_pTransform->Go_Right(fTimeDelta);
	}

	if (nullptr == portalCtl)
	{
		if (pGameInstance->Get_DIKeyState(DIK_I) & 0x80)
		{
			if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("PortalCtrl"), TEXT("Prototype_GameObject_PortalCtrl"))))
				return E_FAIL;

			portalCtl = static_cast<CPortalControl*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("PortalCtrl"), 0));
			portalCtl->Set_Player(m_pTransform);
			Safe_AddRef(portalCtl);
		}
	}

	if (nullptr != portalCtl)
	{
		if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & 0x80)
		{
			portalCtl->Spawn_Portal(LEVEL_GAMEPLAY,m_pTransform, CPortalControl::PORTAL_ORANGE);
		}

		if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON) & 0x80)
		{
			portalCtl->Spawn_Portal(LEVEL_GAMEPLAY, m_pTransform, CPortalControl::PORTAL_BLUE);
		}

		if (pGameInstance->Get_DIKeyState(DIK_SPACE) & 0x80)
		{
			portalCtl->Erase_Portal(LEVEL_GAMEPLAY);
		}
	}

	_long		MouseMove = 0;

	if (MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_X))
	{
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * MouseMove * 0.1f);
	}

	if (MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_Y))
	{
		m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * 0.1f);
	}

	RELEASE_INSTANCE(CGameInstance);

	return __super::Tick(fTimeDelta);		
}

_int CCamera_Dynamic::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	

	return _int();
}

HRESULT CCamera_Dynamic::Render()
{
	return S_OK;
}

HRESULT CCamera_Dynamic::BeforeRender()
{
	m_pGraphic_Device->Clear(0,
		nullptr,
		D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_ARGB(255, 0, 0, 255),	// 백버퍼 색상
		1.f, // z버퍼의 초기화 값
		0);	 // 스텐실 버퍼의 초기화 값

	if(FAILED(__super::BeforeRender()))
		return E_FAIL;

	if (FAILED(__super::Use_Pick(1)))
		return E_FAIL;
	
	return S_OK;

}

HRESULT CCamera_Dynamic::AfterRender()
{
	return __super::AfterRender();
}

CCamera_Dynamic * CCamera_Dynamic::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Dynamic*	pInstance = new CCamera_Dynamic(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CCamera_Dynamic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Dynamic::Clone(void * pArg)
{
	CCamera_Dynamic*	pInstance = new CCamera_Dynamic(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CCamera_Dynamic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Dynamic::Free()
{
	__super::Free();
	Safe_Release(portalCtl);
}

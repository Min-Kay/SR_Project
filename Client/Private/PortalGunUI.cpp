#include "stdafx.h"
#include "PortalGunUI.h"

CPortalGunUI::CPortalGunUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CPortalGunUI::CPortalGunUI(const CPortalGunUI& rhs)
	:CUI(rhs)
{
}

HRESULT CPortalGunUI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPortalGunUI::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CPortalGunUI::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
			return -1;

	return 0;
}

_int CPortalGunUI::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
	return -1;

	return 0;
}

HRESULT CPortalGunUI::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPortalGunUI::SetUp_Components(const _tchar* _texture)
{
	if (FAILED(__super::SetUp_Components(_texture)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPortalGunUI::Tick_UI(_float fTimeDelta)
{
	if (FAILED(__super::Tick_UI(fTimeDelta)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPortalGunUI::Set_RenderState()
{
	if (FAILED(__super::Set_RenderState()))
		return E_FAIL;

	return S_OK;
}

CPortalGunUI* CPortalGunUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPortalGunUI* portal_gun_ui = new CPortalGunUI(pGraphic_Device);

	if(FAILED(portal_gun_ui->NativeConstruct_Prototype()))
	{
		Safe_Release(portal_gun_ui);
		return nullptr;
	}
	return portal_gun_ui;
}

CGameObject* CPortalGunUI::Clone(void* pArg)
{
	CPortalGunUI* portal_gun_ui = new CPortalGunUI(*this);

	if (FAILED(portal_gun_ui->NativeConstruct(pArg)))
	{
		Safe_Release(portal_gun_ui);
		return nullptr;
	}
	return portal_gun_ui;
}

void CPortalGunUI::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "Button_Continue.h"
#include "UI_BackUI.h"
#include "GameInstance.h"
CButton_Continue::CButton_Continue(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CButton(pGraphic_Device)
{
}

CButton_Continue::CButton_Continue(const CButton_Continue& rhs)
	:CButton(rhs)
{
}

HRESULT CButton_Continue::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	return S_OK;

}

HRESULT CButton_Continue::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;
	return S_OK;

}

_int CButton_Continue::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;
	return 0;

}

_int CButton_Continue::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;
	return 0;

}

HRESULT CButton_Continue::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;

}

HRESULT CButton_Continue::SetUp_Components()
{
	if (FAILED(__super::SetUp_Components()))
		return E_FAIL;
	return S_OK;

}

HRESULT CButton_Continue::Tick_UI(_float fTimeDelta)
{
	if(FAILED(__super::Tick_UI(fTimeDelta)))
		return E_FAIL;
	return S_OK;

}

HRESULT CButton_Continue::Set_RenderState()
{
	if (FAILED(__super::Set_RenderState()))
		return E_FAIL;
	return S_OK;

}

HRESULT CButton_Continue::OnClick_Action()
{
	static_cast<CUI_BackUI*>(m_Parent)->Continue_Game();
	return S_OK;
}

void CButton_Continue::OnEnter_Overlay()
{
	Set_CurrFrameIndex(1);
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->Play_Sound(TEXT("Button_Overlay.mp3"), CSoundMgr::UI, 1.0f);
	RELEASE_INSTANCE(CGameInstance);
}

void CButton_Continue::OnEnter_UnOverlay()
{
	Set_CurrFrameIndex(0);
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->StopSound(CSoundMgr::UI);
	RELEASE_INSTANCE(CGameInstance);
}

CButton_Continue* CButton_Continue::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CButton_Continue* p_exit = new CButton_Continue(pGraphic_Device);
	if (FAILED(p_exit->NativeConstruct_Prototype()))
	{
		Safe_Release(p_exit);
		return nullptr;
	}
	return p_exit;
}

CGameObject* CButton_Continue::Clone(void* pArg)
{
	CButton_Continue* p_exit = new CButton_Continue(*this);
	if (FAILED(p_exit->NativeConstruct(pArg)))
	{
		Safe_Release(p_exit);
		return nullptr;
	}
	return p_exit;
}

void CButton_Continue::Free()
{
	__super::Free();
}

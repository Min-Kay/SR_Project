#include "stdafx.h"
#include "Button_Exit.h"
#include "UI_BackUI.h"
#include "GameInstance.h"

CButton_Exit::CButton_Exit(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CButton(pGraphic_Device)
{
}

CButton_Exit::CButton_Exit(const CButton_Exit& rhs)
	:CButton(rhs)
{
}

HRESULT CButton_Exit::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	return S_OK;

}

HRESULT CButton_Exit::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;
	return S_OK;

}

_int CButton_Exit::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;
	return 0;
}

_int CButton_Exit::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;
	return 0;

}

HRESULT CButton_Exit::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;
}

HRESULT CButton_Exit::SetUp_Components()
{
	if (FAILED(__super::SetUp_Components()))
		return E_FAIL;
	return S_OK;

}

HRESULT CButton_Exit::Tick_UI(_float fTimeDelta)
{
	if (FAILED(__super::Tick_UI(fTimeDelta)))
		return E_FAIL;
	return S_OK;

}

HRESULT CButton_Exit::Set_RenderState()
{
	if (FAILED(__super::Set_RenderState()))
		return E_FAIL;
	return S_OK;

}

HRESULT CButton_Exit::OnClick_Action()
{
	static_cast<CUI_BackUI*>(m_Parent)->Exit_Game();
	return S_OK;
}

void CButton_Exit::OnEnter_Overlay()
{
	Set_CurrFrameIndex(1);
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->Play_Sound(TEXT("Button_Overlay.mp3"), CSoundMgr::UI, 1.0f);
	RELEASE_INSTANCE(CGameInstance);
}

void CButton_Exit::OnEnter_UnOverlay()
{
	Set_CurrFrameIndex(0);
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->StopSound(CSoundMgr::UI);
	RELEASE_INSTANCE(CGameInstance);
}

CButton_Exit* CButton_Exit::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CButton_Exit* p_exit = new CButton_Exit(pGraphic_Device);
	if(FAILED(p_exit->NativeConstruct_Prototype()))
	{
		Safe_Release(p_exit);
		return nullptr;
	}
	return p_exit; 
}

CGameObject* CButton_Exit::Clone(void* pArg)
{
	CButton_Exit* p_exit = new CButton_Exit(*this);
	if (FAILED(p_exit->NativeConstruct(pArg)))
	{
		Safe_Release(p_exit);
		return nullptr;
	}
	return p_exit;
}

void CButton_Exit::Free()
{
	__super::Free();
}

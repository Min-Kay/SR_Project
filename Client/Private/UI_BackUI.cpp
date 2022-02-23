#include "stdafx.h"
#include "UI_BackUI.h"
#include "Button.h"
#include "GameInstance.h"
#include "Camera_Player.h"

CUI_BackUI::CUI_BackUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CUI_BackUI::CUI_BackUI(const CUI_BackUI& rhs)
	:CUI(rhs)
{
}

HRESULT CUI_BackUI::NativeConstruct_Prototype()
{
	if(FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	return S_OK;

}

HRESULT CUI_BackUI::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);

	m_Continue = static_cast<CButton*>(p_instance->Get_GameObject(LEVEL_STATIC, TEXT("Continue"), 0));
	if(nullptr == m_Continue)
	{
		UIDESC desc3;
		ZeroMemory(&desc3, sizeof(desc3));

		desc3.Layer = 0;
		desc3.WinCX = g_iWinCX;
		desc3.WinCY = g_iWinCY;
		desc3.PosX = g_iWinCX * 0.8f;
		desc3.PosY = g_iWinCY * 0.6f;
		desc3.SizeX = 300;
		desc3.SizeY = 100;
		desc3.Alpha = ALPHA_BLEND;
		desc3.Texture = TEXT("Prototype_Component_Texture_Continue");
		desc3.FrameCount = 2;

		if (FAILED(p_instance->Add_GameObject(LEVEL_STATIC, TEXT("Continue"), TEXT("Prototype_GameObject_Continue"), &desc3)))
			return E_FAIL;

		m_Continue = static_cast<CButton*>(p_instance->Get_GameObject(LEVEL_STATIC, TEXT("Continue"), 0));
	}
	m_Continue->Set_HWND(g_hWnd);
	m_Continue->Set_ParentUI(this);


	m_Exit = static_cast<CButton*>(p_instance->Get_GameObject(LEVEL_STATIC, TEXT("Exit"), 0));

	if (nullptr == p_instance->Get_GameObject(LEVEL_STATIC, TEXT("Exit"), 0))
	{
		UIDESC desc4;
		ZeroMemory(&desc4,sizeof(desc4));
		desc4.Layer = 0;
		desc4.WinCX = g_iWinCX;
		desc4.WinCY = g_iWinCY;
		desc4.PosX = g_iWinCX * 0.8f;
		desc4.PosY = g_iWinCY * 0.8f;
		desc4.SizeX = 300;
		desc4.SizeY = 100;
		desc4.Alpha = ALPHA_BLEND;
		desc4.FrameCount = 2;
		desc4.Texture = TEXT("Prototype_Component_Texture_Exit");

		if (FAILED(p_instance->Add_GameObject(LEVEL_STATIC, TEXT("Exit"), TEXT("Prototype_GameObject_Exit"), &desc4)))
			return E_FAIL;

		m_Exit = static_cast<CButton*>(p_instance->Get_GameObject(LEVEL_STATIC, TEXT("Exit"), 0));
	}
	m_Exit->Set_HWND(g_hWnd);
	m_Exit->Set_ParentUI(this);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CUI_BackUI::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;
	return 0;
}

_int CUI_BackUI::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;
	return 0;
}

HRESULT CUI_BackUI::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;
}

HRESULT CUI_BackUI::SetUp_Components(const _tchar* _texture)
{
	if (FAILED(__super::SetUp_Components(_texture)))
		return E_FAIL;
	return S_OK;
}

void CUI_BackUI::Open_Menu()
{
	__super::Set_Vaild(true);
	m_Continue->Set_Vaild(true);
	m_Exit->Set_Vaild(true);

}

void CUI_BackUI::Off_Menu()
{
	__super::Set_Vaild(false);
	m_Continue->Set_Vaild(false);
	m_Exit->Set_Vaild(false);
}

void CUI_BackUI::Set_Cam(CCamera_Player* _cam)
{
	cam = _cam;
	Safe_AddRef(cam);
}

HRESULT CUI_BackUI::Tick_UI(_float fTimeDelta)
{
	return CUI::Tick_UI(fTimeDelta);
}

HRESULT CUI_BackUI::Set_RenderState()
{
	return CUI::Set_RenderState();
}

CUI_BackUI* Client::CUI_BackUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_BackUI* p_ui = new CUI_BackUI(pGraphic_Device);

	if(FAILED(p_ui->NativeConstruct_Prototype()))
	{
		Safe_Release(p_ui);
		return nullptr;
	}
	return p_ui;
}

CGameObject* CUI_BackUI::Clone(void* pArg)
{
	CUI_BackUI* p_ui = new CUI_BackUI(*this);

	if (FAILED(p_ui->NativeConstruct(pArg)))
	{
		Safe_Release(p_ui);
		return nullptr;
	}
	return p_ui;
}

void CUI_BackUI::Free()
{
	__super::Free();
	Safe_Release(cam);
}

void CUI_BackUI::Continue_Game()
{
	cam->Control_Menu(false);
}

void CUI_BackUI::Exit_Game()
{
	SendMessage(g_hWnd, WM_CLOSE, 0, 0);
}

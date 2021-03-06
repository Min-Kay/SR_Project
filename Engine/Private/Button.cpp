#include "Button.h"
#include "GameInstance.h"

CButton::CButton(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CButton::CButton(const CButton& rhs)
	: CUI(rhs)
{
}

HRESULT CButton::NativeConstruct_Prototype()
{
	if(FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CButton::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	m_Rect.left = (_long)(m_desc.PosX - m_desc.SizeX * 0.5f);
	m_Rect.top = (_long)(m_desc.PosY - m_desc.SizeY * 0.5f);
	m_Rect.right = (_long)(m_desc.PosX + m_desc.SizeX * 0.5f);
	m_Rect.bottom = (_long)(m_desc.PosY + m_desc.SizeY * 0.5f);

	return S_OK;

}

_int CButton::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	if (!m_Vaild)
		return 0;

	OnClick();
	return 0;
}

_int CButton::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;
	return 0;
}

HRESULT CButton::Render()
{
	if(FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;
}

HRESULT CButton::SetUp_Components()
{
	if (FAILED(__super::SetUp_Components()))
		return E_FAIL;
	return S_OK;
}

HRESULT CButton::Tick_UI(_float fTimeDelta)
{
	if(FAILED(__super::Tick_UI(fTimeDelta)))
		return E_FAIL;
	return S_OK;
}

HRESULT CButton::Set_RenderState()
{
	if (FAILED(__super::Set_RenderState()))
		return E_FAIL;
	return S_OK;
}

HRESULT CButton::OnClick()
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	if (OnEnter() && p_instance->Get_Mouse_Down(CInput_Device::MBS_LBUTTON))
		if(FAILED(OnClick_Action()))
			return E_FAIL;
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

_bool CButton::OnEnter()
{
	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(m_Hwnd, &ptMouse);

	if (PtInRect(&m_Rect, ptMouse))
	{
		if(!isOverlayed)
			OnEnter_Overlay();
		isOverlayed = true;
		return true;
	}

	if (isOverlayed)
	{
		isOverlayed = false; 
		OnEnter_UnOverlay();
	}
	return false;

}

void CButton::Free()
{
	__super::Free();
}

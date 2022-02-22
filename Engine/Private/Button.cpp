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
}

HRESULT CButton::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	m_Rect.left = (_long)(m_fX - m_fSizeX * 0.5f);
	m_Rect.top = (_long)(m_fY - m_fSizeY * 0.5f);
	m_Rect.right = (_long)(m_fX + m_fSizeX * 0.5f);
	m_Rect.bottom = (_long)(m_fY + m_fSizeY * 0.5f);
}

_int CButton::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	OnClick();
}

_int CButton::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;
}

HRESULT CButton::Render()
{
	if(FAILED(__super::Render()))
		return E_FAIL;
}

HRESULT CButton::SetUp_Components(const _tchar* _texture)
{
	if (FAILED(__super::SetUp_Components(_texture)))
		return E_FAIL;
}

HRESULT CButton::Tick_UI(_float fTimeDelta)
{
	if(FAILED(__super::Tick_UI(fTimeDelta)))
		return E_FAIL;
}

HRESULT CButton::Set_RenderState()
{
	if (FAILED(__super::Set_RenderState()))
		return E_FAIL;
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
		OnEnter_Overlay();
		return true;
	}
	else
	{
		OnEnter_UnOverlay();
		return false;
	}
}

void CButton::Free()
{
	__super::Free();
	Safe_Release(m_Parent);
}

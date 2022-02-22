#include "..\public\Input_Device.h"

IMPLEMENT_SINGLETON(CInput_Device)

CInput_Device::CInput_Device()
{
	ZeroMemory(&m_byFormalState,sizeof(m_byFormalState));
	ZeroMemory(&m_MouseFormalState, sizeof(m_MouseFormalState));

}

_bool CInput_Device::Get_Key_Press(_ubyte eKeyID)
{
	_byte currState = Get_DIKeyState(eKeyID);
	if (currState & 0x80)
	{
		m_byFormalState[eKeyID] = currState;
		return true;
	}
	return false;
}

_bool CInput_Device::Get_Key_Down(_ubyte eKeyID)
{
	_byte currState = Get_DIKeyState(eKeyID);
	if ((currState & 0x80) && currState != m_byFormalState[eKeyID])
	{
		m_byFormalState[eKeyID] = currState;
		return true;
	}
	return false;
}

_bool CInput_Device::Get_Key_Up(_ubyte eKeyID)
{
	_byte currState = Get_DIKeyState(eKeyID);
	if (!(currState & 0x80) && currState != m_byFormalState[eKeyID])
	{
		m_byFormalState[eKeyID] = currState;
		return true;
	}
	return false;
}

_bool CInput_Device::Get_Mouse_Button_Down(MOUSEBUTTONSTATE eButtonID)
{
	_byte currState = Get_DIMouseButtonState(eButtonID);
	if ((currState & 0x80) && currState != m_MouseFormalState[eButtonID])
	{
		m_MouseFormalState[eButtonID] = currState;
		return true;
	}
	return false;
}

_bool CInput_Device::Get_Mouse_Button_Up(MOUSEBUTTONSTATE eButtonID)
{
	_byte currState = Get_DIMouseButtonState(eButtonID);
	if (!(currState & 0x80) && currState != m_MouseFormalState[eButtonID])
	{
		m_MouseFormalState[eButtonID] = currState;
		return true;
	}
	return false;
}

_bool CInput_Device::Get_Mouse_Button_Press(MOUSEBUTTONSTATE eButtonID)
{
	_byte currState = Get_DIMouseButtonState(eButtonID);
	if (currState & 0x80)
	{
		m_MouseFormalState[eButtonID] = currState;
		return true;
	}
	return false;
}

HRESULT CInput_Device::Tick_KeyState()
{
	memcpy(&m_byFormalState,&m_byKeyState,sizeof(m_byFormalState));
	memcpy(&m_MouseFormalState, &(m_MouseState.rgbButtons), sizeof(m_MouseFormalState));

	return S_OK;
}

HRESULT CInput_Device::Ready_Input_Device(HINSTANCE hInst, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, nullptr)))
		return E_FAIL;

	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, nullptr)))
		return E_FAIL;

	if (FAILED(m_pKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return E_FAIL;

	if (FAILED(m_pKeyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return E_FAIL;

	if (FAILED(m_pKeyboard->Acquire()))
		return E_FAIL;

	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return E_FAIL;

	if (FAILED(m_pMouse->Acquire()))
		return E_FAIL;

	return S_OK;
}

HRESULT CInput_Device::SetUp_InputDeviceState()
{
	if (nullptr == m_pKeyboard ||
		nullptr == m_pMouse)
		return E_FAIL;

	m_pKeyboard->GetDeviceState(256, m_byKeyState);

	m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);

	return S_OK;
}

void CInput_Device::Free()
{
	Safe_Release(m_pMouse);
	Safe_Release(m_pKeyboard);
	Safe_Release(m_pInput);
}

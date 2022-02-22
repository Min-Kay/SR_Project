#pragma once

#include "Base.h"

BEGIN(Engine)

class CInput_Device final : public CBase
{
	DECLARE_SINGLETON(CInput_Device)

public:
	enum MOUSEMOVESTATE { MMS_X, MMS_Y, MMS_WHEEL, MMS_END };
	enum MOUSEBUTTONSTATE { MBS_LBUTTON, MBS_RBUTTON, MBS_WHEEL, MBS_END };

private:
	CInput_Device();
	virtual ~CInput_Device() = default;

private:
	_byte Get_DIKeyState(_ubyte eKeyID) {
		return m_byKeyState[eKeyID];
	}

	_byte Get_DIMouseButtonState(MOUSEBUTTONSTATE eMouseButtonState) {
		return m_MouseState.rgbButtons[eMouseButtonState];
	}

public:
	_long Get_DIMouseMoveState(MOUSEMOVESTATE eMouseMoveState) {
		return *((_long*)&m_MouseState + eMouseMoveState);
	}

public:
	_bool Get_Key_Press(_ubyte eKeyID);
	_bool Get_Key_Down(_ubyte eKeyID);
	_bool Get_Key_Up(_ubyte eKeyID);

public:
	_bool Get_Mouse_Button_Down(MOUSEBUTTONSTATE eButtonID);
	_bool Get_Mouse_Button_Up(MOUSEBUTTONSTATE eButtonID);
	_bool Get_Mouse_Button_Press(MOUSEBUTTONSTATE eButtonID);

public:
	HRESULT Tick_KeyState();

public:
	HRESULT Ready_Input_Device(HINSTANCE hInst, HWND hWnd);
	HRESULT SetUp_InputDeviceState();

private:
	LPDIRECTINPUT8				m_pInput = nullptr;
	LPDIRECTINPUTDEVICE8		m_pKeyboard = nullptr;
	LPDIRECTINPUTDEVICE8		m_pMouse = nullptr;	


	
private:
	_byte					m_byKeyState[256];
	_byte					m_byFormalState[256];

	DIMOUSESTATE			m_MouseState;
	_byte					m_MouseFormalState[4];


public:
	virtual void Free() override;
};

END

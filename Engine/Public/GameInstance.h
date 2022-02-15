#pragma once

#include "Timer_Manager.h"
#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Camera_Manager.h"


/* 엔진내에 모든(클라이언트에 보여주기위한) 매니져 클래스를 보관한다. */

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
public:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(HINSTANCE hInstance, const CGraphic_Device::GRAPHICDESC& GraphicDesc, _uint iNumLevels, LPDIRECT3DDEVICE9* ppOut);
	_int Tick_Engine(_float fTimeDelta);
	HRESULT Clear_LevelResource(_uint iLevelIndex);

public: /* For.Graphic_Device */
	void Render_Begin(void);
	void Render_End(HWND hWnd = nullptr);	


public: /* For.Timer_Manager */
	_float	Get_TimeDelta(const _tchar* pTimerTag);
	HRESULT Add_Timer(const _tchar* pTimerTag);

public: /* For.Level_Manager */	
	HRESULT OpenLevel(_uint iLevelIndex, class CLevel* pNextLevel);
	HRESULT Render_Level();

public: /* For.Object_Manager */
	class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype); /* 원형객체를 추가한다. */
	HRESULT Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg = nullptr); /* 사본(게임내에 출현해야할 객체) 객체를 추가한다. */

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);

public: /* For.Input_Device */
	_byte Get_DIKeyState(_ubyte eKeyID);
	_long Get_DIMouseMoveState(CInput_Device::MOUSEMOVESTATE eMouseMoveState);
	_byte Get_DIMouseButtonState(CInput_Device::MOUSEBUTTONSTATE eMouseButtonState);

public: /* For.Camera_Manager*/
	HRESULT Render_Camera(CRenderer* renderer);
	HRESULT Add_Camera_Prototype(const _tchar * _Prototypetag, class CCamera * cam);
	HRESULT Add_Camera_Object(const _tchar * _Prototypetag, const _tchar* _ObjectTag, void* pArg = nullptr);
	HRESULT Release_Camera(const _tchar * _tag);
	CCamera* Find_Camera_Object(const _tchar* _ObjTag);

private:
	CGraphic_Device*		m_pGraphic_Device = nullptr;
	CTimer_Manager*			m_pTimer_Manager = nullptr;
	CLevel_Manager*			m_pLevel_Manager = nullptr;
	CObject_Manager*		m_pObject_Manager = nullptr;
	CComponent_Manager*		m_pComponent_Manager = nullptr;
	CInput_Device*			m_pInput_Device = nullptr;
	CCamera_Manager*		m_pCamera_Manager = nullptr;
public:
	static void Release_Engine();
	virtual void Free() override;
};

END
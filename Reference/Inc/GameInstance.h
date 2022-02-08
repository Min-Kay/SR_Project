#pragma once

#include "Timer_Manager.h"
#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "TextureMgr.h"

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

public:
	const LPDIRECT3DDEVICE9 Get_Device();

public: /* For.Graphic_Device */
	void Render_Begin(void);
	void Render_End(HWND hWnd = nullptr);	
	LPD3DXSPRITE		Get_Sprite(void);
	LPD3DXFONT			Get_Font(void);

public: /* For.Timer_Manager */
	_float	Get_TimeDelta(const _tchar* pTimerTag);
	HRESULT Add_Timer(const _tchar* pTimerTag);

public: /* For.Level_Manager */	
	HRESULT OpenLevel(_uint iCurrLevelIndex, _uint iLevelIndex, class CLevel* pNextLevel);
	HRESULT Render_Level();

public: /* For.Object_Manager */
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype); /* 원형객체를 추가한다. */
	HRESULT Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg = nullptr); /* 사본(게임내에 출현해야할 객체) 객체를 추가한다. */

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);

public: /* For.Input_Device */
	_byte Get_DIKeyState(_ubyte eKeyID);
	_long Get_DIMouseMoveState(CInput_Device::MOUSEMOVESTATE eMouseMoveState);
	_byte Get_DIMouseButtonState(CInput_Device::MOUSEBUTTONSTATE eMouseButtonState);


public: /* For.Texture_Manager */
	const TEXINFO* Get_Texture(const TCHAR * pObjKey, const TCHAR * pStateKey = L"", const int& iCnt = 0);
	HRESULT				InsertTexture(CTextureMgr::TEXTYPE eType, const TCHAR * pFilePath, const TCHAR * pObjKey, const TCHAR * pStateKey = L"", const int& iCnt = 0);

private:
	CGraphic_Device*		m_pGraphic_Device = nullptr;
	CTimer_Manager*			m_pTimer_Manager = nullptr;
	CLevel_Manager*			m_pLevel_Manager = nullptr;
	CObject_Manager*		m_pObject_Manager = nullptr;
	CComponent_Manager*		m_pComponent_Manager = nullptr;
	CInput_Device*			m_pInput_Device = nullptr;
	CTextureMgr*			m_pTexture_Manager = nullptr;

public:
	static void Release_Engine();
	virtual void Free() override;
};

END
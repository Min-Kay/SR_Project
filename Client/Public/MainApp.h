#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CRenderer;
class CGameInstance;
END

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT NativeConstruct(); /* 초기화한다. */
	_int Tick(_float fTimeDelta); /* 갱신한다. */
	HRESULT Render(); /* 그린다. */

private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	CGameInstance*			m_pGameInstance = nullptr;
	CRenderer*				m_pRenderer = nullptr;


#ifdef _DEBUG
private:
	_tchar				m_szFPS[MAX_PATH] = TEXT("");
	_ulong				m_dwNumRender = 0;
	_float				m_fTimerAcc = 0.0f;
#endif // _DEBUG

private:
	HRESULT OpenLevel(LEVEL eLevelID);
	HRESULT DefaultSetting();

	HRESULT Ready_Prototype_GameObject();
	HRESULT Ready_Prototype_Component();

public:
	static CMainApp* Create();
	virtual void Free();


};

END
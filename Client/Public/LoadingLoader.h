#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CLoadingLoader :
    public CBase
{
private:
	explicit CLoadingLoader(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLoadingLoader() = default;

public:
	CRITICAL_SECTION Get_CS() {
		return m_CS;
	}

	_bool isFinished() const {
		return m_isFinished;
	}
public:
	HRESULT NativeConstruct();
	HRESULT LoadToLoading();

private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
	HANDLE				m_hThread;
	_bool				m_isFinished = false;

	CRITICAL_SECTION	m_CS;

public:
	static CLoadingLoader* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};
END


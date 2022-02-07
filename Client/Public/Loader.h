#pragma once

#include "Client_Defines.h"
#include "Base.h"

/* 스레드를 생성한다. */
/* 스레드의 진입점함수를 정의한다. (각 레벨마다 로딩해야할 자원들을 생성한다.) */

BEGIN(Client)

class CLoader final : public CBase
{
private:
	explicit CLoader(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLoader() = default;

public:
	CRITICAL_SECTION Get_CS() {
		return m_CS;
	}

	LEVEL Get_LevelID() const {
		return m_eLevel;
	}

	_bool isFinished() const {
		return m_isFinished;
	}
public:
	HRESULT NativeConstruct(LEVEL eNextLevel);
	HRESULT Loading_ForGamePlay();
	HRESULT Loading_ForBoss();

private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
	LEVEL				m_eLevel = LEVEL_END;
	HANDLE				m_hThread;
	_bool				m_isFinished = false;

	CRITICAL_SECTION	m_CS;

public:
	static CLoader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel);
	virtual void Free() override;
};

END
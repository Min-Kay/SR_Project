#pragma once

#include "Base.h"

/* 여러개의 타이머들을 생성하여 보관한다. */

BEGIN(Engine)

class CTimer_Manager final : public CBase
{
	DECLARE_SINGLETON(CTimer_Manager)
private:
	CTimer_Manager();
	virtual ~CTimer_Manager() = default;

public:
	_float	Get_TimeDelta(const _tchar* pTimerTag);

public:
	HRESULT Add_Timer(const _tchar* pTimerTag);


private:
	map<const _tchar*, class CTimer*>			m_Timers;
	typedef map<const _tchar*, class CTimer*>	TIMERS;

private:
	class CTimer* Find_Timer(const _tchar* pTimerTag);

public:
	virtual void Free() override;
};

END
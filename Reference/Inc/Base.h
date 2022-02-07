#pragma once

#include "Engine_Defines.h"

/* 모든 객체들의 부모. */
/* 레퍼런스 카우늩를 관리한다. */

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;
public:
	unsigned long AddRef(); /* RefCnt를 증가시킨다. */
	unsigned long Release(); /* RefCnt를 감소시킨다. or  삭제한다. */

private:
	unsigned long			m_dwRefCnt = 0;	

public:
	virtual void Free() = 0;
};

END


//
//namespace Engine
//{
//	class CBase
//	{
//	public:
//		CBase();
//		~CBase();
//	};
//}


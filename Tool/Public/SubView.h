#pragma once
#include <afxwin.h>
class CSubView :
    public CView
{
	DECLARE_DYNCREATE(CSubView)

protected: // serialization에서만 만들어집니다.
    CSubView();
	virtual ~CSubView();

public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
public:

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


#pragma once
#include <afxwin.h>
class CSubView :
    public CView
{
	DECLARE_DYNCREATE(CSubView)

protected: // serialization������ ��������ϴ�.
    CSubView();
	virtual ~CSubView();

public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
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


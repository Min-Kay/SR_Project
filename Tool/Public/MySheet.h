#pragma once

#include "MyPage1.h"
#include "MyPage2.h"
// CMySheet

class CMySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMySheet)

public:
	CMySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CMySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CMySheet();

protected:
	DECLARE_MESSAGE_MAP()

private:
	CMyPage1			m_MyPage1;
	CMyPage2			m_MyPage2;
public:
	afx_msg void OnClose();
};



// MySheet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MySheet.h"


// CMySheet

IMPLEMENT_DYNAMIC(CMySheet, CPropertySheet)

CMySheet::CMySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	CPropertySheet::AddPage(&m_MyPage1);
	CPropertySheet::AddPage(&m_MyPage2);
}

CMySheet::CMySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	CPropertySheet::AddPage(&m_MyPage1);
	CPropertySheet::AddPage(&m_MyPage2);
}

CMySheet::~CMySheet()
{
}


BEGIN_MESSAGE_MAP(CMySheet, CPropertySheet)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMySheet 메시지 처리기입니다.


void CMySheet::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	ShowWindow(SW_HIDE);

	//CPropertySheet::OnClose();
}

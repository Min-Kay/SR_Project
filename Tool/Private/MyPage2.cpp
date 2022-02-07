// MyPage2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyPage2.h"
#include "afxdialogex.h"


// CMyPage2 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMyPage2, CPropertyPage)

CMyPage2::CMyPage2()
	: CPropertyPage(IDD_MYPAGE2)
{

}

CMyPage2::~CMyPage2()
{
}

void CMyPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyPage2, CPropertyPage)
END_MESSAGE_MAP()


// CMyPage2 메시지 처리기입니다.

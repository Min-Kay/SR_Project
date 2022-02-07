#pragma once


// CMyPage1 대화 상자입니다.

class CMyPage1 : public CPropertyPage
{
	DECLARE_DYNAMIC(CMyPage1)

public:
	CMyPage1();
	virtual ~CMyPage1();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYPAGE1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};

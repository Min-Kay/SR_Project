#pragma once


// CMyPage2 대화 상자입니다.

class CMyPage2 : public CPropertyPage
{
	DECLARE_DYNAMIC(CMyPage2)

public:
	CMyPage2();
	virtual ~CMyPage2();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYPAGE2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};

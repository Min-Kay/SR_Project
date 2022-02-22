#pragma once
#include "afxwin.h"
#include "Tool_Include.h"
// CMapTool 대화 상자입니다.

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapTool();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnDropFiles(HDROP hDropInfo);

public:
	void		HorizontalScroll(void);

	// value
	int				m_iDrawID = 0;

	bool			m_bTerrainText = false;
	bool			m_bRectText = false;
	int				m_itest = 1;
	// 픽처컨트롤은 bmp파일을 지원, 그러나 jpeg, png, gif, bmp 형식을 지원하는 cimage 객체를 이용하여 png 이미지를 출력해보자
	map<CString, CImage*>		m_MapPngImage;


	// control
	CListBox m_ListBox;
	CStatic m_Picture;
	afx_msg void OnSaveData();

	CButton m_TerrainText;
	CButton m_RectText;
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck1();
};
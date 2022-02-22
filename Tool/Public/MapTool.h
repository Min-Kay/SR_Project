#pragma once
#include "afxwin.h"
#include "Tool_Include.h"
// CMapTool ��ȭ �����Դϴ�.

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMapTool();

	// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
	// ��ó��Ʈ���� bmp������ ����, �׷��� jpeg, png, gif, bmp ������ �����ϴ� cimage ��ü�� �̿��Ͽ� png �̹����� ����غ���
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
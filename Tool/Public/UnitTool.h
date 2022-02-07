#pragma once

#include "Tool_Include.h"
#include "afxwin.h"

// CUnitTool 대화 상자입니다.

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUnitTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedButton1();
	afx_msg void OnAddCharacter();
	afx_msg void OnListBox();

public:
	// value
	CString m_strTemp;
	CString m_strResult;
	CString m_strName;
	int m_iAttack;
	int m_iDef;
	CString m_strFindName;


	// control
	CListBox m_ListBox;
	CButton m_Radio[3];
	CButton m_Check[3];
	CButton m_BitMap;

	// general
	map<CString, UNITDATA*>			m_mapUnitData;
	bool							m_bStart = false;

	afx_msg void OnDelete();
	afx_msg void OnSearch();
	afx_msg void OnSaveData();
	afx_msg void OnLoadData();
};


// 삭제, 저장, 불러오기 버튼 기능을 구현해오기

// 1. 삭제 : 리스트 박스에서 선택한 셀의 정보를 삭제
// 2. 저장 : 현재 리스트 박스에 띄워진 정보를 메모장 형태로 저장
// 3. 불러오기 : 저장된 파일을 기준으로 데이터를 읽어들여와서 리스트 박스에 다시 채워져야 함.
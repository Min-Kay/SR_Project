// UnitTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"
#include "afxdialogex.h"
#include "engine_function.h"


// CUnitTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UNITTOOL, pParent)
	, m_strTemp(_T(""))
	, m_strResult(_T(""))
	, m_strName(_T(""))
	, m_iAttack(0)
	, m_iDef(0)
	, m_strFindName(_T(""))
{

}

CUnitTool::~CUnitTool()
{
	for (auto& iter : m_mapUnitData)
		Engine::Safe_Delete(iter.second);

	m_mapUnitData.clear();

	
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strTemp);
	DDX_Text(pDX, IDC_EDIT2, m_strResult);
	DDX_Text(pDX, IDC_EDIT3, m_strName);
	DDX_Text(pDX, IDC_EDIT4, m_iAttack);
	DDX_Text(pDX, IDC_EDIT5, m_iDef);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_RADIO1, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio[2]);

	if (!m_bStart)
	{
		m_Radio[0].SetCheck(TRUE);
		m_bStart = true;
	}

	DDX_Control(pDX, IDC_CHECK1, m_Check[0]);
	DDX_Control(pDX, IDC_CHECK2, m_Check[1]);
	DDX_Control(pDX, IDC_CHECK3, m_Check[2]);
	DDX_Control(pDX, IDC_BUTTON3, m_BitMap);

	HBITMAP		hBitMap = (HBITMAP)LoadImage(nullptr, L"../Texture/JusinLogo1.bmp", IMAGE_BITMAP, 100, 50, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	m_BitMap.SetBitmap(hBitMap);

	DDX_Text(pDX, IDC_EDIT6, m_strFindName);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CUnitTool::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CUnitTool::OnAddCharacter)
	ON_LBN_SELCHANGE(IDC_LIST1, &CUnitTool::OnListBox)
	ON_BN_CLICKED(IDC_BUTTON6, &CUnitTool::OnDelete)
	ON_EN_CHANGE(IDC_EDIT6, &CUnitTool::OnSearch)
	ON_BN_CLICKED(IDC_BUTTON4, &CUnitTool::OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON5, &CUnitTool::OnLoadData)
END_MESSAGE_MAP()


// CUnitTool 메시지 처리기입니다.


void CUnitTool::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	// UpdateData(TRUE) : 다이얼로그 박스로부터 입력된 값들을 얻어옴
	UpdateData(TRUE);

	m_strResult = m_strTemp;

	// UpdateData(FALSE) : 변수에 저장된 값들을 다이얼로그 박스에 반영
	UpdateData(FALSE);
}


void CUnitTool::OnAddCharacter()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	UNITDATA*		pUnit = new UNITDATA;

	pUnit->strName = m_strName;
	pUnit->iAttack = m_iAttack;
	pUnit->iDef = m_iDef;

	for (int i = 0; i < 3; ++i)
	{
		if (m_Radio[i].GetCheck())
		{
			pUnit->byJobIndex = i;
			break;
		}
	}

	pUnit->byItem = 0x00;

	if (m_Check[0].GetCheck())
		pUnit->byItem |= RUBY;

	if (m_Check[1].GetCheck())
		pUnit->byItem |= DIAMOND;

	if (m_Check[2].GetCheck())
		pUnit->byItem |= SAPPHIRE;
		

	m_ListBox.AddString(pUnit->strName);

	m_mapUnitData.emplace(pUnit->strName, pUnit);

	UpdateData(FALSE);
}


void CUnitTool::OnListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CString	strFindName;

	// GetCurSel : 현재 리스트박에서 선택된 목록의 인덱스 값을 반환
	int iSelect = m_ListBox.GetCurSel();

	// GetText 인덱스 값에 해당하는 문자열을 얻어와서 2인자에게 넣어준다.
	m_ListBox.GetText(iSelect, strFindName);

	auto		iter = m_mapUnitData.find(strFindName);

	if (iter == m_mapUnitData.end())
		return;

	m_strName = iter->second->strName;
	m_iAttack = iter->second->iAttack;
	m_iDef = iter->second->iDef;

	for (int i = 0; i < 3; ++i)
		m_Radio[i].SetCheck(FALSE);	 
	
	m_Radio[iter->second->byJobIndex].SetCheck(TRUE);


	for (int i = 0; i < 3; ++i)
		m_Check[i].SetCheck(FALSE);

	if (iter->second->byItem & RUBY)
		m_Check[0].SetCheck(TRUE);

	if (iter->second->byItem & DIAMOND)
		m_Check[1].SetCheck(TRUE);

	if (iter->second->byItem & SAPPHIRE)
		m_Check[2].SetCheck(TRUE);


	UpdateData(FALSE);
}


void CUnitTool::OnDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CString	strFindName;

	// GetCurSel : 현재 리스트박에서 선택된 목록의 인덱스 값을 반환
	int iSelect = m_ListBox.GetCurSel();

	// LB_ERR(-1)
	if (LB_ERR == iSelect)
		return;

	m_ListBox.GetText(iSelect, strFindName);

	auto& iter = m_mapUnitData.find(strFindName);

	if (iter == m_mapUnitData.end())
		return;

	Engine::Safe_Delete(iter->second);
	m_mapUnitData.erase(strFindName);

	// 해당 목록의 리스트 문자열을 삭제하는 함수
	m_ListBox.DeleteString(iSelect);

	UpdateData(FALSE);
}



void CUnitTool::OnSearch()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	auto& iter = m_mapUnitData.find(m_strFindName);

	if (iter == m_mapUnitData.end())
		return;

	// 인덱스 0번부터 탐색하여 리스트 박스내에 있는 일치하는 문자 항목 인덱스 반환
	int iIndex = m_ListBox.FindString(0, m_strFindName);

	if (LB_ERR == iIndex)
		return;

	// 현재 인덱스가 선택된 것처럼 표시
	m_ListBox.SetCurSel(iIndex);


	m_strName = iter->second->strName;
	m_iAttack = iter->second->iAttack;
	m_iDef = iter->second->iDef;

	for (int i = 0; i < 3; ++i)
		m_Radio[i].SetCheck(FALSE);

	m_Radio[iter->second->byJobIndex].SetCheck(TRUE);


	for (int i = 0; i < 3; ++i)
		m_Check[i].SetCheck(FALSE);

	if (iter->second->byItem & RUBY)
		m_Check[0].SetCheck(TRUE);

	if (iter->second->byItem & DIAMOND)
		m_Check[1].SetCheck(TRUE);

	if (iter->second->byItem & SAPPHIRE)
		m_Check[2].SetCheck(TRUE);


	UpdateData(FALSE);

}


void CUnitTool::OnSaveData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// CFileDialog : 파일 열기, 저장 작업에 필요한 대화상자를 생성하는 객체

	/*1. TRUE(열기), FALSE(다른이름으로 저장) 모드 지정
	2. default 파일 확장자명
	3. 대화 상자에 최초로 표시될 파일명
	4. OFN_HIDEREADONLY(읽기 전용 체크박스 숨김) | OFN_OVERWRITEPROMPT(중복 파일 저장 시 경고 메세지 띄움)
	5. 대화 상자에 표시될 파일 형식 "콤보박스에 출력될 문자열 | 실제 사용할 필터링 문자열 ||"
	6. 부모 윈도우 주소*/

	CFileDialog		Dlg(FALSE,
					L"dat",
					L"*.dat", 
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
					L"Data Files(*.dat)|*.dat||",
					this);

	TCHAR	szPath[MAX_PATH] = L"";

	// 현재 프로젝트있는 디렉토리 경로를 얻어오는 함수
	GetCurrentDirectory(MAX_PATH, szPath);

	//	D:\유준환\124기\Frame124\Tool

	// 전체 경로에서 파일 이름만 잘라주는 함수
	// 만약 경로 상에 파일명이 없다면 맨 마지막 폴더명을 잘라냄

	PathRemoveFileSpec(szPath);
	//	D:\유준환\124기\Frame124\
	
	lstrcat(szPath, L"\\Data");
	//	D:\유준환\124기\Frame124\Data

	// 대화 상자를 열었을 때 보이는 기본 경로를 설정
	Dlg.m_ofn.lpstrInitialDir = szPath;

	// DoModal : 대화상자를 실행, IDOK 또는 IDCANCEL을 반환
	if (IDOK == Dlg.DoModal())
	{
		// GetPathName : 선택된 경로를 반환하는 함수
		CString				str = Dlg.GetPathName().GetString();
		const TCHAR*		pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD	dwByte = 0;
		DWORD	dwStrByte = 0;

		for (auto& iter : m_mapUnitData)
		{
			dwStrByte = sizeof(TCHAR) * (iter.first.GetLength() + 1);

			// key 값 저장
			WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, iter.first.GetString(), dwStrByte, &dwByte, nullptr);

			// value 값 저장 : unitdata 구조체 내부에 cstring 클래스 멤버를 갖고 있으면 구조체 통째로 출력하는 것은 위험, 따라서 멤버 별로 따로 저장
			WriteFile(hFile, &(iter.second->byItem), sizeof(BYTE), &dwByte, nullptr);
			WriteFile(hFile, &(iter.second->byJobIndex), sizeof(BYTE), &dwByte, nullptr);
			WriteFile(hFile, &(iter.second->iDef), sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &(iter.second->iAttack), sizeof(int), &dwByte, nullptr);

		}
		CloseHandle(hFile);
	}
}

/*
string	strName = "홍길동";
char*	pName = strName;
char szName[128] = strName;

strcpy_s(szName, 128, strName.c_str());*/

void CUnitTool::OnLoadData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CFileDialog		Dlg(TRUE,
						L"dat",
						L"*.dat",
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						L"Data Files(*.dat)|*.dat||",
						this);

	TCHAR	szPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szPath);

	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		for (auto& iter : m_mapUnitData)
			Engine::Safe_Delete(iter.second);

		m_mapUnitData.clear();

		// 기존의 리스트 목록을 모두 초기화
		m_ListBox.ResetContent();


		CString				str = Dlg.GetPathName().GetString();
		const TCHAR*		pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD	dwByte = 0;
		DWORD	dwStrByte = 0;

		UNITDATA	tData{};

		while (true)
		{
			// key 값 로드
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			TCHAR*		pName = new TCHAR[dwStrByte];

			ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

			// value 값 로드
			ReadFile(hFile, &(tData.byItem), sizeof(BYTE), &dwByte, nullptr);
			ReadFile(hFile, &(tData.byJobIndex), sizeof(BYTE), &dwByte, nullptr);
			ReadFile(hFile, &(tData.iDef), sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &(tData.iAttack), sizeof(int), &dwByte, nullptr);

			if (0 == dwByte)
			{
				delete[]pName;
				pName = nullptr;
				break;
			}

			UNITDATA*		pUnitData = new UNITDATA;
			pUnitData->strName = pName;
			delete[]pName;
			pName = nullptr;

			pUnitData->byItem = tData.byItem;
			pUnitData->byJobIndex = tData.byJobIndex;
			pUnitData->iDef = tData.iDef;
			pUnitData->iAttack = tData.iAttack;

			m_mapUnitData.emplace( pUnitData->strName, pUnitData);
			m_ListBox.AddString(pUnitData->strName);
		}
		CloseHandle(hFile);
	}


	UpdateData(FALSE);
}

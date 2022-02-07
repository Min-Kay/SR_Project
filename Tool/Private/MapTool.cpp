// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "Terrain.h"


// CMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
{

}

CMapTool::~CMapTool()
{
	for (auto& iter : m_MapPngImage)
	{
		iter.second->Destroy();
		Safe_Delete(iter.second);
	}
	m_MapPngImage.clear();

}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnLbnSelchangeList1)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnSaveData)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기입니다.


void CMapTool::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString	strFindName;

	// GetCurSel : 현재 리스트박에서 선택된 목록의 인덱스 값을 반환
	int iSelect = m_ListBox.GetCurSel();

	// GetText 인덱스 값에 해당하는 문자열을 얻어와서 2인자에게 넣어준다.
	m_ListBox.GetText(iSelect, strFindName);

	auto		iter = m_MapPngImage.find(strFindName);

	if (iter == m_MapPngImage.end())
		return;

	m_Picture.SetBitmap(*(iter->second));

	int i = 0;

	for (; i < strFindName.GetLength(); ++i)
	{
		//	isdigit : 인자 값으로 넘겨받은 단일 문자가 숫자인지 아니면 일반 글자인지 판별하는 함수
		// 숫자라고 판명된 글자인 경우 0이 아닌 값을 반환
		if (0 != isdigit(strFindName[i]))
			break;
	}

	// 첫 인자의 위치부터 두 번째 인자의 위치(카운트) 만큼 문자를 삭제하는 함수
	strFindName.Delete(0, i);

	// _tstoi : 문자를 정수형으로 바꿔주는 함수, 비슷한 이름의 함수가 매우 많으니 유의해서 찾아볼 것
	m_iDrawID = _tstoi(strFindName);

	UpdateData(FALSE);
}


void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(TRUE);
	CDialog::OnDropFiles(hDropInfo);


	TCHAR	szFilePath[MAX_PATH] = L"";

	// DragQueryFile : 드롭된 파일을 정보를 얻어오는 함수
	// 0xffffffff(-1) : 두 번째 인자값을 -1로 지정하면 드롭된 파일 개수를 반환하는 옵션

	int iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	TCHAR szFileName[64] = L"";

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);

		CString		strRelative = CFileInfo::ConvertRelativePath(szFilePath);

		// 경로 중 파일 이름만 추출하는 함수
		CString		strFileName = PathFindFileName(strRelative);

		lstrcpy(szFileName, strFileName.GetString());
		// 확장자명을 잘라내는 함수
		PathRemoveExtension(szFileName);

		// 키값과 일치하는 문자열을 넣어주기 위해 확장자명 까지 잘라낸 파일이름을 다시 strFileName에 저장해주고 있음
		strFileName = szFileName;

		auto	iter = m_MapPngImage.find(strFileName);

		if (iter == m_MapPngImage.end())
		{
			CImage*		pPngImage = new  CImage;
			pPngImage->Load(strRelative); // 해당 경로 이미지를 로드

			m_MapPngImage.emplace(strFileName, pPngImage);
			m_ListBox.AddString(strFileName);
		}			
		
	}
	HorizontalScroll();


	UpdateData(FALSE);
}

void CMapTool::HorizontalScroll(void)
{
	CString		strName;
	CSize		size;

	int			iCX = 0;

	// 리스트 박스의 DC를 얻어옴
	CDC*		pDC = m_ListBox.GetDC();

	// 현재 리스트 박스 목록의 개수를 반환
	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strName);
		
		// 현재 문자열의 길이를 픽셀단위로 변환
		size = pDC->GetTextExtent(strName);

		if (size.cx > iCX)
			iCX = size.cx;
	}

	m_ListBox.ReleaseDC(pDC);

	// GetHorizontalExtent :리스트 박스 가로로 스크롤 할 수 있는 최대 범위를 얻어오는 함수
	if (iCX > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iCX);


}


void CMapTool::OnSaveData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog		Dlg(FALSE,
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
		// GetPathName : 선택된 경로를 반환하는 함수
		CString				str = Dlg.GetPathName().GetString();
		const TCHAR*		pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;


		CMainFrame*	pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CToolView*	pToolView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
		CTerrain*	pTerrain = pToolView->m_pTerrain;
		vector<TILE*>& vecTile = pTerrain->Get_Tile();
	
		DWORD	dwByte = 0;

		for (auto& iter : vecTile)
		{
			WriteFile(hFile, iter, sizeof(TILE), &dwByte, nullptr);

		}
		CloseHandle(hFile);
	}
}

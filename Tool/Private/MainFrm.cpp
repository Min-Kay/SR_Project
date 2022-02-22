
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "Tool.h"

#include "MainFrm.h"
#include "MyForm.h"
#include "ToolView.h"
#include "SubView.h"
#include "GameInstance.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

// 특정 윈도우 메세지가 발생할 경우 호출되는 함수들을 모아놓은 전처리기
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	
}

CMainFrame::~CMainFrame()
{
	
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
		
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// 뷰 객체를 생성하는 시점에 호출(사용자가 보 생성을 제어하거나 추가 생성이 가능)


	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// CreateStatic  : 분할 창을 만들기 위한 함수
	// 1인자 : 부모 윈도우의 주소
	// 2인자 : 행의 수
	// 3인자 : 열의 수
	// 4인자 : 창 스타일
	// 5인자 : 자식창의 id 값

	/*m_MainSplitter.CreateStatic(this, 1, 2);

	m_MainSplitter.CreateView(0, 0, RUNTIME_CLASS(CMyForm), CSize(300, WINCY), pContext);
	m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CToolView), CSize(WINCX, WINCY), pContext);*/

	/*m_MainSplitter.CreateStatic(this, 2, 2);

	m_MainSplitter.CreateView(0, 0, RUNTIME_CLASS(CMiniView), CSize(300, 300), pContext);
	m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CMyForm), CSize(300, 300), pContext);
	m_MainSplitter.CreateView(1, 0, RUNTIME_CLASS(CMyForm), CSize(300, 300), pContext);
	m_MainSplitter.CreateView(1, 1, RUNTIME_CLASS(CToolView), CSize(WINCX, WINCY), pContext);*/
	
	m_MainSplitter.CreateStatic(this, 1, 2);
	//m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CRuntime), CSize(g_iWinCX, g_iWinCY), pContext);
	m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CToolView), CSize(g_iWinCX, g_iWinCY), pContext);
//	CRuntime* pRunTime = (CRuntime *)AfxGetApp();
	//pRunTime->OnIdle(0);
	// WS_CHILD : 자식 창이라는 의미
	// WS_VISIBLE : 생성 후 바로 화면에 표시하겠다는 의미
	// IdFromRowCol : 지정한 행과 열에 해당하는 창의 id를 반환하는 함수

	//m_SecondSplitter.CreateStatic(&m_MainSplitter, 2, 1, WS_CHILD | WS_VISIBLE, m_MainSplitter.IdFromRowCol(0, 0));

	m_MainSplitter.CreateView(0, 0, RUNTIME_CLASS(CMyForm), CSize(300, 300), pContext);
	//m_SecondSplitter.CreateView(1, 0, RUNTIME_CLASS(CMyForm), CSize(300, 300), pContext);

	// 열 번호, 열의 크기 지정, 허용 가능한 최소 크기
	m_MainSplitter.SetColumnInfo(0, 300, 10);
	return TRUE; //CFrameWnd::OnCreateClient(lpcs, pContext);
}

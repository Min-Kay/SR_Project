// MiniView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MiniView.h"
#include "GameInstance.h"
#include "MainFrm.h"
#include "ToolView.h"

// CMiniView

IMPLEMENT_DYNCREATE(CMiniView, CView)

CMiniView::CMiniView()
{

}

CMiniView::~CMiniView()
{
}

BEGIN_MESSAGE_MAP(CMiniView, CView)
END_MESSAGE_MAP()


// CMiniView 그리기입니다.


void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.

	CGameInstance::GetInstance()->Render_Begin();

	// AfxGetMainWnd : 현재 스레드로부터 wnd를 반환

	CMainFrame*		pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	CToolView*		pMainView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	CTerrain*		pTerrain = pMainView->Get_Terrain();
	pTerrain->MiniRender();


	CGameInstance::GetInstance()->Render_End();

}



// CMiniView 진단입니다.

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiniView 메시지 처리기입니다.

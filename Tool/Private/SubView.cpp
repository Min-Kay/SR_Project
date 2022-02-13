#include "stdafx.h"
#include "Tool.h"
#include "SubView.h"
#include "GameInstance.h"
#include "ToolView.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CSubView, CView)

BEGIN_MESSAGE_MAP(CSubView, CView)
END_MESSAGE_MAP()

CSubView::CSubView()
{
}

CSubView::~CSubView()
{
}

void CSubView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	pInstance->Render_Begin();
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
	pInstance->Render_End(m_hWnd);

	RELEASE_INSTANCE(CGameInstance);
}




#ifdef _DEBUG
void CSubView::AssertValid() const
{
}
#ifndef _WIN32_WCE
void CSubView::Dump(CDumpContext& dc) const
{
}
#endif
#endif //_DEBUG

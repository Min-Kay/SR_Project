// MiniView.cpp : ���� �����Դϴ�.
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


// CMiniView �׸����Դϴ�.


void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.

	CGameInstance::GetInstance()->Render_Begin();

	// AfxGetMainWnd : ���� ������κ��� wnd�� ��ȯ

	CMainFrame*		pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	CToolView*		pMainView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	CTerrain*		pTerrain = pMainView->Get_Terrain();
	pTerrain->MiniRender();


	CGameInstance::GetInstance()->Render_End();

}



// CMiniView �����Դϴ�.

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


// CMiniView �޽��� ó�����Դϴ�.

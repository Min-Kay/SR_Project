
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "SubView.h"
#include "MyForm.h"
#include "GameInstance.h"
#include "Renderer.h"
#include "Camera_Dynamic.h"
#include "Runtime.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CToolView

HWND	g_hWnd;
IMPLEMENT_DYNCREATE(CToolView, CView)
BEGIN_MESSAGE_MAP(CToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
//	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

CToolView::CToolView()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
}

CToolView::~CToolView()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pRenderer);
	Safe_Release(m_device);
	CGameInstance::Release_Engine();


}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CToolView �׸���

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//ƽȣ��
	Tick(1.f);
//Render
		CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
		pInstance->Render_Camera(m_pRenderer);
		
		RELEASE_INSTANCE(CGameInstance);

	


}


// CToolView �μ�

_int CToolView::Tick(_float fTimeDelta)
{
	
	//CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	if (0 > m_pGameInstance->Tick_Tool(fTimeDelta))
		return -1;

	//#ifdef _DEBUG
	//	m_fTimerAcc += fTimeDelta;
	//#endif // _DEBUG

	//RELEASE_INSTANCE(CGameInstance);
	return _int();
}

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����



void CToolView::OnInitialUpdate()

{
	CView::OnInitialUpdate();

	// ��ũ�� ���� ����� �����ϴ� �Լ�
	//  MM_TEXT: �ȼ� ������ ����� �����ϰڴٴ� ���ڰ�
	// 2���� : ����� ǥ���ϴ� Ŭ����

	//SetScrollSizes(MM_TEXT, CSize(TILECX * TILEX, (TILECY * TILEY / 2)));
	//MM_TEXT
	g_hWnd = m_hWnd;
	m_bReady = true;
	//HINSTANCE hInst = GetModuleHandle(NULL);

	CGraphic_Device::GRAPHICDESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(CGraphic_Device::GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;
	GraphicDesc.eScreenMode = CGraphic_Device::TYPE_WINMODE;
	
	//CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pGameInstance->Initialize_Engine(nullptr, GraphicDesc,2, &m_device)))//g_hInst = nullptr;
	{
		AfxMessageBox(L"Device Init Failed");
		return;
	}

	////Ready_Prototype_GameObject
	if (FAILED(m_pGameInstance->Add_Prototype(0, TEXT("Prototype_Component_Renderer"), m_pRenderer = CRenderer::Create(m_device))))
		return;

	m_device->SetRenderState(D3DRS_LIGHTING,false);

	m_pGameInstance->Add_Camera_Prototype(TEXT("Prototype_GameObject_Camera"),CCamera_Dynamic::Create(m_device));
	m_pGameInstance->Add_Camera_Object(TEXT("Prototype_GameObject_Camera"),TEXT("Camera"));


	////Ready_Prototype_Component


	if (nullptr == m_pGameInstance)
	{
		AfxMessageBox(L"Device Init Failed");
		return;
	}

	/* For.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), CTransform::Create(m_device))))
		return ;

	/* For.Prototype_Component_VIBuffer_Rect */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_device))))
	//	return ;

	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_device))))
		return ;


	/* For.Prototype_Component_Texture_Default */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Default"), CTexture::Create(m_device, CTexture::TYPE_CUBEMAP, TEXT("../../Resources/Textures/SkyBox/burger%d.dds"),4))))
		return ;


	/* For.Prototype_Component_Texture_Default */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Camera"), CTexture::Create(m_device, g_iWinCX, g_iWinCY))))

	Safe_AddRef(m_pRenderer);
	//RELEASE_INSTANCE(CGameInstance);


		// AfxGetMainWnd : ���� ���� �����츦 ��ȯ�ϴ� ���� �Լ�
		// ��ȯŸ���� �θ�Ÿ���̾ �ڽ� Ÿ������ ����ȯ ����
	CMainFrame*	pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWindow{};

	// GetWindowRect : ���� �������� rect ������ ������ �Լ�
	pMainFrm->GetWindowRect(&rcWindow);

	// SetRect : ������ ���ڰ� ��� rect������ �����ϴ� �Լ�
	// 0,0 �������� ������ ��Ʈ ������ ������
	//SetRect(&rcWindow, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);

	RECT		rcMainView{};

	// ���� view â�� rect ������ ������ �Լ�
	GetClientRect(&rcMainView);

	//float	fRowFrm = float(rcWindow.right - rcMainView.right);
	//float	fColFrm = float(rcWindow.bottom - rcMainView.bottom);

	// SetWindowPos : ������ â�� ��ġ �� ũ�⸦ �������ϴ� �Լ�
	// 1���� : ��ġ�� �������� z������ ���� ������
	 //x��ǥ, y��ǥ, ���� ũ��, ���� ũ��
	// SWP_NOZORDER : ���� z������ �����ϰڴٴ� �÷��� ��
	//pMainFrm->SetWindowPos(NULL, 0,0, int(g_iWinCX + fRowFrm), int(g_iWinCY + fColFrm), SWP_NOZORDER);
	

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	

	//�纻 ��ü ����
	if (FAILED(Ready_Prototype_GameObject()))
	{
		AfxMessageBox(L"Ready_Prototype_GameObject create Failed");
		return;

	}

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
	{
		AfxMessageBox(L"Ready_Layer_Camera create Failed");
		return;

	}


	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
	{
		AfxMessageBox(L"Ready_Layer_BackGround create Failed");
		return;
	}
		
		//Safe_Release(m_device);


}


HRESULT CToolView::Ready_Prototype_GameObject()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* ������ü ���� */
	if (FAILED(pGameInstance->Add_Camera_Prototype(TEXT("Prototype_GameObject_Camera_Dynamic"), CCamera_Dynamic::Create(m_device))))
		return E_FAIL;
	/* For.Prototype_GameObject_BackGround */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"), CBackGround::Create(m_device))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CToolView::Ready_Prototype_Component()
{
	return E_NOTIMPL;
}

HRESULT CToolView::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* �纻��ü�� ����������. */
	if (FAILED(pGameInstance->Add_GameObject(1, pLayerTag, TEXT("Prototype_GameObject_BackGround"))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CToolView::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* �纻��ü�� ����������. */
	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CameraDesc));

	CameraDesc.vEye = _float3(0.f, 0.f, -30.f);
	CameraDesc.vAt = _float3(0.f, 0.f,0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 300.f;

	CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	CameraDesc.iLevel = LEVEL_LOGO;


	if (FAILED(pGameInstance->Add_Camera_Object(TEXT("Prototype_GameObject_Camera_Dynamic"), pLayerTag, &CameraDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;

}

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CView::OnLButtonDown(nFlags, point);

	/*if (GetAsyncKeyState(VK_LBUTTON))
	{
		OnDraw(nullptr);
	}*/
	Invalidate(FALSE);
	////Invalidate : ȣ�� �� ������ wm_paint�� wm_erasebkgnd �޼����� �߻���Ŵ
	//// ondraw �Լ��� �ٽ� �� �� ȣ��
	//// ���ڰ��� FALSE�϶��� wm_paint�� �޽����� �߻�
	//// ���ڰ��� true�϶� wm_paint�� wm_erasebkgnd �� �޼����� ���ÿ� �߻�
	//// wm_erasebkgnd �޼��� : ����� ������ �޽���
	//Invalidate(FALSE);


	//CMainFrame*	pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	////CMainFrame*	pMain = dynamic_cast<CMainFrame*>(GetParentFrame());

	//CSubView*	pMiniView = dynamic_cast<CSubView*>(pMain->m_SecondSplitter.GetPane(0, 0));

	//CMyForm*	pMyForm = dynamic_cast<CMyForm*>(pMain->m_SecondSplitter.GetPane(1, 0));

	//CMapTool*	pMapTool = &pMyForm->m_MapTool;

	//
	//pMiniView->Invalidate(FALSE);
}


//void CToolView::OnMouseMove(UINT nFlags, CPoint point)
//{
//	//// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
//
//	CView::OnMouseMove(nFlags, point);
//
//	Invalidate(FALSE);
//
//
//	//	Invalidate(FALSE);
//
//		//CMainFrame*	pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
//		//CSubView*	pMiniView = dynamic_cast<CSubView*>(pMain->m_SecondSplitter.GetPane(0, 0));
//		//CMyForm*	pMyForm = dynamic_cast<CMyForm*>(pMain->m_SecondSplitter.GetPane(1, 0));
//		//CMapTool*	pMapTool = &pMyForm->m_MapTool;
//
//		//pMiniView->Invalidate(FALSE);
//	//}
//}
void CToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	Invalidate(FALSE);
//	if (GetKeyState('W') & 0x0800)
//	{
//		int a = 10;
//	}

}
void CToolView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
	Invalidate(FALSE);
}



BOOL CToolView::OnIdle(LONG lCount)
{
	//CWinApp::OnIdle(lCount);

	//AfxGetMainWnd()->Invalidate(false);

	return TRUE;
}



// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
//	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
	// TODO: 여기에 생성 코드를 추가합니다.
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
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//틱호출
	Tick(1.f);
//Render
		CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
		pInstance->Render_Camera(m_pRenderer);
		
		RELEASE_INSTANCE(CGameInstance);

	


}


// CToolView 인쇄

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
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기



void CToolView::OnInitialUpdate()

{
	CView::OnInitialUpdate();

	// 스크롤 바의 사이즈를 지정하는 함수
	//  MM_TEXT: 픽셀 단위로 사이즈를 조정하겠다는 인자값
	// 2인자 : 사이즈를 표현하는 클래스

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


		// AfxGetMainWnd : 현재 메인 윈도우를 반환하는 전역 함수
		// 반환타입이 부모타입이어서 자식 타입으로 형변환 했음
	CMainFrame*	pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWindow{};

	// GetWindowRect : 현재 윈도우의 rect 정보를 얻어오는 함수
	pMainFrm->GetWindowRect(&rcWindow);

	// SetRect : 지정한 인자값 대로 rect정보를 세팅하는 함수
	// 0,0 기준으로 윈도우 렉트 정보를 재조정
	//SetRect(&rcWindow, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);

	RECT		rcMainView{};

	// 현재 view 창의 rect 정보를 얻어오는 함수
	GetClientRect(&rcMainView);

	//float	fRowFrm = float(rcWindow.right - rcMainView.right);
	//float	fColFrm = float(rcWindow.bottom - rcMainView.bottom);

	// SetWindowPos : 윈도우 창의 위치 및 크기를 재조정하는 함수
	// 1인자 : 배치할 윈도우의 z순서에 대한 포인터
	 //x좌표, y좌표, 가로 크기, 세로 크기
	// SWP_NOZORDER : 현재 z순서를 유지하겠다는 플래그 값
	//pMainFrm->SetWindowPos(NULL, 0,0, int(g_iWinCX + fRowFrm), int(g_iWinCY + fColFrm), SWP_NOZORDER);
	

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	

	//사본 객체 생성
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

	/* 원본객체 생성 */
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

	/* 사본객체를 생성ㅎ나다. */
	if (FAILED(pGameInstance->Add_GameObject(1, pLayerTag, TEXT("Prototype_GameObject_BackGround"))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CToolView::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* 사본객체를 생성ㅎ나다. */
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonDown(nFlags, point);

	/*if (GetAsyncKeyState(VK_LBUTTON))
	{
		OnDraw(nullptr);
	}*/
	Invalidate(FALSE);
	////Invalidate : 호출 시 윈도우 wm_paint와 wm_erasebkgnd 메세지를 발생시킴
	//// ondraw 함수를 다시 한 번 호출
	//// 인자값이 FALSE일때는 wm_paint만 메시지만 발생
	//// 인자값이 true일때 wm_paint와 wm_erasebkgnd 두 메세지를 동시에 발생
	//// wm_erasebkgnd 메세지 : 배경을 지우라는 메시지
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
//	//// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	Invalidate(FALSE);
//	if (GetKeyState('W') & 0x0800)
//	{
//		int a = 10;
//	}

}
void CToolView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
	Invalidate(FALSE);
}



BOOL CToolView::OnIdle(LONG lCount)
{
	//CWinApp::OnIdle(lCount);

	//AfxGetMainWnd()->Invalidate(false);

	return TRUE;
}


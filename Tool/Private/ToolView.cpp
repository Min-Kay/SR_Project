
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

#include "BackGround.h"
#include "Cube.h"
#include "Terrain.h"

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
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
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

void CToolView::OnDraw(CDC* pDC)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	/* For.Timer_Default */
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	Tick(1.f);
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


	//////////////////////////////////////////////////////기본설정
	g_hWnd = m_hWnd;

	CGraphic_Device::GRAPHICDESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(CGraphic_Device::GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;
	GraphicDesc.eScreenMode = CGraphic_Device::TYPE_WINMODE;

	if (FAILED(m_pGameInstance->Initialize_Engine(nullptr, GraphicDesc, 2, &m_device)))//g_hInst = nullptr;
	{
		AfxMessageBox(L"Device Init Failed");
		return;
	}

	if (FAILED(DefaultSetting()))
	{
		AfxMessageBox(L"DefaultSetting Init Failed");
		return;
	}

	if (FAILED(Ready_Prototype_Component()))
	{
		AfxMessageBox(L"Ready_Prototype_Component Init Failed");
		return;
	}

	if (FAILED(Ready_Prototype_GameObject()))
	{
		AfxMessageBox(L"Ready_Prototype_GameObject Init Failed");
		return;
	}

	//////////////////////////////////////////////////////


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
	//if (FAILED(Ready_Layer_Cube(TEXT("Layer_Cube"))))
	//{
	//	AfxMessageBox(L"Ready_Layer_Cube create Failed");
	//	return;
	//}
	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
	{
		AfxMessageBox(L"Ready_Layer_Terrain create Failed");
		return;
	}


}


HRESULT CToolView::DefaultSetting()
{
	if (nullptr == m_device)
		return E_FAIL;

	/* 바인딩되어있는 텍스쳐로부터 픽셀값을 얻어오는 작업을 수행할때에 대한 설정. */
	m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	m_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	/* 그리는 상태를 셋팅한다. */
	m_device->SetRenderState(D3DRS_LIGHTING, false);
	//m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);


	return S_OK;
}

HRESULT CToolView::Ready_Prototype_GameObject()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	/* 원본객체 생성 */
	if (FAILED(pGameInstance->Add_Camera_Prototype(TEXT("Prototype_GameObject_Camera_Dynamic"), CCamera_Dynamic::Create(m_device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"), CBackGround::Create(m_device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cube"), CCube::Create(m_device))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"), CTerrain::Create(m_device))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CToolView::Ready_Prototype_Component()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* For.Prototype_Component_Renderer */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), m_pRenderer = CRenderer::Create(m_device))))
		return E_FAIL;

	/* For.Prototype_Component_Transform */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), CTransform::Create(m_device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_device))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Default */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Default"), CTexture::Create(m_device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Terrain/Block_%d.png"), 6))))
		return E_FAIL;


	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_device))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Cube"), CTexture::Create(m_device, CTexture::TYPE_CUBEMAP, TEXT("../../Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"), CVIBuffer_Terrain::Create(m_device,16,16 )/*TEXT("../../Resources/Textures/Terrain/Height.bmp")*//*129, 129*/)))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Terrain"), CTexture::Create(m_device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Terrain/Block_%d.png"), 6))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
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

HRESULT CToolView::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* 사본객체를 생성ㅎ나다. */
	if (FAILED(pGameInstance->Add_GameObject(1, pLayerTag, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CToolView::Ready_Layer_Cube(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* 사본객체를 생성ㅎ나다. */

	if (FAILED(pGameInstance->Add_GameObject(1, pLayerTag, TEXT("Prototype_GameObject_Cube"))))
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

	CameraDesc.vEye = _float3(0.f, 30.f, -30.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
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

//void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
//{
//}



//void CToolView::OnMouseMove(UINT nFlags, CPoint point)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//
//	CView::OnMouseMove(nFlags, point);
//
//	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
//
//	CToolView *pView = (CToolView *)pFrame->GetActiveView();
//
//	GetCursorPos(&m_MousePos_ToolView);
//	ScreenToClient(pView->m_hWnd, &m_MousePos_ToolView);
//	//m_MousePos_ToolView
//	//Invalidate(false);
//}



//

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
	// ǥ�� �μ� ����Դϴ�.
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


	//////////////////////////////////////////////////////�⺻����
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

	/* ���ε��Ǿ��ִ� �ؽ��ķκ��� �ȼ����� ������ �۾��� �����Ҷ��� ���� ����. */
	m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	m_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	/* �׸��� ���¸� �����Ѵ�. */
	m_device->SetRenderState(D3DRS_LIGHTING, false);
	//m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);


	return S_OK;
}

HRESULT CToolView::Ready_Prototype_GameObject()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	/* ������ü ���� */
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

	/* �纻��ü�� ����������. */
	if (FAILED(pGameInstance->Add_GameObject(1, pLayerTag, TEXT("Prototype_GameObject_BackGround"))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CToolView::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* �纻��ü�� ����������. */
	if (FAILED(pGameInstance->Add_GameObject(1, pLayerTag, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CToolView::Ready_Layer_Cube(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* �纻��ü�� ����������. */

	if (FAILED(pGameInstance->Add_GameObject(1, pLayerTag, TEXT("Prototype_GameObject_Cube"))))
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
//	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
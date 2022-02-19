#include "stdafx.h"
#include "..\Public\MainApp.h"
#include "GameInstance.h"
#include "Level_Logo.h"
#include "Level_Loading.h"
#include "Camera_Static.h"
#include "UI.h"
CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::NativeConstruct()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	CGraphic_Device::GRAPHICDESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(CGraphic_Device::GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;
	GraphicDesc.eScreenMode = CGraphic_Device::TYPE_WINMODE;

	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, GraphicDesc, LEVEL_END, &m_pGraphic_Device)))
		return E_FAIL;

	if (FAILED(DefaultSetting()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_GameObject()))
		return E_FAIL;	

	if (FAILED(OpenLevel(LEVEL_LOGO)))
		return E_FAIL;



	return S_OK;
}

_int CMainApp::Tick(_float fTimeDelta)
{
	if (nullptr == m_pGameInstance)
		return -1;

	if (0 > m_pGameInstance->Tick_Engine(fTimeDelta))
		return -1;

//#ifdef _DEBUG
//	m_fTimerAcc += fTimeDelta;
//#endif // _DEBUG

	return _int();
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;


	m_pGameInstance->Render_Camera(m_pRenderer);

//#ifdef _DEBUG
//	++m_dwNumRender;
//
//	if (m_fTimerAcc >= 1.f)
//	{
//		wsprintf(m_szFPS, TEXT("FPS : %d"), m_dwNumRender);
//		SetWindowText(g_hWnd, m_szFPS);
//
//		m_dwNumRender = 0;
//		m_fTimerAcc = 0.f;
//	}
//#endif // _DEBUG

	return S_OK;
}


HRESULT CMainApp::OpenLevel(LEVEL eLevelID)
{
	CLevel*			pLevel = nullptr;

	switch (eLevelID)
	{
	case LEVEL_LOGO: /* 바로 로고레벨로 할당들어간다. */
		pLevel = CLevel_Logo::Create(m_pGraphic_Device);
		break;
	case LEVEL_GAMEPLAY: /* 바로 로딩레벨로 선할당. 로딩레벨안에서 로딩이 끝나면 게임플레이로 할당들어간다. */
	case LEVEL_BOSS:		
		pLevel = CLevel_Loading::Create(m_pGraphic_Device, eLevelID);		
		eLevelID = LEVEL_LOADING;
		break;
	}	

	if (nullptr == pLevel)
		return E_FAIL;

	if (FAILED(m_pGameInstance->OpenLevel(/*eLevelID == LEVEL_LOGO ? eLevelID : LEVEL_LOADING*/eLevelID, pLevel)))
		return E_FAIL;	

	return S_OK;
}

HRESULT CMainApp::DefaultSetting()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/* 바인딩되어있는 텍스쳐로부터 픽셀값을 얻어오는 작업을 수행할때에 대한 설정. */
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	/* 그리는 상태를 셋팅한다. */
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);
	// m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);


	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObject()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Camera_Prototype(CAM_STATIC, CCamera_Static::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(PROTO_UI, CUI::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* For.Prototype_Component_Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PROTO_RENDERER, m_pRenderer = CRenderer::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PROTO_TRANSFORM, CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PROTO_RECT, CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Logo */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Logo"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Main/Main.jpg")))))
		return E_FAIL;

	Safe_AddRef(m_pRenderer);

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CMainApp");
		Safe_Release(pInstance);		
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pGameInstance);

	CGameInstance::Release_Engine();

}


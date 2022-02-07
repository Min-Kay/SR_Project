#include "Graphic_Device.h"

IMPLEMENT_SINGLETON(CGraphic_Device)

CGraphic_Device::CGraphic_Device()
	: m_p3D(nullptr) , m_pDevice(nullptr), m_pSprite(nullptr), m_pFont(nullptr)
{

}



HRESULT CGraphic_Device::InitDevice(const GRAPHICDESC& GraphicDesc, LPDIRECT3DDEVICE9* ppOut)
{
	m_p3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9			DeviceCaps;
	
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	
	if (FAILED(m_p3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		MSGBOX("GetDeviceCaps Failed"); // mfc에서만 사용가능한 메세지 박스 함수
		return E_FAIL;
	}



	DWORD	vp = 0;		// 버텍스 프로세싱 : 정점 연산 + 조명 처리

	
	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;		// 그래픽 카드

	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;		// 직접 연산(CPU)

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dpp, GraphicDesc);

	if (FAILED(m_p3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GraphicDesc.hWnd, vp, &d3dpp, &m_pDevice)))
	{
		MSGBOX("CreateDevice Failed"); // mfc에서만 사용가능한 메세지 박스 함수
		return E_FAIL;
	}

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		MSGBOX("m_pSprite Failed");
		return E_FAIL;
	}

	D3DXFONT_DESCW	tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 20;
	tFontInfo.Width = 10;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"궁서");
	
	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		MSGBOX("m_pFont Failed");
		return E_FAIL;
	}	

	*ppOut = m_pDevice;

	Safe_AddRef(m_pDevice);

	return S_OK;
}

void CGraphic_Device::SetParameters(D3DPRESENT_PARAMETERS & d3dpp, const GRAPHICDESC& GraphicDesc)
{
	d3dpp.BackBufferWidth = GraphicDesc.iWinCX;
	d3dpp.BackBufferHeight = GraphicDesc.iWinCY;

	// 후면 버퍼의 픽셀 포맷 , 32비트 포맷 사용
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	
	// 후면 버퍼의 개수
	d3dpp.BackBufferCount = 1;
	
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GraphicDesc.hWnd;	
	
	d3dpp.Windowed = GraphicDesc.eScreenMode; 

	/* 깊이 스텐실 버퍼를 생성한다ㅏ. */

	
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

void CGraphic_Device::Render_Begin(void)
{
	
	m_pDevice->Clear(0,
		nullptr,
		D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_ARGB(255, 0, 0, 255),	// 백버퍼 색상
		1.f, // z버퍼의 초기화 값
		0);	 // 스텐실 버퍼의 초기화 값

	m_pDevice->BeginScene();

}

void CGraphic_Device::Render_End()
{	
	m_pDevice->EndScene();

	// 후면버퍼에 연결되어 있는 서피스와 전면 버퍼에 연결된 서피스를 교환하는 과정
	m_pDevice->Present(nullptr, nullptr, 0, nullptr);
	// 1,2인자 : 스왑체인 기법이 D3DSWAPEFFECT_COPY가 아닌 이상 NULL
	// 3인자 : 출력 대상 윈도우 핸들, 기본 nullptr인 경우 d3dpp.hDeviceWindow을 참조
	// 4인자 :  스왑체인 기법이 D3DSWAPEFFECT_COPY가 아닌 이상 NULL

}

void CGraphic_Device::Free(void)
{
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);

	if (0 != Safe_Release(m_pDevice))
		MSGBOX("Failed to Release IDirect3DDevice9");
	
	if (0 != Safe_Release(m_p3D))
		MSGBOX("Failed to Release IDirect3D9");
}

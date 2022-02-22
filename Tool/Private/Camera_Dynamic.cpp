#include "stdafx.h"
#include "Camera_Dynamic.h"
#include "GameInstance.h"
#include "ToolView.h"
#include "MainFrm.h"

CCamera_Dynamic::CCamera_Dynamic(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{
}

CCamera_Dynamic::CCamera_Dynamic(const CCamera_Dynamic & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Dynamic::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Dynamic::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CCamera_Dynamic::Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	
	if (GetKeyState('W') & 0x80)
	{
		m_pTransform->Go_Straight(fTimeDelta*0.1f);
	}

	if (GetKeyState('S') & 0x80)
	{
		m_pTransform->Go_BackWard(fTimeDelta*0.1f);
	}

	if (GetKeyState('A') & 0x80)
	{
		m_pTransform->Go_Left(fTimeDelta*0.1f);
	}

	if (GetKeyState('D') & 0x80)
	{
		m_pTransform->Go_Right(fTimeDelta*0.1f);
	}
	if (GetKeyState('Q') & 0x80)
	{
		m_pTransform->Turn(_float3(0.f,1.f,0.f),fTimeDelta*0.1f);
	}
	if (GetKeyState('E') & 0x80)
	{
		m_pTransform->Turn(_float3(0.f, -1.f, 0.f), fTimeDelta*0.1f);
	}
	//마우스를 우클릭하면 거기에 좌표를 고정하고 m_MousePos
	// 움직이면 그만큼 비례해서 회전하면 될듯?
	//그리고 마우스 놓으면 다시 초기화
	//  0:       이전에 누른 적이 없고 호출시점에 안 눌린 상태
	//	0x8000 : 이전에 누른 적이 없고 호출시점에 눌린 상태
	//	0x8001 : 이전에 누른 적이 있고 호출시점에 눌린 상태
	//	1 : 이전에 누른 적이 있고 호출시점에 안 눌린 상태
	if (GetKeyState(VK_RBUTTON) & 1)
	{
		CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CToolView *pView = (CToolView *)pFrame->GetActiveView();

		GetCursorPos(&m_MousePos);
		ScreenToClient(pView->m_hWnd, &m_MousePos);

		//m_pTransform->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * 1 * 0.1f);
	}
	if (GetKeyState(VK_RBUTTON) & 0x8001)
	{
		CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
		CToolView *pView = (CToolView *)pFrame->GetActiveView();
	
		GetCursorPos(&m_HoldMousePos);
		ScreenToClient(pView->m_hWnd, &m_HoldMousePos);
		
		_float testX = m_HoldMousePos.x - m_MousePos.x ;
		_float testY = m_HoldMousePos.y - m_MousePos.y ;

		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * testX * 0.001f);
		m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), fTimeDelta * testY * 0.001f);

		m_MousePos = m_HoldMousePos;
	}

	

	//if (GetKeyState('2') & 0x80)
	//{
	//	m_pTransform->Turn(_float3(0.f, 1.f, 0.f),fTimeDelta * 1 * 0.1f);
	//}
	//if (GetKeyState('2') & 0x80)
	//{
	//	m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), fTimeDelta * 1 * 0.1f);
	//}
	////_long		MouseMove = 0;
	//CToolView* pToolView = (CToolView *)AfxGetApp();
	//_long  MouseMove;
	//POINT  m_HoldMousePos;
	//GetCursorPos(&MousePos);
	//ScreenToClient(pToolView->m_hWnd, &MousePos);
	//
	//POINT CnterPos;
	//CnterPos.x = g_iWinCX * 0.5f + 300;
	//CnterPos.y = g_iWinCY * 0.5f;
	////POINT NomalMousePos()
	////CInput_Device::MMS_X
	//if (MouseMove = CnterPos.x - MousePos.x)
	//{
	//	m_pTransform->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * MouseMove * 0.000005f);
	//}

	//if (MouseMove = CnterPos.y - MousePos.y/*pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_Y)*/)
	//{
	//	m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * 0.000005f);
	//}

	RELEASE_INSTANCE(CGameInstance);

	return __super::Tick(fTimeDelta);		
}

_int CCamera_Dynamic::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	return _int();
}

HRESULT CCamera_Dynamic::Render()
{
	return S_OK;
}

HRESULT CCamera_Dynamic::BeforeRender()
{
	m_pGraphic_Device->Clear(0,
		nullptr,
		D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_ARGB(255, 0, 0, 255),	// 백버퍼 색상
		1.f, // z버퍼의 초기화 값
		0);	 // 스텐실 버퍼의 초기화 값


	return __super::BeforeRender();
}

HRESULT CCamera_Dynamic::AfterRender()
{
	return __super::AfterRender();
}

CCamera_Dynamic * CCamera_Dynamic::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Dynamic*	pInstance = new CCamera_Dynamic(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CCamera_Dynamic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Dynamic::Clone(void * pArg)
{
	CCamera_Dynamic*	pInstance = new CCamera_Dynamic(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CCamera_Dynamic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Dynamic::Free()
{
	__super::Free();



}

#include "stdafx.h"
#include "Camera_Dynamic.h"
#include "GameInstance.h"
#include "ToolView.h"

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

	
	if (GetKeyState(VK_UP) & 0x80)
	{
		m_pTransform->Go_Straight(fTimeDelta*0.1f);
	}

	if (GetKeyState(VK_DOWN) & 0x80)
	{
		m_pTransform->Go_BackWard(fTimeDelta*0.1f);
	}

	if (GetKeyState(VK_LEFT) & 0x80)
	{
		m_pTransform->Go_Left(fTimeDelta*0.1f);
	}

	if (GetKeyState(VK_RIGHT) & 0x80)
	{
		m_pTransform->Go_Right(fTimeDelta*0.1f);
	}
	if (GetKeyState('1') & 0x80)
	{
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f),fTimeDelta * 1 * 0.1f);
	}
	if (GetKeyState('2') & 0x80)
	{
		m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), fTimeDelta * 1 * 0.1f);
	}
	//_long		MouseMove = 0;
	CToolView* pToolView = (CToolView *)AfxGetApp();
	_long  MouseMove;
	POINT  MousePos;
	GetCursorPos(&MousePos);
	ScreenToClient(pToolView->m_hWnd, &MousePos);
	
	POINT CnterPos;
	CnterPos.x = g_iWinCX * 0.5f + 300;
	CnterPos.y = g_iWinCY * 0.5f;
	//POINT NomalMousePos()
	//CInput_Device::MMS_X
	if (MouseMove = CnterPos.x - MousePos.x)
	{
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * MouseMove * 0.000005f);
	}

	if (MouseMove = CnterPos.y - MousePos.y/*pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_Y)*/)
	{
		m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * 0.000005f);
	}

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

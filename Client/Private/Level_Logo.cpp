#include "stdafx.h"
#include "..\Public\Level_Logo.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Camera_Dynamic.h"
#include "LoadingLoader.h"
#include "UI.h"


CLevel_Logo::CLevel_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{

}

HRESULT CLevel_Logo::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("PORTAL_LOGO"));

	/* 현재 레벨이 생성될 때, 레벨에서 사용하고자하는 사본객체를 생성한다. */
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	return S_OK;
}

_int CLevel_Logo::Tick(_float fTimeDelta)
{
	if(0 > __super::Tick(fTimeDelta))
		return -1;

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	if (pInstance->Get_Key_Down(DIK_RETURN))
	{
		if (FAILED(pInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_STAGEONE))))
			return E_FAIL;

	}
	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

_int CLevel_Logo::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	return 0;
}

HRESULT CLevel_Logo::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* 사본객체를 생성ㅎ나다. */
	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CameraDesc));

	CameraDesc.vEye = _float3(0.f, 0.f, -100.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 300.f;

	CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	CameraDesc.iLevel = LEVEL_LOGO;


	if (FAILED(pGameInstance->Add_Camera_Object(CAM_STATIC, pLayerTag, &CameraDesc)))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CUI::UIDESC desc;
	ZeroMemory(&desc,sizeof(CUI::UIDESC));
	desc.WinCX = g_iWinCX;
	desc.WinCY = g_iWinCY;
	desc.PosX = g_iWinCX * 0.5f;
	desc.PosY = g_iWinCY * 0.5f;
	desc.SizeX = g_iWinCX;
	desc.SizeY = g_iWinCY;
	desc.Layer = 1;
	desc.Texture = TEXT("Prototype_Component_Texture_Logo");

	/* 사본객체를 생성ㅎ나다. */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, pLayerTag, PROTO_UI,&desc)))
		return E_FAIL;

	CUI::UIDESC desc2;
	ZeroMemory(&desc2, sizeof(desc2));
	desc2.WinCX = g_iWinCX;
	desc2.WinCY = g_iWinCY;
	desc2.Alpha = CUI::ALPHA_BLEND;
	desc2.PosX = g_iWinCX * 0.5f;
	desc2.PosY = g_iWinCY * 0.8f;
	desc2.SizeX = 640.f;
	desc2.SizeY = 360.f;
	desc2.Layer = 0;
	desc2.Texture = TEXT("Prototype_Component_Texture_Press");

	pGameInstance->Add_GameObject(LEVEL_LOGO, TEXT("Layer_Press_Start"), PROTO_UI, &desc2);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Logo*	pInstance = new CLevel_Logo(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "..\Public\Level_Logo.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "BackGround.h"
#include "Camera_Dynamic.h"

CLevel_Logo::CLevel_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{

}

HRESULT CLevel_Logo::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;


	if (FAILED(Ready_Prototype_GameObject()))
		return E_FAIL;

	

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

	return 0;
}

_int CLevel_Logo::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		if (FAILED(pGameInstance->OpenLevel(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, LEVEL_GAMEPLAY))))
			return E_FAIL;

		RELEASE_INSTANCE(CGameInstance);
	}

	return 0;
}

HRESULT CLevel_Logo::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("로고레벨임. "));	

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Prototype_GameObject()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* 원본객체 생성 */

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"), CBackGround::Create(m_pGraphic_Device))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* 사본객체를 생성ㅎ나다. */
	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CameraDesc));

	CameraDesc.vEye = _float3(0.f, 0.f, -50.f);
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


	CCamera::CAMERADESC		CameraDesc2;
	ZeroMemory(&CameraDesc2, sizeof(CameraDesc2));

	CameraDesc2.vEye = _float3(0.f, 0.f, -50.f);
	CameraDesc2.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc2.vAxisY = _float3(0.f, 1.f, 0.f);
			
	CameraDesc2.fFovy = D3DXToRadian(60.0f);
	CameraDesc2.fAspect = 1;
	CameraDesc2.fNear = 0.1f;
	CameraDesc2.fFar = 300.f;
			
	CameraDesc2.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc2.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	CameraDesc2.iLevel = LEVEL_LOGO;


	if (FAILED(pGameInstance->Add_Camera_Object(TEXT("Prototype_GameObject_Camera_Sub"), TEXT("Layer_Camera_Sub"), &CameraDesc2)))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* 사본객체를 생성ㅎ나다. */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_BackGround"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_BackGround"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_BackGround"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_BackGround"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_BackGround"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_BackGround"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_BackGround"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_BackGround"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_BackGround"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_BackGround"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOGO, pLayerTag, TEXT("Prototype_GameObject_BackGround"))))
		return E_FAIL;

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

#include "stdafx.h"
#include "..\Public\Level_Loading.h"
#include "Loader.h"
#include "GameInstance.h"
#include "Level_Ending.h"
#include "Level_StageOne.h"
#include "Level_StageTwo.h"
#include "UI.h"

CLevel_Loading::CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{

}

HRESULT CLevel_Loading::NativeConstruct(LEVEL eNextLevel)
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("PORTAL_LOADING"));

	m_eNextLevel = eNextLevel;

	Ready();

	return S_OK;
}

_int CLevel_Loading::Tick(_float fTimeDelta)
{
	if(!m_setting)
	{
		CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
		p_instance->SetMouseMode(true, nullptr);
		p_instance->StopAll();
		RELEASE_INSTANCE(CGameInstance);

		m_setting = true;
	}

	if(0 > __super::Tick(fTimeDelta))
		return -1;
	return 0;
}

_int CLevel_Loading::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (true == m_pLoader->isFinished())
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		CLevel* m_Level = nullptr;

		switch (m_eNextLevel)
		{
		case LEVEL_STAGEONE:
			g_CurrLevel = LEVEL_STAGEONE;
			m_Level = CLevel_StageOne::Create(m_pGraphic_Device);
			break;
		case LEVEL_STAGETWO:
			g_CurrLevel = LEVEL_STAGETWO;
			m_Level = CLevel_StageTwo::Create(m_pGraphic_Device);
			break;
		case LEVEL_ENDING:
			g_CurrLevel = LEVEL_ENDING;
			m_Level = CLevel_Ending::Create(m_pGraphic_Device);
			break;
		}
		if (FAILED(pGameInstance->OpenLevel(m_eNextLevel, m_Level,false)))
		{
			RELEASE_INSTANCE(CGameInstance);
			return E_FAIL;
		}

		RELEASE_INSTANCE(CGameInstance);
	}

	return 0;
}

HRESULT CLevel_Loading::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Loading::Ready()
{
	m_pLoader = CLoader::Create(m_pGraphic_Device, m_eNextLevel);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

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
	CameraDesc.iLevel = LEVEL_LOADING;

	if (FAILED(pGameInstance->Add_Camera_Object(CAM_STATIC, TEXT("Loading_Camera"), &CameraDesc)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	if (m_eNextLevel != LEVEL_ENDING)
	{
		CUI::UIDESC desc;
		ZeroMemory(&desc, sizeof(Engine::CUI::UIDESC));
		desc.WinCX = g_iWinCX;
		desc.WinCY = g_iWinCY;
		desc.PosX = g_iWinCX * 0.5f;
		desc.PosY = g_iWinCY * 0.5f;
		desc.SizeX = g_iWinCX;
		desc.SizeY = g_iWinCY;
		desc.Texture = TEXT("Prototype_Component_Texture_Loading");
		desc.FrameCount = 50;
		desc.AnimateSpeed = 100.f;
		desc.Style = CUI::STYLE_REPEAT;
		desc.Shader_Control = &g_ControlShader;
		desc.Shader_Style = SHADER_NONE;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOADING, TEXT("Loading"), PROTO_UI, &desc)))
		{
			RELEASE_INSTANCE(CGameInstance);
			return E_FAIL;
		}
	}
	else
	{
		CUI::UIDESC desc;
		ZeroMemory(&desc, sizeof(Engine::CUI::UIDESC));
		desc.WinCX = g_iWinCX;
		desc.WinCY = g_iWinCY;
		desc.PosX = g_iWinCX * 0.5f;
		desc.PosY = g_iWinCY * 0.5f;
		desc.SizeX = g_iWinCX;
		desc.SizeY = g_iWinCY;
		desc.Texture = TEXT("Prototype_Component_Texture_White");
		desc.FrameCount = 1;
		desc.Style = CUI::STYLE_FIX;
		desc.Shader_Control = &g_ControlShader;
		desc.Shader_Style = SHADER_NONE;

		if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOADING, TEXT("White"), PROTO_UI, &desc)))
		{
			RELEASE_INSTANCE(CGameInstance);
			return E_FAIL;
		}
	}

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(eNextLevel)))
	{
		MSGBOX("Failed to Creating CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);

}

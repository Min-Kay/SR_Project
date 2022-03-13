#include "stdafx.h"
#include "Level_Ending.h"

#include <UI.h>

#include "GameInstance.h"
CLevel_Ending::CLevel_Ending(LPDIRECT3DDEVICE9 m_pGraphic_Device)
	:CLevel(m_pGraphic_Device)
{
}

HRESULT CLevel_Ending::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("PORTAL_ENDING"));


	if (FAILED(Set_Camera()))
		return E_FAIL;

	if (FAILED(Set_UI()))
		return E_FAIL;

	g_ControlShader = 0.f;
	g_ControlTime = 1.f;

	return S_OK;
}

_int CLevel_Ending::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	m_Scroll->Set_Pos(m_Scroll->Get_Pos().x, m_Scroll->Get_Pos().y -
		fTimeDelta *50.f);

	return 0;
}

_int CLevel_Ending::LateTick(_float fTimeDelta)
{

	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	return 0;
}

HRESULT CLevel_Ending::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Ending::Set_Camera()
{
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
	CameraDesc.iLevel = LEVEL_ENDING;


	if (FAILED(pGameInstance->Add_Camera_Object(CAM_STATIC, MAIN_CAM, &CameraDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Ending::Set_UI()
{
	CUI::UIDESC desc;
	ZeroMemory(&desc,sizeof(&desc));
	desc.WinCX = g_iWinCX;
	desc.WinCY = g_iWinCY;
	desc.Shader_Style = SHADER_NONE;
	desc.Shader_Control = &g_ControlShader;
	desc.PosX = g_iWinCX * 0.5f;
	desc.PosY = g_iWinCY * 3.f;
	desc.SizeX = g_iWinCX;
	desc.SizeY = 1810.f;
	desc.Layer = 0;
	desc.Texture = TEXT("Prototype_Component_Texture_Ending");


	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);

	if (FAILED(p_instance->Add_GameObject(LEVEL_ENDING, TEXT("Ending_UI"), PROTO_UI, &desc)))
		return E_FAIL;

	m_Scroll = static_cast<CUI*>(p_instance->Get_GameObject_End(LEVEL_ENDING, TEXT("Ending_UI")));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_Ending* CLevel_Ending::Create(LPDIRECT3DDEVICE9 m_pGraphic_Device)
{
	CLevel_Ending* p_ending = new CLevel_Ending(m_pGraphic_Device);

	if(FAILED(p_ending->NativeConstruct()))
	{
		Safe_Release(p_ending);
		return nullptr;
	}
	return p_ending;
}

void CLevel_Ending::Free()
{
	__super::Free();
}

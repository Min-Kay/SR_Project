#include "stdafx.h"
#include "LoadingLoader.h"

#include "Camera_Dynamic.h"
#include "Camera_Player.h"
#include "GameInstance.h"
#include "UI.h"
#include "Camera_Static.h"
#include "Cam_Portal.h"


CLoadingLoader::CLoadingLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	m_pGraphic_Device = pGraphic_Device;
	Safe_AddRef(m_pGraphic_Device);
}

unsigned int APIENTRY ThreadFunctionLoading(void* pArg)
{
	/* CLoader객체안에 로딩에 필요한 데이터들을 담아놨는데 그걸 써야혀? */
	/* CLoader객체의 주소를 가지고있으면 좋을것같다. */
	CLoadingLoader* pLoader = (CLoadingLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CS());

	pLoader->LoadToLoading();

	LeaveCriticalSection(&pLoader->Get_CS());

	return 0;
}

HRESULT CLoadingLoader::NativeConstruct()
{
	InitializeCriticalSection(&m_CS);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunctionLoading, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoadingLoader::LoadToLoading()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	///* For.Prototype_Component_Texture_Loadingscreen */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Loading/Loading_Blue/Aperture_%d.png"),50))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Portal */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, PROTO_PORTAL, CVIBuffer_Portal::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, PROTO_CUBE, CVIBuffer_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, PROTO_COLOR, CVIBuffer_Color::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, PROTO_SHADER_RECT, CShader::Create(m_pGraphic_Device,TEXT("../../Resources/ShaderFiles/Shader_Rect.hlsl")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, PROTO_SHADER_CUBE, CShader::Create(m_pGraphic_Device, TEXT("../../Resources/ShaderFiles/Shader_Cube.hlsl")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Camera */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Camera"), CTexture::Create(m_pGraphic_Device, g_iWinCX, g_iWinCY))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Camera_Prototype(CAM_DYNAMIC, CCamera_Dynamic::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Camera_Prototype(CAM_PORTAL, CCam_Portal::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Camera_Prototype(CAM_PLAYER, CCamera_Player::Create(m_pGraphic_Device))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	
	m_isFinished = true;

	return S_OK;
}

CLoadingLoader* CLoadingLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLoadingLoader* pInstance = new CLoadingLoader(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CLoadingLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteObject(m_hThread);

	DeleteCriticalSection(&m_CS);

	Safe_Release(m_pGraphic_Device);
}

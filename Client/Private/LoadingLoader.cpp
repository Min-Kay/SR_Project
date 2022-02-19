#include "stdafx.h"
#include "LoadingLoader.h"
#include "GameInstance.h"

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
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading"), CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../../Resources/Textures/Portal/Loading/Loading_Blue/Aperture_%d.png"),100))))
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

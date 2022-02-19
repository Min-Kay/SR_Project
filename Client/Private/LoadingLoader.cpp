#include "stdafx.h"
#include "LoadingLoader.h"
#include "GameInstance.h"
#include "UI.h"
CLoadingLoader::CLoadingLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	m_pGraphic_Device = pGraphic_Device;
	Safe_AddRef(m_pGraphic_Device);
}

unsigned int APIENTRY ThreadFunctionLoading(void* pArg)
{
	/* CLoader��ü�ȿ� �ε��� �ʿ��� �����͵��� ��Ƴ��µ� �װ� �����? */
	/* CLoader��ü�� �ּҸ� ������������ �����Ͱ���. */
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

	CUI::UIDESC desc;
	ZeroMemory(&desc,sizeof(desc));

	desc.Alpha = CUI::ALPHA_BLEND;
	desc.PosX = g_iWinCX * 0.5f;
	desc.PosY = g_iWinCY * 0.8f;
	desc.SizeX = 640.f;
	desc.SizeY = 360.f;
	desc.Layer = 0;
	desc.Texture = TEXT("Prototype_Component_Texture_Press");

	pGameInstance->Add_GameObject(LEVEL_LOGO,TEXT("Layer_Press_Start"),PROTO_UI,&desc);

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

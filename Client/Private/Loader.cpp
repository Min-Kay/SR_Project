#include "stdafx.h"
#include "..\Public\Loader.h"

CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

unsigned int APIENTRY ThreadFunction(void* pArg)
{	
	/* CLoader객체안에 로딩에 필요한 데이터들을 담아놨는데 그걸 써야혀? */
	/* CLoader객체의 주소를 가지고있으면 좋을것같다. */
	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CS());

	switch (pLoader->Get_LevelID())
	{
	case LEVEL_GAMEPLAY:
		pLoader->Loading_ForGamePlay();
		break;
	case LEVEL_BOSS:
		pLoader->Loading_ForBoss();
		break;
	}	

	LeaveCriticalSection(&pLoader->Get_CS());	

	return 0;
}



HRESULT CLoader::NativeConstruct(LEVEL eNextLevel)
{
	InitializeCriticalSection(&m_CS);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunction, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	m_eLevel = eNextLevel;

	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlay()
{
	for (_uint i = 0; i < 999999999
		; ++i)
		int a = 10;

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForBoss()
{
	for (_uint i = 0; i < 9999999999; ++i)
		int a = 10;

	m_isFinished = true;
	
	return S_OK;
}

CLoader * CLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CLoader*	pInstance = new CLoader(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(eLevel)))
	{
		MSGBOX("Failed to Creating CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteObject(m_hThread);

	DeleteCriticalSection(&m_CS);

	Safe_Release(m_pGraphic_Device);


}

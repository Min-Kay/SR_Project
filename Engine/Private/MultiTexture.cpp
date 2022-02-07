#include "MultiTexture.h"
#include "GameInstance.h"

CMultiTexture::CMultiTexture()
	:CTexture()
{
}

const TEXINFO* CMultiTexture::Get_Texture(const TCHAR* pStateKey /*= L""*/, const int& iCnt /*= 0*/)
{
	auto	iter = find_if(m_mapMultiTex.begin(),
		m_mapMultiTex.end(),
		[&](auto& MyPair)->bool
	{
		if (pStateKey == MyPair.first)
			return true;

		return false;
	});

	if (iter == m_mapMultiTex.end())
		return nullptr;

	return iter->second[iCnt];
}

HRESULT CMultiTexture::InsertTexture(const TCHAR* pFilePath, 
	const TCHAR* pStateKey /*= L""*/, 
	const int& iCnt /*= 0*/)
{
	// 우리가 지정하는 경로의 최대 범위는 MAX_PATH크기 내에서 가능하다.
	TCHAR		szFullPath[MAX_PATH] = L"";

	for (int i = 0; i < iCnt; ++i)
	{
		wsprintf(szFullPath, pFilePath, i);

		TEXINFO*	pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));
				
		if (FAILED(D3DXGetImageInfoFromFile(szFullPath, &(pTexInfo->tImgInfo))))
		{
			MSGBOX("szFullPath InsertTexture Error");
			Safe_Delete(pTexInfo);
			return E_FAIL;
		}

		CGameInstance* pInstance = CGameInstance::GetInstance();
		Safe_AddRef(pInstance);

		if (FAILED(D3DXCreateTextureFromFileEx(pInstance->Get_Device(),
			szFullPath,
			pTexInfo->tImgInfo.Width,
			pTexInfo->tImgInfo.Height,
			pTexInfo->tImgInfo.MipLevels,
			0, // D3DUSAGE_RENDERTARGET 화면 전체출력용 텍스처 생성 옵션
			pTexInfo->tImgInfo.Format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,  // 이미지 필터링 방식 : 픽셀의 확대축소 시 어떻게 처리할 것인지 묻는 인자
			D3DX_DEFAULT,  // 밉맵을 이용한 확대 축소 시 필터링을 어떻게 할지 지정
			0,				// colorkey : 제거할 색상, 0을 주면 컬러키 무효화
			nullptr,		// m_pTexInfo->tImgInfo 이미지 정보 기록
			nullptr,		// 팔레트를 처리할 것인지 지정(8비트 시절 ARGB 값을 표현하기 위해 만든 구조체)
			&(pTexInfo->pTexture))))
		{
			MSGBOX("MultiTexture Create Failed");
			Safe_Delete(pTexInfo);
			return E_FAIL;
		}
		
		Safe_Release(pInstance);

		m_mapMultiTex[pStateKey].push_back(pTexInfo);
	}

	return S_OK;
}

CMultiTexture* CMultiTexture::Create()
{
	CMultiTexture* pInstance = new CMultiTexture;
	return pInstance;
}

void CMultiTexture::Free()
{
	__super::Free();

	for_each(m_mapMultiTex.begin(), m_mapMultiTex.end(),
		[](auto& MyPair)
		{
			for_each(MyPair.second.begin(), MyPair.second.end(), Safe_Delete<TEXINFO*>);
			MyPair.second.clear();
		});

	m_mapMultiTex.clear();
}

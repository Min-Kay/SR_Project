#include "SingleTexture.h"
#include "GameInstance.h"
CSingleTexture::CSingleTexture()
	:CTexture()
{
}

HRESULT CSingleTexture::InsertTexture(const TCHAR* pFilePath, const TCHAR* pStateKey /*= L""*/, const int& iCnt /*= 0*/)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	// 지정한 이미지 파일에 관한 정보를 얻어와 이미지 구조체에 값을 기록하는 함수
	if (FAILED(D3DXGetImageInfoFromFile(pFilePath, &(m_pTexInfo->tImgInfo))))
	{
		MSGBOX("TexInfo Insert Failed ");
		return E_FAIL;
	}

	// D3DPOOL_DEFAULT = 0, : 가장 적합한 메모리에 보관한다는 뜻, 동적 버퍼를 사용할 경우 사용가능한 옵션
	// D3DPOOL_MANAGED = 1, : direct3d에 의해 자원을 관리하며 그래픽 메모리를 사용하지만 이를 ram에 백업하는 옵션
	// D3DPOOL_SYSTEMMEM = 2, : ram을 사용하겠다는 옵션
	// D3DPOOL_SCRATCH : 시스템 메모리를 사용하지만 dx 장치로부터 접근할 수 없다.
	
	CGameInstance* pInstance = CGameInstance::GetInstance();
	Safe_AddRef(pInstance);

	if (FAILED(D3DXCreateTextureFromFileEx(pInstance->Get_Device(),
		pFilePath, 
		m_pTexInfo->tImgInfo.Width, 
		m_pTexInfo->tImgInfo.Height, 
		m_pTexInfo->tImgInfo.MipLevels,
		0, // D3DUSAGE_RENDERTARGET 화면 전체출력용 텍스처 생성 옵션
		m_pTexInfo->tImgInfo.Format, 
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,  // 이미지 필터링 방식 : 픽셀의 확대축소 시 어떻게 처리할 것인지 묻는 인자
		D3DX_DEFAULT,  // 밉맵을 이용한 확대 축소 시 필터링을 어떻게 할지 지정
		0,				// colorkey : 제거할 색상, 0을 주면 컬러키 무효화
		nullptr,		// m_pTexInfo->tImgInfo 이미지 정보 기록
		nullptr,		// 팔레트를 처리할 것인지 지정(8비트 시절 ARGB 값을 표현하기 위해 만든 구조체)
		&(m_pTexInfo->pTexture))))
	{
		MSGBOX("SingleTexture Create Failed");
		return E_FAIL;
	}

	Safe_Release(pInstance);

	return S_OK;
}

CSingleTexture* CSingleTexture::Create()
{
	CSingleTexture* pInstance = new CSingleTexture;
	return pInstance;
}

void CSingleTexture::Free()
{
	__super::Free();
	Safe_Release(m_pTexInfo->pTexture);
	Safe_Delete(m_pTexInfo);
}

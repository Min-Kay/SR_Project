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

	// ������ �̹��� ���Ͽ� ���� ������ ���� �̹��� ����ü�� ���� ����ϴ� �Լ�
	if (FAILED(D3DXGetImageInfoFromFile(pFilePath, &(m_pTexInfo->tImgInfo))))
	{
		MSGBOX("TexInfo Insert Failed ");
		return E_FAIL;
	}

	// D3DPOOL_DEFAULT = 0, : ���� ������ �޸𸮿� �����Ѵٴ� ��, ���� ���۸� ����� ��� ��밡���� �ɼ�
	// D3DPOOL_MANAGED = 1, : direct3d�� ���� �ڿ��� �����ϸ� �׷��� �޸𸮸� ��������� �̸� ram�� ����ϴ� �ɼ�
	// D3DPOOL_SYSTEMMEM = 2, : ram�� ����ϰڴٴ� �ɼ�
	// D3DPOOL_SCRATCH : �ý��� �޸𸮸� ��������� dx ��ġ�κ��� ������ �� ����.
	
	CGameInstance* pInstance = CGameInstance::GetInstance();
	Safe_AddRef(pInstance);

	if (FAILED(D3DXCreateTextureFromFileEx(pInstance->Get_Device(),
		pFilePath, 
		m_pTexInfo->tImgInfo.Width, 
		m_pTexInfo->tImgInfo.Height, 
		m_pTexInfo->tImgInfo.MipLevels,
		0, // D3DUSAGE_RENDERTARGET ȭ�� ��ü��¿� �ؽ�ó ���� �ɼ�
		m_pTexInfo->tImgInfo.Format, 
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,  // �̹��� ���͸� ��� : �ȼ��� Ȯ����� �� ��� ó���� ������ ���� ����
		D3DX_DEFAULT,  // �Ӹ��� �̿��� Ȯ�� ��� �� ���͸��� ��� ���� ����
		0,				// colorkey : ������ ����, 0�� �ָ� �÷�Ű ��ȿȭ
		nullptr,		// m_pTexInfo->tImgInfo �̹��� ���� ���
		nullptr,		// �ȷ�Ʈ�� ó���� ������ ����(8��Ʈ ���� ARGB ���� ǥ���ϱ� ���� ���� ����ü)
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

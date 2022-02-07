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
	// �츮�� �����ϴ� ����� �ִ� ������ MAX_PATHũ�� ������ �����ϴ�.
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
			0, // D3DUSAGE_RENDERTARGET ȭ�� ��ü��¿� �ؽ�ó ���� �ɼ�
			pTexInfo->tImgInfo.Format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,  // �̹��� ���͸� ��� : �ȼ��� Ȯ����� �� ��� ó���� ������ ���� ����
			D3DX_DEFAULT,  // �Ӹ��� �̿��� Ȯ�� ��� �� ���͸��� ��� ���� ����
			0,				// colorkey : ������ ����, 0�� �ָ� �÷�Ű ��ȿȭ
			nullptr,		// m_pTexInfo->tImgInfo �̹��� ���� ���
			nullptr,		// �ȷ�Ʈ�� ó���� ������ ����(8��Ʈ ���� ARGB ���� ǥ���ϱ� ���� ���� ����ü)
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

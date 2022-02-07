#include "TextureMgr.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
	:CBase()
{
}

const TEXINFO * CTextureMgr::Get_Texture(const TCHAR* pObjKey, const TCHAR * pStateKey, const int & iCnt)
{
	auto	iter = find_if(m_mapTexture.begin(),
		m_mapTexture.end(),
		[&](auto& MyPair)->bool
	{
		if (pObjKey == MyPair.first)
			return true;

		return false;
	});

	if (iter == m_mapTexture.end())
		return nullptr;


	return iter->second->Get_Texture(pStateKey, iCnt);
}

HRESULT CTextureMgr::InsertTexture(TEXTYPE eType, const TCHAR * pFilePath, const TCHAR * pObjKey, const TCHAR * pStateKey, const int & iCnt)
{
	auto	iter = find_if(m_mapTexture.begin(),
						   m_mapTexture.end(),
							[&](auto& MyPair)->bool
						{
							if (pObjKey == MyPair.first)
								return true;

							return false;
						});

	if (iter == m_mapTexture.end())
	{
		CTexture*		pTexture = nullptr;

		switch (eType)
		{
		case TEX_SINGLE:
			pTexture = CSingleTexture::Create();
			break;

		case TEX_MULTI:
			pTexture = CMultiTexture::Create();
			break;
		}

		if (FAILED(pTexture->InsertTexture(pFilePath, pStateKey, iCnt)))
		{
			MSGBOX("Texture Insert Error");
			return E_FAIL;
		}

		m_mapTexture.emplace(pObjKey, pTexture);
	}
	else if (TEX_MULTI == eType)
		iter->second->InsertTexture(pFilePath, pStateKey, iCnt);

	return S_OK;
}

void CTextureMgr::Free()
{
	for_each(m_mapTexture.begin(), m_mapTexture.end(),
		[](auto& MyPair)
		{
			Safe_Release (MyPair.second);
		});

	m_mapTexture.clear();
}

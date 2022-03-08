#include "..\Public\Texture.h"
#include "Shader.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CTexture::CTexture(const CTexture & rhs)
	: CComponent(rhs)
	, m_Textures(rhs.m_Textures)
{
	for (auto& pTexture : m_Textures)
		Safe_AddRef(pTexture);

}

HRESULT CTexture::NativeConstruct_Prototype(TYPE eType, const _tchar * pTextureFilePath, _uint iNumTextures)
{
	_tchar		szFullPath[MAX_PATH] = TEXT("");

	m_Textures.reserve(iNumTextures);

	for (_uint i = 0; i < iNumTextures; ++i)
	{
		wsprintf(szFullPath, pTextureFilePath, i);

		LPDIRECT3DBASETEXTURE9		pTexture = nullptr;
		HRESULT		hr = 0;

		switch (eType)
		{	
		case Engine::CTexture::TYPE_DEFAULT:
			hr = D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, (LPDIRECT3DTEXTURE9*)&pTexture);
			if (FAILED(hr))
				return E_FAIL;
			break;
		case Engine::CTexture::TYPE_CUBEMAP:
			hr = D3DXCreateCubeTextureFromFile(m_pGraphic_Device, szFullPath, (LPDIRECT3DCUBETEXTURE9*)&pTexture);
			break;
		}
		if (FAILED(hr))
			return E_FAIL;
		m_Textures.push_back(pTexture); 	
		
	}

	

	return S_OK;
}

HRESULT CTexture::NativeConstruct_Prototype(_uint iWidth, _uint iHeight)
{
	LPDIRECT3DBASETEXTURE9		pTexture = nullptr;
	
	//if (FAILED(m_pGraphic_Device->CreateTexture(iWidth, iHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, (LPDIRECT3DTEXTURE9*) & pTexture, NULL)))
	//	return E_FAIL;

	//m_Textures.push_back(pTexture);

	return S_OK;
}

HRESULT CTexture::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CTexture::Bind_OnGraphicDevice(_uint iTextureIndex)
{
	if (iTextureIndex >= m_Textures.size())
		return E_FAIL;

	return m_pGraphic_Device->SetTexture(0, m_Textures[iTextureIndex]);	
}

LPDIRECT3DBASETEXTURE9* CTexture::GetTexture(_uint iIndex)
{
	if (iIndex >= m_Textures.size())
		return nullptr; 

	return &(m_Textures[iIndex]);
}

HRESULT CTexture::Add_Texture(_uint iWidth, _uint iHeight)
{
	LPDIRECT3DBASETEXTURE9		pTexture = nullptr;
	
	if (FAILED(m_pGraphic_Device->CreateTexture(iWidth, iHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, (LPDIRECT3DTEXTURE9*)&pTexture, NULL)))
		return E_FAIL;

	m_Textures.push_back(pTexture);

	return S_OK;
}

const _uint CTexture::Get_Textures_Count() const
{
	return (_uint)m_Textures.size();
}


HRESULT CTexture::Bind_OnShader(CShader* pShader, D3DXHANDLE hParameter, _uint iTextureIndex)
{
	if (iTextureIndex >= m_Textures.size())
		return E_FAIL;

	return pShader->SetUp_TextureOnShader(hParameter, m_Textures[iTextureIndex]);
}


CTexture * CTexture::Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eType, const _tchar * pTextureFilePath, _uint iNumTextures)
{
	CTexture*	pInstance = new CTexture(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(eType, pTextureFilePath, iNumTextures)))
	{
		MSGBOX("Failed to Creating CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iWidth, _uint iHeight)
{
	CTexture* pInstance = new CTexture(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(iWidth, iHeight)))
	{
		MSGBOX("Failed to Creating CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTexture::Clone(void * pArg)
{
	CTexture*	pInstance = new CTexture(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto& pTexture : m_Textures)
		Safe_Release(pTexture);

	m_Textures.clear();
}

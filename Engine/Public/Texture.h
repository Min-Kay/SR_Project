#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
public:
	enum TYPE { TYPE_DEFAULT, TYPE_CUBEMAP, TYPE_END };
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(TYPE eType, const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual HRESULT NativeConstruct_Prototype(_uint iWidth, _uint iHeight);

	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	HRESULT Bind_OnGraphicDevice(_uint iTextureIndex = 0);
	LPDIRECT3DBASETEXTURE9* GetTexture();

private:
	vector<LPDIRECT3DBASETEXTURE9>			m_Textures;
	typedef vector<LPDIRECT3DBASETEXTURE9>	TEXTURES;

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eType, const _tchar* pTextureFilePath, _uint iNumTextures = 1);
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iWidth, _uint iHeight);

	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
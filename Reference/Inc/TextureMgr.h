#pragma once

#include "SingleTexture.h"
#include "MultiTexture.h"

BEGIN(Engine)
class CTextureMgr : public CBase
{
	DECLARE_SINGLETON(CTextureMgr)

private:
	CTextureMgr();
	virtual ~CTextureMgr() = default;

public:
	enum TEXTYPE { TEX_SINGLE, TEX_MULTI, TEX_END };

public:
	HRESULT Set_Device(LPDIRECT3DDEVICE9* m_device);

public:
	const TEXINFO*		Get_Texture(const TCHAR* pObjKey, const TCHAR* pStateKey = L"", const int& iCnt = 0);

public:
	HRESULT				InsertTexture(TEXTYPE eType, const TCHAR* pFilePath, const TCHAR* pObjKey, const TCHAR* pStateKey = L"", const int& iCnt = 0); 

private:
	map<wstring, CTexture*>			m_mapTexture;
	LPDIRECT3DDEVICE9	m_device = nullptr;

	
public:

	virtual void Free() override;

};
END


#pragma once

#include "Texture.h"

class CMultiTexture :	public CTexture
{
protected:
	CMultiTexture();
	virtual ~CMultiTexture() = default;

public:
	virtual const TEXINFO* Get_Texture(const TCHAR* pStateKey = L"", const int& iCnt = 0) override;
	virtual HRESULT InsertTexture(const TCHAR* pFilePath, const TCHAR* pStateKey = L"", const int& iCnt = 0) override;

private:
	map<wstring, vector<TEXINFO*>>			m_mapMultiTex;


public:
	static CMultiTexture* Create();
	virtual void Free() override;

};


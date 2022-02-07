#pragma once
#include "Texture.h"
class CSingleTexture :	public CTexture
{
protected:
	CSingleTexture();
	virtual ~CSingleTexture() = default;

public:
	virtual const TEXINFO*		Get_Texture(const TCHAR* pStateKey = L"", const int& iCnt = 0) { return m_pTexInfo; }
	virtual HRESULT InsertTexture(const TCHAR* pFilePath, const TCHAR* pStateKey = L"", const int& iCnt = 0) override;

private:
	TEXINFO*			m_pTexInfo = nullptr;


public:
	static CSingleTexture* Create();
	virtual void Free() override;

};


#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Color final : public CVIBuffer
{
protected:
	explicit CVIBuffer_Color(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Color(const CVIBuffer_Color& rhs);
	virtual ~CVIBuffer_Color() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

	HRESULT ChangeColor(DWORD color);

private:
	DWORD m_lCOLOR;
	
public:
	static CVIBuffer_Color* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END
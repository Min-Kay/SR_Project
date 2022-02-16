#pragma once
#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Portal :
    public CVIBuffer
{

protected:
	explicit CVIBuffer_Portal(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Portal(const CVIBuffer_Portal& rhs);
	virtual ~CVIBuffer_Portal() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	virtual HRESULT Render();
public:
    static CVIBuffer_Portal* Create(LPDIRECT3DDEVICE9 pGraphicDevice);
    virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};
END
#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final:
    public CVIBuffer
{
protected:
	explicit CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

private:
	_uint RectCount = 1000;
	_uint vertexCount = 0; 

public:
	static CVIBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual CComponent* Clone(void* pArg) override; 
    virtual void Free() override;
};
END


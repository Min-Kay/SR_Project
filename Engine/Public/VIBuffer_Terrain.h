#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
protected:
	explicit CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ);
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pHeightMapFilePath);
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	_float Compute_Y(const _float3& vPoint);

private:
	_uint			m_iNumVerticesX = 0;
	_uint			m_iNumVerticesZ = 0;

public:
	static CVIBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ);
	static CVIBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pHeightMapFilePath);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END
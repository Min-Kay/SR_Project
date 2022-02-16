#include "VIBuffer_Portal.h"

CVIBuffer_Portal::CVIBuffer_Portal(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Portal::CVIBuffer_Portal(const CVIBuffer_Portal& rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Portal::NativeConstruct_Prototype()
{
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 362;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_ePrimitiveType = D3DPT_TRIANGLEFAN;
	m_iNumPrimitive = 360;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXTEX* pVertices = nullptr;

	_float radiusX = 0.3f;
	_float radiusY = 0.4f;
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(0.f, 0.f, 0.f);
	pVertices[0].vTexUV = _float2(0.5f, 0.5f);

	for (int i = 0; i < 361; ++i)
	{
		pVertices[i].vPosition = _float3(radiusX * cosf(D3DXToRadian(-i)), radiusY * sinf(D3DXToRadian(-i)), 0.f);
		pVertices[i].vTexUV = _float2((cosf(D3DXToRadian(i)) - 1.f) * 0.5f, (sinf(D3DXToRadian(i)) - 1.f) * 0.5f);
	}

	/*pVertices[1].vPosition = _float3(-radiusX * cosf(D3DXToRadian(45.f)), radiusY * sinf(D3DXToRadian(45.f)), 0.f);
	pVertices[1].vTexUV = _float2(0.25f, 0.25f);

	pVertices[2].vPosition = _float3(-radiusX * cosf(D3DXToRadian(67.5f)), radiusY * sinf(D3DXToRadian(67.5f)), 0.f);
	pVertices[2].vTexUV = _float2(0.375f, 0.125f);

	pVertices[3].vPosition = _float3(0.f, radiusY, 0.f);
	pVertices[3].vTexUV = _float2(0.5f, 0.f);

	pVertices[4].vPosition = _float3(radiusX * cosf(D3DXToRadian(67.5f)), radiusY * sinf(D3DXToRadian(67.5f)), 0.f);
	pVertices[4].vTexUV = _float2(0.625f, 0.125f);

	pVertices[5].vPosition = _float3(radiusX * cosf(D3DXToRadian(45.f)), radiusY * sinf(D3DXToRadian(45.f)), 0.f);
	pVertices[5].vTexUV = _float2(0.75f, 0.25f);

	pVertices[6].vPosition = _float3(radiusX * cosf(D3DXToRadian(22.5f)), radiusY * sinf(D3DXToRadian(22.5f)), 0.f);
	pVertices[6].vTexUV = _float2(0.875f, 0.375f);

	pVertices[7].vPosition = _float3(radiusX, 0, 0.f);
	pVertices[7].vTexUV = _float2(1.f, 0.5f);

	pVertices[8].vPosition = _float3(radiusX * cosf(D3DXToRadian(22.5f)), -radiusY * sinf(D3DXToRadian(22.5f)), 0.f);
	pVertices[8].vTexUV = _float2(0.875f, 0.625f);

	pVertices[9].vPosition = _float3(radiusX * cosf(D3DXToRadian(45.f)), -radiusY * sinf(D3DXToRadian(45.f)), 0.f);
	pVertices[9].vTexUV = _float2(0.75f, 0.75f);

	pVertices[10].vPosition = _float3(radiusX * cosf(D3DXToRadian(67.5f)), -radiusY * sinf(D3DXToRadian(67.5f)), 0.f);
	pVertices[10].vTexUV = _float2(0.625f, 0.875f);

	pVertices[11].vPosition = _float3(0, -radiusY, 0.f);
	pVertices[11].vTexUV = _float2(0.5f, 1.f);

	pVertices[12].vPosition = _float3(-radiusX * cosf(D3DXToRadian(67.5f)), -radiusY * sinf(D3DXToRadian(67.5f)), 0.f);
	pVertices[12].vTexUV = _float2(0.375f, 0.875f);

	pVertices[13].vPosition = _float3(-radiusX * cosf(D3DXToRadian(45.f)), -radiusY * sinf(D3DXToRadian(45.f)), 0.f);
	pVertices[13].vTexUV = _float2(0.25f, 0.75f);

	pVertices[14].vPosition = _float3(-radiusX * cosf(D3DXToRadian(22.5f)), -radiusY * sinf(D3DXToRadian(22.5f)), 0.f);
	pVertices[14].vTexUV = _float2(0.125f, 0.625f);

	pVertices[15].vPosition = _float3(-radiusX, 0.f, 0.f);
	pVertices[15].vTexUV = _float2(0.f, 0.5f);

	pVertices[16].vPosition = _float3(-radiusX * cosf(D3DXToRadian(22.5f)), radiusY * sinf(D3DXToRadian(22.5f)), 0.f);
	pVertices[16].vTexUV = _float2(0.125f, 0.375f);

	pVertices[17].vPosition = _float3(-radiusX * cosf(D3DXToRadian(45.f)), radiusY * sinf(D3DXToRadian(45.f)), 0.f);
	pVertices[17].vTexUV = _float2(0.25f, 0.25f);*/
	
	m_pVB->Unlock();

	//m_iIndicesSize = sizeof(FACEINDICES16);
	//m_eIndexFormat = D3DFMT_INDEX16;
	//if (FAILED(__super::Create_IndexBuffer()))
	//	return E_FAIL;

	//FACEINDICES16* pIndices = nullptr;

	//m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	//pIndices[0]._0 = 0;
	//pIndices[0]._1 = 1;
	//pIndices[0]._2 = 2;

	//pIndices[1]._0 = 0;
	//pIndices[1]._1 = 2;
	//pIndices[1]._2 = 3;

	//m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Portal::NativeConstruct(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Portal::Render()
{
	if (nullptr == m_pVB)
		return E_FAIL;

	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride);
	//m_pGraphic_Device->SetIndices(m_pIB);
	m_pGraphic_Device->SetFVF(m_dwFVF);

	m_pGraphic_Device->DrawPrimitive(m_ePrimitiveType, 0, m_iNumPrimitive);
	//m_pGraphic_Device->DrawIndexedPrimitive(m_ePrimitiveType, 0, 0, m_iNumVertices, 0, m_iNumPrimitive);
}

CVIBuffer_Portal* CVIBuffer_Portal::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	CVIBuffer_Portal* pInstance = new CVIBuffer_Portal(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CComponent* CVIBuffer_Portal::Clone(void* pArg)
{
	CVIBuffer_Portal* pInstance = new CVIBuffer_Portal(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CVIBuffer_Portal::Free()
{
	__super::Free();
}

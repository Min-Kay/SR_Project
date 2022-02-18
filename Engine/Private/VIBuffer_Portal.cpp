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

	m_pVB->Unlock();

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

	m_pGraphic_Device->SetFVF(m_dwFVF);

	m_pGraphic_Device->DrawPrimitive(m_ePrimitiveType, 0, m_iNumPrimitive);

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

#include "..\Public\VIBuffer.h"

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_iStride(rhs.m_iStride)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_dwFVF(rhs.m_dwFVF)
	, m_ePrimitiveType(rhs.m_ePrimitiveType)
	, m_iNumPrimitive(rhs.m_iNumPrimitive)
	, m_iIndicesSize(rhs.m_iIndicesSize)
	, m_eIndexFormat(rhs.m_eIndexFormat)
{
	Safe_AddRef(m_pIB);
	Safe_AddRef(m_pVB);
}

HRESULT CVIBuffer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	if (nullptr == m_pVB)
		return E_FAIL;

	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride);
	m_pGraphic_Device->SetIndices(m_pIB);
	m_pGraphic_Device->SetFVF(m_dwFVF);

	// m_pGraphic_Device->DrawPrimitive(m_ePrimitiveType, 0, m_iNumPrimitive);
	m_pGraphic_Device->DrawIndexedPrimitive(m_ePrimitiveType, 0, 0, m_iNumVertices, 0, m_iNumPrimitive);

	return S_OK;
}

HRESULT CVIBuffer::Create_VertexBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	
	/* 정점들을 담는 배열 공간을 할당한다. */
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iStride * m_iNumVertices, 0 /*D3DUSAGE_DYNAMIC*/, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer::Create_IndexBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/* 정점들을 담는 배열 공간을 할당한다. */
	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndicesSize * m_iNumPrimitive, 0 /*D3DUSAGE_DYNAMIC*/, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer::Free()
{
	__super::Free();

	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

}

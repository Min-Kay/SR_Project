#include "VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs)
    :CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype()
{
	vertexCount = RectCount + 1;

	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = vertexCount * vertexCount;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = (RectCount * RectCount * 2);

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXTEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (int i = 0; i < vertexCount; ++i)
	{
		for (int j = 0; j < vertexCount; ++j)
		{
			pVertices[j + i * vertexCount].vPosition = _float3(1.0f * j, 1.0f * i, 0.f);
			pVertices[j + i * vertexCount].vTexUV = _float2(_float(j) / (vertexCount - 1), _float(i)/ (vertexCount-1));
		}
	}

	m_pVB->Unlock();

	m_iIndicesSize = sizeof(FACEINDICES32);
	m_eIndexFormat = D3DFMT_INDEX32;
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	FACEINDICES32* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);
	
	for (int i = 0; i < RectCount; ++i)
	{
		for (int j = 0; j < RectCount; ++j)
		{
			_uint index = (j + i * RectCount) * 2;
			_uint vertex = j + i * vertexCount;
			pIndices[index]._0 = vertex + vertexCount;
			pIndices[index]._1 = vertex + vertexCount + 1;
			pIndices[index]._2 = vertex + 1;
							 
			pIndices[index + 1]._0 = vertex + vertexCount;
			pIndices[index + 1]._1 = vertex + 1;
			pIndices[index + 1]._2 = vertex;
		}
	}

	m_pIB->Unlock();

    return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct(void* pArg)
{
    return S_OK;
}

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Creating CVIBuffer_Rect");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CVIBuffer_Terrain::Clone(void* pArg)
{
    CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Creating CVIBuffer_Terrain");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();
}

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
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 121;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 200;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXTEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (int i = 0; i < 11; ++i)
	{
		for (int j = 0; j < 11; ++j)
		{
			pVertices[j + i * 11].vPosition = _float3(10.0f * j, 10.0f * i, 0.f);
			pVertices[j + i * 11].vTexUV = _float2(0.f,0.f);
		}
	}

	m_pVB->Unlock();

	m_iIndicesSize = sizeof(FACEINDICES16);
	m_eIndexFormat = D3DFMT_INDEX16;
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	FACEINDICES16* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);
	
	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			_uint index = (j + i * 10) * 2;
			_uint vertex = j + i * 11;
			pIndices[index]._0 = vertex + 11;
			pIndices[index]._1 = vertex + 12;
			pIndices[index]._2 = vertex + 1;
							 
			pIndices[index + 1]._0 = vertex + 11;
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

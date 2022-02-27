#include "..\Public\VIBuffer_Color.h"

CVIBuffer_Color::CVIBuffer_Color(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Color::CVIBuffer_Color(const CVIBuffer_Color & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Color::NativeConstruct_Prototype()
{
	
	m_iStride = sizeof(VTXCOLOR);
	m_iNumVertices = 4;
	m_dwFVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE /*| D3DFVF_TEX1*/  ) ;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 2;
	m_lCOLOR = D3DXCOLOR(0.5f,0.5f,0.5f,0.5f);
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	m_pVertices = new VTXCOLOR[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXCOLOR) * m_iNumVertices);

	VTXCOLOR*			pVertices = nullptr;
	
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vColor = m_lCOLOR;
	//pVertices[0].vTexUV = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vColor = m_lCOLOR;
	//pVertices[1].vTexUV = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vColor = m_lCOLOR;
	//pVertices[2].vTexUV = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vColor = m_lCOLOR;
	//pVertices[3].vTexUV = _float2(0.f, 1.f);

	memcpy(m_pVertices, pVertices, sizeof(VTXCOLOR) * m_iNumVertices);


	m_pVB->Unlock();

	m_iIndicesSize = sizeof(FACEINDICES16);
	m_eIndexFormat = D3DFMT_INDEX16;
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	m_pIndices = new FACEINDICES16[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);
	FACEINDICES16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;

	m_pIB->Unlock();


	memcpy(m_pIndices, pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	return S_OK;
}

HRESULT CVIBuffer_Color::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Color::ChangeColor(DWORD color)
{

	VTXCOLOR*		pVertices = (VTXCOLOR*)m_pVertices;

	m_lCOLOR = color;
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices->vColor = m_lCOLOR;
	memcpy(m_pVertices, pVertices, sizeof(VTXCOLOR) * m_iNumVertices);


	m_pVB->Unlock();
	return S_OK;
}

CVIBuffer_Color * CVIBuffer_Color::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Color*	pInstance = new CVIBuffer_Color(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CVIBuffer_Color");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Color::Clone(void * pArg)
{
	CVIBuffer_Color*	pInstance = new CVIBuffer_Color(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CVIBuffer_Color");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Color::Free()
{
	__super::Free();

}

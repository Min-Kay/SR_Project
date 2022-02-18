#include "..\Public\VIBuffer.h"
#include "Picking.h"

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
	, m_pVertices(rhs.m_pVertices)
	, m_pIndices(rhs.m_pIndices)
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

_bool CVIBuffer::Pick(_uint _type, const _float4x4& WorldMatrixInverse, _float3 * pOut)
{	
	CPicking*		pPicking = GET_INSTANCE(CPicking);

	pPicking->Transform_ToLocalSpace((CPicking::TYPE)_type, WorldMatrixInverse);

	_uint		iIndexByte = 0;

	if (m_eIndexFormat == D3DFMT_INDEX16)
		iIndexByte = 2;
	else
		iIndexByte = 4;	

	_bool		isPick = false;

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		_uint	iIndices[3] = { 0 };

		/* void*를 _byte*로 캐스팅한 이유 : 주소에 상수를 더했을때 그 상수바이트크기만큼ㅂ 이동할 수 있도록 만들어주기위해. */
		/* _short*포인터형변수에 1을더하면 2byte씩 이동하낟.  */
		/* _uint*포인터형변수에 1을더하면 4byte씩 이동하낟.  */
		/* _byte*포인터형변수에 1을더하면 1byte씩 이동하낟.  */

		/* 129, 130, 1 */
		/* 129, 1, 0 */

		for (_uint j = 0; j < 3; ++j)					
			memcpy(&iIndices[j], (((_byte*)m_pIndices + m_iIndicesSize * i) + iIndexByte * j), iIndexByte);

		_float3		vPoint[3] = {
			*(_float3*)(((_byte*)m_pVertices) + m_iStride * iIndices[0]), 
			*(_float3*)(((_byte*)m_pVertices) + m_iStride * iIndices[1]),
			*(_float3*)(((_byte*)m_pVertices) + m_iStride * iIndices[2])
		};

		if (isPick = pPicking->isPick((CPicking::TYPE)_type, vPoint, pOut))
		{
			_float4x4		WorldMatrix;
			D3DXMatrixInverse(&WorldMatrix, nullptr, &WorldMatrixInverse);
			D3DXVec3TransformCoord(pOut, pOut, &WorldMatrix);

			break;
		}
	}

	RELEASE_INSTANCE(CPicking);

	return isPick;
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

	if(false == m_isCloned)
	{
		Safe_Delete_Array(m_pVertices);
		Safe_Delete_Array(m_pIndices);
	}

	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

}

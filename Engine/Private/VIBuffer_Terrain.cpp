#include "..\Public\VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{

}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
{

}

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ)
{
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = iNumVerticesX * iNumVerticesZ;
	m_iNumVerticesX = iNumVerticesX;
	m_iNumVerticesZ = iNumVerticesZ;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = (iNumVerticesX - 1) * (iNumVerticesZ - 1) * 2;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXTEX*			pVertices = nullptr;

	m_pVertices = new VTXTEX[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXTEX) * m_iNumVertices);

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _float3(j, 0.0f, i);
			pVertices[iIndex].vTexUV = _float2((_float)j / (m_iNumVerticesX - 1) * 20.f, (_float)i / (m_iNumVerticesZ - 1) * 20.f);
			((VTXTEX*)m_pVertices)[iIndex] = pVertices[iIndex];
		}
	}

	m_pVB->Unlock();

	m_iIndicesSize = sizeof(FACEINDICES32);
	m_eIndexFormat = D3DFMT_INDEX32;
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	m_pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	FACEINDICES32*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	_uint		iNumPrimitive = 0;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			_uint iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[iNumPrimitive]._0 = iIndices[0];
			pIndices[iNumPrimitive]._1 = iIndices[1];
			pIndices[iNumPrimitive]._2 = iIndices[2];
			++iNumPrimitive;

			pIndices[iNumPrimitive]._0 = iIndices[0];
			pIndices[iNumPrimitive]._1 = iIndices[2];
			pIndices[iNumPrimitive]._2 = iIndices[3];
			++iNumPrimitive;
		}
	}
	memcpy(m_pIndices, pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);
	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype(const _tchar * pHeightMapFilePath)
{
	_ulong		dwByte = 0;
	HANDLE		hFile = CreateFile(pHeightMapFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	BITMAPFILEHEADER			fh;
	BITMAPINFOHEADER			ih;

	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	_ulong* pPixel = new _ulong[ih.biWidth * ih.biHeight];
	ZeroMemory(pPixel, sizeof(_ulong) * ih.biWidth * ih.biHeight);

	ReadFile(hFile, pPixel, sizeof(_ulong) * ih.biWidth * ih.biHeight, &dwByte, nullptr);

	CloseHandle(hFile);

	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = ih.biWidth * ih.biHeight;
	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	m_pVertices = new VTXTEX[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXTEX) * m_iNumVertices);

	VTXTEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _float3(j, (pPixel[iIndex] & 0x000000ff) / 10.0f, i);
			pVertices[iIndex].vTexUV = _float2((_float)j / (m_iNumVerticesX - 1) * 20.f, (_float)i / (m_iNumVerticesZ - 1) * 20.f);

			((VTXTEX*)m_pVertices)[iIndex] = pVertices[iIndex];
		}
	}

	m_pVB->Unlock();

	m_iIndicesSize = sizeof(FACEINDICES32);
	m_eIndexFormat = D3DFMT_INDEX32;
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	m_pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	FACEINDICES32* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	_uint		iNumPrimitive = 0;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			_uint iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[iNumPrimitive]._0 = iIndices[0];
			pIndices[iNumPrimitive]._1 = iIndices[1];
			pIndices[iNumPrimitive]._2 = iIndices[2];
			++iNumPrimitive;

			pIndices[iNumPrimitive]._0 = iIndices[0];
			pIndices[iNumPrimitive]._1 = iIndices[2];
			pIndices[iNumPrimitive]._2 = iIndices[3];
			++iNumPrimitive;
		}
	}
	memcpy(m_pIndices, pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);
	m_pIB->Unlock();

	Safe_Delete_Array(pPixel);

	return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct(void * pArg)
{
	return S_OK;
}

_float CVIBuffer_Terrain::Compute_Y(const _float3& vPoint)
{
	VTXTEX*		pVertices = (VTXTEX*)m_pVertices;

	_uint		iIndex = (_uint)vPoint.z * m_iNumVerticesX + (_uint)vPoint.x;

	_uint		iIndices[4] = {
		iIndex + m_iNumVerticesX, 
		iIndex + m_iNumVerticesX + 1,
		iIndex + 1, 
		iIndex
	};

	_float		fWidth = vPoint.x - pVertices[iIndices[0]].vPosition.x;
	_float		fDepth = pVertices[iIndices[0]].vPosition.z - vPoint.z;

	D3DXPLANE	Plane;

	/* Y : (-AX - CZ - D) / B*/

	/* 오른쪽 위 삼각형에 있다. */
	if (fWidth > fDepth)
	{
		D3DXPlaneFromPoints(&Plane, &pVertices[iIndices[0]].vPosition,
			&pVertices[iIndices[1]].vPosition,
			&pVertices[iIndices[2]].vPosition);		
	}
	/* 왼쪽 하단 삼각형에 있다. */
	else
	{
		D3DXPlaneFromPoints(&Plane, &pVertices[iIndices[0]].vPosition,
			&pVertices[iIndices[2]].vPosition,
			&pVertices[iIndices[3]].vPosition);
	}

	return (-Plane.a * vPoint.x - Plane.c * vPoint.z - Plane.d) / Plane.b;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(iNumVerticesX, iNumVerticesZ)))
	{
		MSGBOX("Failed to Creating CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pHeightMapFilePath)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(pHeightMapFilePath)))
	{
		MSGBOX("Failed to Creating CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Terrain::Clone(void * pArg)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(*this);

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

#include "..\Public\Picking.h"

IMPLEMENT_SINGLETON(CPicking)

CPicking::CPicking()
{

}

HRESULT CPicking::NativeConstruct(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd)
{
	m_hWnd = hWnd;
	m_pGraphic_Device = pGraphic_Device;
	Safe_AddRef(m_pGraphic_Device);	

	return S_OK;
}

HRESULT CPicking::Transform_ToWorldSpace_Mouse()
{
	/* 뷰포트상의 마우스 위치를 구한다. */
	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);	

	D3DVIEWPORT9		ViewPortDesc;
	m_pGraphic_Device->GetViewport(&ViewPortDesc);

	/* 투영스페이스 상의 마우스 위치를 구한다. */
	/* 0, 0 -> WinCX, WinCY */
	/* -1, 1 -> 1, -1*/
	/* 로컬위치 * 월드 * 뷰 * 투영 */

	_float4		vMousePos;
	vMousePos.x = ptMouse.x / (ViewPortDesc.Width * 0.5f) - 1;
	vMousePos.y = ptMouse.y / -(ViewPortDesc.Height * 0.5f) + 1;
	vMousePos.z = 0.f;
	vMousePos.w = 1.f;

	/* 뷰스페이스 상의 위치로 변환한다. */
	/* 로컬위치 * 월드 * 뷰 */
	_float4x4		ProjMatrixInverse;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrixInverse);
	D3DXMatrixInverse(&ProjMatrixInverse, nullptr, &ProjMatrixInverse);
	D3DXVec4Transform(&vMousePos, &vMousePos, &ProjMatrixInverse);
	memcpy(&m_vRayDir, &(vMousePos - _float4(0.f, 0.f, 0.f, 1.f)), sizeof(_float3));
	m_vRayPos = _float3(0.f, 0.f, 0.f);

	/* 월드스페이스 상의 위치로 변환한다. */
	/* 로컬위치 * 월드 */
	_float4x4		ViewMatrixInverse;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrixInverse);
	D3DXMatrixInverse(&ViewMatrixInverse, nullptr, &ViewMatrixInverse);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &ViewMatrixInverse);
	D3DXVec3TransformCoord(&m_vRayPos, &m_vRayPos, &ViewMatrixInverse);

	return S_OK;
}

HRESULT CPicking::Transform_ToWorldSpace_Crosshair()
{
	_float4		vCrosshairPos = {0.f,0.f,0.f,1.f};

	/* 뷰스페이스 상의 위치로 변환한다. */
	/* 로컬위치 * 월드 * 뷰 */
	_float4x4		ProjMatrixInverse;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrixInverse);
	D3DXMatrixInverse(&ProjMatrixInverse, nullptr, &ProjMatrixInverse);
	D3DXVec4Transform(&vCrosshairPos, &vCrosshairPos, &ProjMatrixInverse);
	memcpy(&m_vRayDirCH, &(vCrosshairPos - _float4(0.f, 0.f, 0.f, 1.f)), sizeof(_float3));
	m_vRayPosCH = _float3(0.f, 0.f, 0.f);

	/* 월드스페이스 상의 위치로 변환한다. */
	/* 로컬위치 * 월드 */
	_float4x4		ViewMatrixInverse;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrixInverse);
	D3DXMatrixInverse(&ViewMatrixInverse, nullptr, &ViewMatrixInverse);
	D3DXVec3TransformNormal(&m_vRayDirCH, &m_vRayDirCH, &ViewMatrixInverse);
	D3DXVec3TransformCoord(&m_vRayPosCH, &m_vRayPosCH, &ViewMatrixInverse);
	return S_OK;
}

HRESULT CPicking::Transform_ToLocalSpace(TYPE _type, _float4x4 WorldMatrixInverse)
{
	switch (_type)
	{
	case Engine::CPicking::TYPE_MOUSE:
		D3DXVec3TransformNormal(&m_vRayDirInLocal, &m_vRayDir, &WorldMatrixInverse);
		D3DXVec3Normalize(&m_vRayDirInLocal, &m_vRayDirInLocal);
		D3DXVec3TransformCoord(&m_vRayPosInLocal, &m_vRayPos, &WorldMatrixInverse);
		break;
	case Engine::CPicking::TYPE_CROSSHAIR:
		D3DXVec3TransformNormal(&m_vRayDirInLocalCH, &m_vRayDirCH, &WorldMatrixInverse);
		D3DXVec3Normalize(&m_vRayDirInLocalCH, &m_vRayDirInLocalCH);
		D3DXVec3TransformCoord(&m_vRayPosInLocalCH, &m_vRayPosCH, &WorldMatrixInverse);
		break;
	}
	

	return S_OK;
}


_bool CPicking::isPick(TYPE _type, _float3* pLocalPoints, _float3 * pOut)
{
	_float		fU, fV, fDist;

	switch (_type)
	{
	case Engine::CPicking::TYPE_MOUSE:
		if (true == D3DXIntersectTri(&pLocalPoints[0], &pLocalPoints[1], &pLocalPoints[2],
			&m_vRayPosInLocal, &m_vRayDirInLocal, &fU, &fV, &fDist))
		{
			*pOut = m_vRayPosInLocal + m_vRayDirInLocal * fDist;
			return true;
		}
		break;
	case Engine::CPicking::TYPE_CROSSHAIR:
		if (true == D3DXIntersectTri(&pLocalPoints[0], &pLocalPoints[1], &pLocalPoints[2],
			&m_vRayPosInLocalCH, &m_vRayDirInLocalCH, &fU, &fV, &fDist))
		{
			*pOut = m_vRayPosInLocalCH + m_vRayDirInLocalCH * fDist;
			return true;
		}
		break;
	}
	return false;
}

void CPicking::Free()
{
	Safe_Release(m_pGraphic_Device);
}

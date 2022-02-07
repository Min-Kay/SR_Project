#include "..\Public\Camera.h"


CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
{

}

HRESULT CCamera::NativeConstruct_Prototype()
{
	

	return S_OK;
}

HRESULT CCamera::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_CameraDesc, pArg, sizeof(CAMERADESC));

	m_pTransform = CTransform::Create(m_pGraphic_Device);
	if (nullptr == m_pTransform)
		return E_FAIL;

	m_pTransform->Set_TransformDesc(m_CameraDesc.TransformDesc);

	_float3		vLook = m_CameraDesc.vAt - m_CameraDesc.vEye;
	D3DXVec3Normalize(&vLook, &vLook);

	_float3		vRight = *D3DXVec3Cross(&vRight, &m_CameraDesc.vAxisY, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	_float3		vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	m_pTransform->Set_State(CTransform::STATE_UP, vUp);
	m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
	m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);
	m_pTransform->Set_State(CTransform::STATE_POSITION, m_CameraDesc.vEye);

	return S_OK;
}

_int CCamera::Tick(_float fTimeDelta)
{
	if (nullptr == m_pGraphic_Device)
		return -1;

	if (FAILED(m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_pTransform->Get_WorldMatrixInverse())))
		return -1;

	_float4x4		ProjMatrix;

	D3DXMatrixPerspectiveFovLH(&ProjMatrix, m_CameraDesc.fFovy, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar);
	if (FAILED(m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &ProjMatrix)))
		return -1;

	return _int();
}

_int CCamera::LateTick(_float fTimeDelta)
{

	return _int();
}

HRESULT CCamera::Render()
{

	return S_OK;
}

void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);

}

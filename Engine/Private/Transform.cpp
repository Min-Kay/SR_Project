#include "..\Public\Transform.h"
#include "Shader.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{
}

_float3 CTransform::Get_Scale()
{	
	return _float3(D3DXVec3Length(&Get_State(CTransform::STATE_RIGHT)),
		D3DXVec3Length(&Get_State(CTransform::STATE_UP)), 
		D3DXVec3Length(&Get_State(CTransform::STATE_LOOK)));	
}

_float4x4 CTransform::Get_WorldMatrixInverse()
{
	_float4x4		WorldMatrixInverse;

	D3DXMatrixInverse(&WorldMatrixInverse, nullptr, &m_WorldMatrix);

	return WorldMatrixInverse;	
}

void CTransform::Set_TransformDesc(const TRANSFORMDESC & TransformDesc)
{
	m_TransformDesc = TransformDesc;
}

const CTransform::TRANSFORMDESC& CTransform::Get_TransformDesc() const
{
	return m_TransformDesc;
}

HRESULT CTransform::Bind_OnShader(CShader* _shader)
{
	_float4x4		WorldMatrix, ViewMatrix, ProjMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);

	if(FAILED(_shader->SetUp_ValueOnShader("g_WorldMatrix", D3DXMatrixTranspose(&WorldMatrix, &m_WorldMatrix), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(_shader->SetUp_ValueOnShader("g_ViewMatrix", D3DXMatrixTranspose(&ViewMatrix, &ViewMatrix), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(_shader->SetUp_ValueOnShader("g_ProjMatrix", D3DXMatrixTranspose(&ProjMatrix, &ProjMatrix), sizeof(_float4x4))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTransform::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_WorldMatrix = *D3DXMatrixIdentity(&m_WorldMatrix);

	return S_OK;
}

HRESULT CTransform::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORMDESC));

	return S_OK;
}

void CTransform::Go_Straight(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec  * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_BackWard(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec  * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Left(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition -= *D3DXVec3Normalize(&vRight, &vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Right(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition += *D3DXVec3Normalize(&vRight, &vRight) * m_TransformDesc.fSpeedPerSec* fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Up(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vUp = Get_State(CTransform::STATE_UP);

	vPosition += *D3DXVec3Normalize(&vUp, &vUp) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Down(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CTransform::STATE_POSITION);
	_float3		vUp = Get_State(CTransform::STATE_UP);

	vPosition -= *D3DXVec3Normalize(&vUp, &vUp) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::LookAt(_float3 vTargetPos)
{
	_float3	vPosition = Get_State(CTransform::STATE_POSITION);
	_float3	vScale = Get_Scale();

	_float3 vRight, vUp, vLook;

	vLook = vTargetPos - vPosition;
	vLook = *D3DXVec3Normalize(&vLook, &vLook) * vScale.z;

	vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vLook);
	if(vRight.x == 0.f && vRight.y == 0.f && vRight.z ==0.f)
		D3DXVec3Cross(&vRight, &_float3(0.f, 0.f, 1.f), &vLook);

	vRight = *D3DXVec3Normalize(&vRight, &vRight) * vScale.x;

	vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);
	vUp = *D3DXVec3Normalize(&vUp, &vUp) * vScale.y;

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::Turn(_float3 vAxis, _float fTimeDelta)
{
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp = Get_State(CTransform::STATE_UP);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	_float4x4	RotationMatrix;	
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, m_TransformDesc.fRotationPerSec * fTimeDelta);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::Rotation(_float3 vAxis, _float fRadian)
{
	_float3		vRight = _float3(1.f, 0.f, 0.f) * Get_Scale().x;
	_float3		vUp = _float3(0.f, 1.f, 0.f) * Get_Scale().y;
	_float3		vLook = _float3(0.f, 0.f, 1.f) * Get_Scale().z;

	_float4x4	RotationMatrix;
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, fRadian);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::Scaled(_float3 vScale)
{
	_float3		vRight = Get_State(CTransform::STATE_RIGHT);
	_float3		vUp = Get_State(CTransform::STATE_UP);
	_float3		vLook = Get_State(CTransform::STATE_LOOK);

	vRight = *D3DXVec3Normalize(&vRight, &vRight) * vScale.x;
	vUp = *D3DXVec3Normalize(&vUp, &vUp) * vScale.y;
	vLook = *D3DXVec3Normalize(&vLook, &vLook) * vScale.z;

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::Gravity(_float fWeight, _float fTimeDelta)
{

	_float3		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition -= _float3(0.f,1.f,0.f) * fWeight * (m_fGravity  + m_Press) * fTimeDelta ;

	m_fVelocity = !m_bOnCollide ? m_fVelocity + fTimeDelta : 0.f;
	m_Press = !m_bOnCollide ? m_Press + fTimeDelta * 1.1f: 0.f;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Add_Velocity(_float _add)
{
	m_fVelocity = m_fVelocity + _add >= 0.f ? m_fVelocity + _add : 0.f;
}

void CTransform::Set_Velocity(_float _vel)
{
	m_fVelocity = _vel >= 0.f ? _vel : m_fVelocity;
}

const _float& CTransform::Get_Velocity() const
{
	return m_fVelocity; 
}

void CTransform::Add_Force(_float fTimeDelta)
{
	if (!m_Forcing)
		return;

	if (!m_bOnCollide && m_Forcing)
	{
		if (m_fForce > 0.f)
		{
			_float3 pos = Get_State(STATE_POSITION);
			pos += m_vForceAxis * m_fForce * 0.2f;
			Set_State(CTransform::STATE_POSITION, pos);
			m_fForce -= fTimeDelta ;
		}
		else
		{
			m_fVelocity = 0.f;
			m_vForceAxis = _float3(0.f, 0.f, 0.f);
			m_Forcing = false;
			m_fForce = 0.f;
		}
	}
	else if (m_bOnCollide && m_Forcing)
	{
		m_FormalForce = m_fForce;
		if (m_fForce <= 0.f)
		{
			m_Forcing = false;
			return;
		}

		_float3 pos = Get_State(STATE_POSITION);
		pos += m_vForceAxis * m_fForce * 0.03f;
		Set_State(CTransform::STATE_POSITION, pos);
		m_fForce -= fTimeDelta * 5.f;
	}
	else
	{
		m_fVelocity = 0.f;
		m_vForceAxis = _float3(0.f, 0.f, 0.f);
		m_Forcing = false;
		m_fForce = 0.f;
	}
}

void CTransform::Set_Force(_float3 _axis)
{
	m_Forcing = true;
	m_fForce += m_fVelocity + m_Press;
	m_fVelocity  = 0.f;
	m_vForceAxis = _axis;
}

void CTransform::Set_OnCollide(_bool _bool)
{
	m_bOnCollide = _bool;
}

const _bool& CTransform::Get_OnCollide() const
{
	return m_bOnCollide;
}

HRESULT CTransform::Bind_OnGraphicDevice()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);	
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform*	pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform*	pInstance = new CTransform(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();

}

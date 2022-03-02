#include "stdafx.h"
#include "Effect_Alert.h"

#include "Transform.h"
#include "VIBuffer_Rect.h"

CEffect_Alert::CEffect_Alert(LPDIRECT3DDEVICE9 m_pGraphic_Device)
	:CEffect(m_pGraphic_Device)
{
}

CEffect_Alert::CEffect_Alert(const CEffect_Alert& rhs)
	:CEffect(rhs)
{
}

HRESULT CEffect_Alert::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	return S_OK;

}

HRESULT CEffect_Alert::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;
	return S_OK;
}

_int CEffect_Alert::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	if (m_pCameraTr && m_pTargetTr)
	{
		_float3 camUp = m_pCameraTr->Get_State(CTransform::STATE_UP);
		D3DXVec3Normalize(&camUp,&camUp);
		_float3 targetPos = m_pTargetTr->Get_State(CTransform::STATE_POSITION); 

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, targetPos + camUp * m_AdditionalPos);
	}

	return 0;
}

_int CEffect_Alert::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	_float3 vScale = m_pTransformCom->Get_Scale();

	_float3 vRight = m_pCameraTr->Get_State(CTransform::STATE_RIGHT);
	_float3 vUp = m_pCameraTr->Get_State(CTransform::STATE_UP);
	_float3 vLook = m_pCameraTr->Get_State(CTransform::STATE_LOOK);

	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
	if (!m_Desc.FixY)
		m_pTransformCom->Set_State(CTransform::STATE_UP, vUp * vScale.y);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook * vScale.z);

	return 0;
}

HRESULT CEffect_Alert::Render()
{
	if(FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;
}

HRESULT CEffect_Alert::SetUp_Components()
{
	if(FAILED(__super::SetUp_Components()))
		return E_FAIL;
	return S_OK;
}

HRESULT CEffect_Alert::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	switch (m_Desc.Alpha)
	{
	case EFFECTALPHA_DEFAULT:
		m_pVIBufferCom->Render();

		break;
	case EFFECTALPHA_BLEND:
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_pVIBufferCom->Render();

		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		break;
	case EFFECTALPHA_TEST:
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, m_Desc.Ref);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, m_Desc.Func);

		m_pVIBufferCom->Render();

		m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		break;
	}
	return S_OK;

}

HRESULT CEffect_Alert::Release_RenderState()
{
	if (FAILED(__super::Release_RenderState()))
		return E_FAIL;
	return S_OK;

}

void CEffect_Alert::Set_CamPos(CTransform* _tr)
{
	m_pCameraTr = _tr;
}

void CEffect_Alert::Set_TargetPos(CTransform* _tr)
{
	m_pTargetTr = _tr;
}

CEffect_Alert* CEffect_Alert::Create(LPDIRECT3DDEVICE9 m_pGraphic_Device)
{
	CEffect_Alert* p_alert = new CEffect_Alert(m_pGraphic_Device);
	if(FAILED(p_alert->NativeConstruct_Prototype()))
	{
		Safe_Release(p_alert);
		return nullptr;
	}
	return p_alert;
}

CGameObject* CEffect_Alert::Clone(void* pArg)
{
	CEffect_Alert* p_alert = new CEffect_Alert(*this);
	if (FAILED(p_alert->NativeConstruct(pArg)))
	{
		Safe_Release(p_alert);
		return nullptr;
	}
	return p_alert;
}

void CEffect_Alert::Free()
{
	__super::Free();
}

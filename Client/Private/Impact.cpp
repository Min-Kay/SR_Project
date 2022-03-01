#include "stdafx.h"
#include "..\Public\Impact.h"
#include "GameInstance.h"

CImpact::CImpact(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CImpact::CImpact(const CImpact & rhs)
	: CGameObject(rhs)
{
}

HRESULT CImpact::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;



	return S_OK;
}

HRESULT CImpact::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_Impact = *static_cast<IMPACT*>(pArg);
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_Impact.Pos);
	m_pTransformCom->Scaled(m_Impact.Size);

	_float3 centerPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);//임시로 만든 퍼지는점 중심

	mt19937 ranX(rd());
	mt19937 ranY(rd());
	mt19937 ranZ(rd());

	uniform_real_distribution <_double> spread(-m_Impact.randomPos, m_Impact.randomPos);

	_float randomPos[3];

	randomPos[0] = spread(ranX);
	randomPos[1] = spread(ranY);
	randomPos[2] = spread(ranZ);

	m_fvecdir = _float3((centerPos.x + randomPos[0]), (centerPos.y + randomPos[1]), (centerPos.z + spread(ranZ))) - centerPos;
	D3DXVec3Normalize(&m_fvecdir, &m_fvecdir);

	m_pVIBufferCom->ChangeColor(m_Impact.Color);//rgba

	return S_OK;
}

_int CImpact::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	m_fFrame += fTimeDelta;

	//if (m_fFrame == 5)
	//{
	//	m_fFrame = 0.f;
	//	m_Impact.Color = D3DXCOLOR(0, 0, 0, 0);
	//	m_pVIBufferCom->ChangeColor(D3DXCOLOR(1, 0.9, 0, 0));//rgba
	//}
	//else 
	//{
	//	m_Impact.Color -= D3DXCOLOR(0,0.1,0,0);
	//	m_pVIBufferCom->ChangeColor(m_Impact.Color);
	//	//m_fFrame = 0.f;
	//
	//}


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_fvecdir * m_pTransformCom->Get_TransformDesc().fSpeedPerSec * fTimeDelta);


	return _int();
}

_int CImpact::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	
	if(m_fcount == 5)
	{
		m_Impact.DeleteImpact = true;
	}
	else
	{
		m_fcount += fTimeDelta;
		m_Impact.DeleteImpact = true;
	}




	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CImpact::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	FaceOn_Camera();

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}



HRESULT CImpact::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);



	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLOR, COM_BUFFER, (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Box */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLLIDER, COM_COLLIDER, (CComponent**)&m_pBoxColliderCom)))
		return E_FAIL;


	m_pBoxColliderCom->Set_ParentInfo(this);
	m_pBoxColliderCom->Set_State(CBoxCollider::COLLIDERINFO::COLL_SIZE, _float3(1.f, 1.f, 1.f));

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_STATIC, m_pBoxColliderCom);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CImpact::FaceOn_Camera()
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);

	_float4x4		ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	_float3		vCamPosition = *(_float3*)&ViewMatrix.m[3][0];
	_float3		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3		vDir = vPosition - vCamPosition;
	_float		m_fCamDistance = D3DXVec3Length(&vDir);

	if (!m_pTarget)
	{
		m_pTarget = p_instance->Find_Camera_Object(MAIN_CAM)->Get_CameraTransform();
	}

	if (m_pTarget)
	{
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, m_pTarget->Get_State(CTransform::STATE_RIGHT) * m_pTransformCom->Get_Scale().x);
		m_pTransformCom->Set_State(CTransform::STATE_UP, m_pTarget->Get_State(CTransform::STATE_UP) * m_pTransformCom->Get_Scale().y);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_pTarget->Get_State(CTransform::STATE_LOOK) * m_pTransformCom->Get_Scale().z);
	}


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}


HRESULT CImpact::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CImpact::Release_RenderState()
{

	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	return S_OK;
}



CImpact * CImpact::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CImpact*	pInstance = new CImpact(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CImpact");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CImpact::Clone(void* pArg)
{
	/* 새로운객체를 복제하여 생성한다. */
	CImpact*	pInstance = new CImpact(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CImpact");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CImpact::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pBoxColliderCom);
}

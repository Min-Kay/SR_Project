#include "stdafx.h"
#include "..\Public\Impact.h"
#include "GameInstance.h"
#include "VIBuffer_Color.h"

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

	m_pTransformCom->Scaled(_float3(1.f, 1.f, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION ,_float3(rand() % 20 + 2, rand() % 20 + 2, rand() % 20 + 2));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, _float3(0.f, 0.f, rand() % 30));
	m_fLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	return S_OK;
}

_int CImpact::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	m_fFrame += 90.0f * fTimeDelta;

	if (m_fFrame >= 90.0f)
		m_fFrame = 0.f;

	/*m_lColor += 90.0f * fTimeDelta;

	if (m_lColor >= 90.0f)
		m_lColor = 0.f;*/

	
		

		m_lColor = D3DXCOLOR(1.f, 0.f, 1.f, 1.f);

	m_pVIBufferCom->ChangeColor(m_lColor);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr != m_pBoxColliderCom)
	{
		m_pBoxColliderCom->Set_Coilider();
	}

	if (!m_pTarget)
	{
	
		m_pTarget = pGameInstance->Find_Camera_Object(MAIN_CAM)->Get_CameraTransform();
	
		
	}
	RELEASE_INSTANCE(CGameInstance);


	/*testGravity = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	testGravity.y -= 4.5 * fTimeDelta * 0.1f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, testGravity);*/
	return _int();
}

_int CImpact::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	

	//if (m_pTarget)
	//{
	//	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, m_pTarget->Get_State(CTransform::STATE_RIGHT));
	//	m_pTransformCom->Set_State(CTransform::STATE_UP, m_pTarget->Get_State(CTransform::STATE_UP));
	//	m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_pTarget->Get_State(CTransform::STATE_LOOK));
	//}

	//_float4x4		ViewMatrix;
	//m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	//D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	//m_pTransformCom->Set_State(CTransform::STATE_RIGHT, (*(_float3*)&ViewMatrix.m[0][0]) * m_pTransformCom->Get_Scale().x);
	////m_pTransformCom->Set_State(CTransform::STATE_UP, (*(_float3*)&ViewMatrix.m[1][0]) * m_pTransformCom->Get_Scale().y);
	//m_pTransformCom->Set_State(CTransform::STATE_LOOK, (*(_float3*)&ViewMatrix.m[2][0]) * m_pTransformCom->Get_Scale().z);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CImpact::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;
	
	m_pBoxColliderCom->Draw_Box();

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

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

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLOR, COM_BUFFER, (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	///* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Impact"), COM_TEXTURE, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Box */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLLIDER, COM_COLLIDER, (CComponent**)&m_pBoxColliderCom)))
		return E_FAIL;

	m_pBoxColliderCom->Set_ParentInfo(this);
	m_pBoxColliderCom->Set_State(CBoxCollider::COLLIDERINFO::COLL_SIZE, _float3(1.f, 1.f, 1.f));

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr != m_pBoxColliderCom)
	{
		pGameInstance->Add_Collider(CCollision_Manager::COLLOBJTYPE_STATIC,m_pBoxColliderCom);
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


HRESULT CImpact::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;


	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CImpact::Release_RenderState()
{
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

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

CGameObject * CImpact::Clone(void* pArg )
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
	Safe_Release(m_pBoxColliderCom);
	Safe_Release(m_pTextureCom); 
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

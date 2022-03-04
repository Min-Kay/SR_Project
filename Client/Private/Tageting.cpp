#include "stdafx.h"
#include "..\Public\Tageting.h"
#include "GameInstance.h"
#include "Player.h"

CTageting::CTageting(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CTageting::CTageting(const CTageting & rhs)
	:CGameObject(rhs)
	, m_pTransformCom(rhs.m_pTransformCom)
	, m_pBoxColliderCom(rhs.m_pBoxColliderCom)
	, m_pRendererCom(rhs.m_pRendererCom)
	, m_pTextureCom(rhs.m_pTextureCom)
	, m_pVIBufferCom(rhs.m_pVIBufferCom)
{
	Safe_AddRef(m_pTransformCom);
	Safe_AddRef(m_pBoxColliderCom);
	Safe_AddRef(m_pRendererCom);
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pVIBufferCom);

}
HRESULT CTageting::NativeConstruct_Prototype()
{
	if(FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
	
}

HRESULT CTageting::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	CTransform* PlayerTrans = (CTransform*)m_pPlayer->Get_Component(COM_TRANSFORM);
	_float3 PlayerPos = PlayerTrans->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, PlayerPos);
	return S_OK;
}

_int CTageting::Tick(_float fTimeDelta)
{
	if (Get_Dead())
		return 0;

	if (0 > __super::Tick(fTimeDelta))
		return -1;

	m_pBoxColliderCom->Set_Collider();
	timer += fTimeDelta * 0.5f;

	if (timer / 5 >= 1.0f)
	{
		Set_Dead(true);
		timer = 0;
		return 0;
	}

	return _int();
}

_int CTageting::LateTick(_float fTimeDelta)
{
	if (Get_Dead())
		return 0;

	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CTageting::Render()
{
	if (Get_Dead())
		return 0;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;
	FaceOn_Camera();
	Set_RanderState();

	m_pVIBufferCom->Render();

	Release_RanderState();
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTageting::Set_RanderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CTageting::Release_RanderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	return S_OK;
}



HRESULT CTageting::FaceOn_Camera()
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



HRESULT CTageting::SetUp_Component()
{
	CTransform::TRANSFORMDESC desc;
	desc.fRotationPerSec = 90.f;
	desc.fSpeedPerSec = 5.f;

	
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransformCom, &desc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLLIDER, COM_COLLIDER, (CComponent**)&m_pBoxColliderCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RECT, COM_BUFFER, (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tageting"), COM_TEXTURE, (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	Set_Type(OBJ_STATIC);


	m_pTransformCom->Scaled(_float3(3.f, 3.f, 3.f));

	m_pBoxColliderCom->Set_ParentInfo(this);
	m_pBoxColliderCom->Set_CollStyle(CCollider::COLLSTYLE_TRIGGER);
	m_pBoxColliderCom->Set_State(CBoxCollider::COLL_SIZE, _float3(1.f, 1.f, 1.f));
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_STATIC, m_pBoxColliderCom);
	m_pPlayer = static_cast<CPlayer*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("Layer_Player"))); 
	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

CTageting * CTageting::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTageting* pTageting = new CTageting(pGraphic_Device);
	if (FAILED(pTageting->NativeConstruct_Prototype()))
	{
		MSGBOX("Fail Create Missle")
			Safe_Release(pTageting);
		return nullptr;
	}
	return pTageting;


}

CGameObject * CTageting::Clone(void * pArg)
{
	CTageting* pTageting = new CTageting(*this);
	if (FAILED(pTageting->NativeConstruct(pArg)))
	{
		MSGBOX("Fail Create CTageting")
			Safe_Release(pTageting);
		return nullptr;
	}
	return pTageting;
}

void CTageting::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBoxColliderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}

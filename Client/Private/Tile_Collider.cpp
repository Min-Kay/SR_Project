#include "stdafx.h"
#include "..\Public\Tile_Collider.h"
#include "GameInstance.h"

CTileCollider::CTileCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)	
{

}

CTileCollider::CTileCollider(const CTileCollider & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTileCollider::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	

	return S_OK;
}

HRESULT CTileCollider::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	Set_Type(OBJ_STATIC);


	return S_OK;
}

_int CTileCollider::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;


	if (m_pBoxColliderCom)
		m_pBoxColliderCom->Set_Collider();

	return _int();
}

_int CTileCollider::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CTileCollider::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	/*if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(m_iTextureIndex)))
		return E_FAIL;*/

	_float			blend = 0;


	m_pTransformCom->Bind_OnShader(m_pShader);

	m_pShader->SetUp_ValueOnShader("g_ColorStack", &g_ControlShader, sizeof(_float));
	m_pShader->SetUp_ValueOnShader("g_Alpha", &blend, sizeof(_uint));
	m_pShader->SetUp_ValueOnShader("g_Color", &_float4(0.f, 0.f,0.f, 0.f), sizeof(_float4));
	m_pTextureCom->Bind_OnShader(m_pShader, "g_Texture", m_iTextureIndex);

	m_pShader->Begin_Shader(SHADER_SETCOLOR);

	m_pVIBufferCom->Render();

	m_pShader->End_Shader();


	return S_OK;
}

HRESULT CTileCollider::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RECT, COM_BUFFER, (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tile"), COM_TEXTURE, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Box */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLLIDER, COM_COLLIDER, (CComponent**)&m_pBoxColliderCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_SHADER_RECT, COM_SHADER, (CComponent**)&m_pShader)))
		return E_FAIL;

	m_pBoxColliderCom->Set_ParentInfo(this);
	//m_pBoxColliderCom->Set_State(CBoxCollider::COLLIDERINFO::COLL_SIZE, _float3(1.f, 1.f, 1.f));

	m_pBoxColliderCom->Set_CollStyle(CCollider::COLLSTYLE_ENTER);

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_STATIC, m_pBoxColliderCom);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


CTileCollider * CTileCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTileCollider*	pInstance = new CTileCollider(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CTileCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTileCollider::Clone(void* pArg )
{
	/* 새로운객체를 복제하여 생성한다. */
	CTileCollider*	pInstance = new CTileCollider(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CTileCollider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTileCollider::Free()
{
	__super::Free();
	Safe_Release(m_pBoxColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShader);

}

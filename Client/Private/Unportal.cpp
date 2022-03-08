#include "stdafx.h"
#include "Unportal.h"

#include "Transform.h"
#include "Renderer.h"
#include "VIBuffer_Cube.h"
#include "Texture.h"
#include "BoxCollider.h"
#include "GameInstance.h"
#include "Player.h"
#include "Shader.h"

CUnportal::CUnportal(LPDIRECT3DDEVICE9 m_pGraphic_Device)
	:CGameObject(m_pGraphic_Device)
{
}

CUnportal::CUnportal(const CUnportal& rhs)
	:CGameObject(rhs)
	,m_pTransform(rhs.m_pTransform)
	,m_pTexture(rhs.m_pTexture)
	,m_pBuffer(rhs.m_pBuffer)
	,m_pCollider(rhs.m_pCollider)
	,m_pRenderer(rhs.m_pRenderer)
	,m_pShader(rhs.m_pShader)
{
	Safe_AddRef(m_pTransform);
	Safe_AddRef(m_pTexture);
	Safe_AddRef(m_pBuffer);
	Safe_AddRef(m_pCollider);
	Safe_AddRef(m_pRenderer);
	Safe_AddRef(m_pShader);

}

HRESULT CUnportal::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUnportal::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	return S_OK;
}

_int CUnportal::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	m_pCollider->Set_Collider();


	if(m_pPlayer)
	{
		CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
		if (p_instance->Get_Collide(m_pCollider, static_cast<CBoxCollider*>(m_pPlayer->Get_Component(COM_COLLIDER))))
			m_pPlayer->Erase_Portal();
		RELEASE_INSTANCE(CGameInstance);
	}


	return 0;
}

_int CUnportal::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);
	return 0;
}

HRESULT CUnportal::Render()
{
	if(FAILED(__super::Render()))
		return E_FAIL;

	//if (FAILED(m_pTransform->Bind_OnGraphicDevice()))
	//	return E_FAIL;

	//if (FAILED(m_pTexture->Bind_OnGraphicDevice()))
	//	return E_FAIL;

	//m_pBuffer->Render();

	m_pTransform->Bind_OnShader(m_pShader);

	m_pShader->SetUp_ValueOnShader("g_ColorStack", &g_ControlShader, sizeof(_float));

	m_pTexture->Bind_OnShader(m_pShader, "g_Texture", 0);

	m_pShader->Begin_Shader(SHADER_SETCOLOR_BLEND_CUBE);
	m_pBuffer->Render();
	m_pShader->End_Shader();

	return S_OK; 
}

HRESULT CUnportal::SetUp_Component()
{
	CTransform::TRANSFORMDESC desc;
	desc.fRotationPerSec = 90.f;
	desc.fSpeedPerSec = 5.f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransform, &desc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLLIDER, COM_COLLIDER, (CComponent**)&m_pCollider)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRenderer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_CUBE, COM_BUFFER, (CComponent**)&m_pBuffer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_SHADER_CUBE, COM_SHADER, (CComponent**)&m_pShader)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UnPortal"), COM_TEXTURE, (CComponent**)&m_pTexture)))
		return E_FAIL;


	m_pCollider->Set_ParentInfo(this);
	m_pCollider->Set_State(CBoxCollider::COLL_SIZE, _float3(1.f, 1.f, 1.f));
	m_pCollider->Set_CollStyle(CCollider::COLLSTYLE_ONLYRAY);

	Set_Type(OBJ_STATIC);
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_STATIC, m_pCollider);

	m_pPlayer = static_cast<CPlayer*>(p_instance->Get_GameObject_End(g_CurrLevel, TEXT("Layer_Player")));
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;

}

CUnportal* CUnportal::Create(LPDIRECT3DDEVICE9 m_pGraphic_Device)
{
	CUnportal* p_unportal = new CUnportal(m_pGraphic_Device);
	if(FAILED(p_unportal->NativeConstruct_Prototype()))
	{
		Safe_Release(p_unportal);
		return nullptr;
	}
	return p_unportal;
}

CGameObject* CUnportal::Clone(void* pArg)
{
	CUnportal* p_unportal = new CUnportal(*this);
	if (FAILED(p_unportal->NativeConstruct(pArg)))
	{
		Safe_Release(p_unportal);
		return nullptr;
	}
	return p_unportal;
}

void CUnportal::Free()
{
	__super::Free();
	Safe_Release(m_pTransform);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pCollider);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pShader);
}

#include "stdafx.h"
#include "BazierBullet.h"

#include "Transform.h"
#include "BoxCollider.h"
#include "Renderer.h"
#include "VIBuffer_Cube.h"
#include "Texture.h"

CBazierBullet::CBazierBullet(LPDIRECT3DDEVICE9 m_pGraphic_Device)
	:CGameObject(m_pGraphic_Device)
{
}

CBazierBullet::CBazierBullet(const CBazierBullet& rhs)
	:CGameObject(rhs)
	,m_pTransformCom(rhs.m_pTransformCom)
	,m_pBoxCollider(rhs.m_pBoxCollider)
	,m_pRendererCom(rhs.m_pRendererCom)
	,m_pTextureCom(rhs.m_pTextureCom)
	,m_pVIBufferCom(rhs.m_pVIBufferCom)
{
	Safe_AddRef(m_pTransformCom);
	Safe_AddRef(m_pBoxCollider);
	Safe_AddRef(m_pRendererCom);
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pVIBufferCom);

}

HRESULT CBazierBullet::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CBazierBullet::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	return S_OK;
}

_int CBazierBullet::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	Move_By_Bazier(fTimeDelta);
	m_pBoxCollider->Set_Collider();
	return 0;
}

_int CBazierBullet::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);


	return 0;
}

HRESULT CBazierBullet::Render()
{

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return -1;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return -1;

	m_pVIBufferCom->Render();

	if (FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;
}

void CBazierBullet::Set_Pos(_float3 _start,_float3 _mid, _float3 _end)
{
	m_StartPos = _start;
	m_MidPos = _mid; 
	m_EndPos = _end; 
}

void CBazierBullet::Move_By_Bazier(_float fTimeDelta)
{
	if (m_fTimer >= 1.f)
		m_fTimer = 0.f;

	m_fTimer += fTimeDelta * 0.5f;
	_float3 CurrPos;

	CurrPos = pow(1 - m_fTimer, 2) * m_StartPos + 2 * m_fTimer * (1 - m_fTimer) * m_MidPos + pow(m_fTimer, 2) * m_EndPos;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, CurrPos);

}

CBazierBullet* CBazierBullet::Create(LPDIRECT3DDEVICE9 m_pGraphic_Device)
{
	CBazierBullet* p_bullet = new CBazierBullet(m_pGraphic_Device);
	if(FAILED(p_bullet->NativeConstruct_Prototype()))
	{
		Safe_Release(p_bullet);
		return nullptr;
	}
	return p_bullet;
}

CGameObject* CBazierBullet::Clone(void* pArg)
{
	CBazierBullet* p_bullet = new CBazierBullet(*this);
	if (FAILED(p_bullet->NativeConstruct(pArg)))
	{
		Safe_Release(p_bullet);
		return nullptr;
	}
	return p_bullet;
}

void CBazierBullet::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);

}

HRESULT CBazierBullet::SetUp_Component()
{

	CTransform::TRANSFORMDESC desc;
	desc.fRotationPerSec = 90.f;
	desc.fSpeedPerSec = 10.f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransformCom, &desc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRendererCom)))
		return E_FAIL;


	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLLIDER, COM_COLLIDER, (CComponent**)&m_pBoxCollider)))
		return E_FAIL;


	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_CUBE, COM_BUFFER, (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CubeAttack"), COM_TEXTURE, (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	m_pBoxCollider->Set_ParentInfo(this);
	m_pBoxCollider->Set_State(CBoxCollider::COLL_SIZE, _float3(0.3f, 0.3f, 0.3f));
	Set_Type(OBJ_INTERACTION);
	m_pBoxCollider->Set_CollStyle(CCollider::COLLSTYLE_TRIGGER);
	return S_OK;
}

#include "stdafx.h"
#include "CubeBullet.h"

#include "Transform.h"
#include "BoxCollider.h"
#include "Renderer.h"
#include "VIBuffer_Cube.h"

#include "GameInstance.h"
#include "Player.h"

Client::CCubeBullet::CCubeBullet(LPDIRECT3DDEVICE9 m_pGraphic_Device)
	:CGameObject(m_pGraphic_Device)
{
}

Client::CCubeBullet::CCubeBullet(const CCubeBullet& rhs)
	:CGameObject(rhs)
	,m_pOnlyRotation(rhs.m_pOnlyRotation)
{
	Safe_AddRef(m_pOnlyRotation);
}

HRESULT Client::CCubeBullet::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT Client::CCubeBullet::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;


	SetUp_Component();

	return S_OK;
}

_int Client::CCubeBullet::Tick(_float fTimeDelta)
{
	if (Get_Dead())
		return 0;
	Move(fTimeDelta);
	Synchronize_Transform(fTimeDelta);
	m_pBoxCollider->Set_Collider();
	return 0;
}

_int Client::CCubeBullet::LateTick(_float fTimeDelta)
{

	if (Get_Dead())
		return 0;

	m_Timer += fTimeDelta;
	if(Check_Collide() || m_Timer >= 5.f)
	{
		m_pBoxCollider->Set_Dead(true);
		Set_Dead(true);
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return 0;
}

HRESULT Client::CCubeBullet::Render()
{
	if (FAILED(m_pOnlyRotation->Bind_OnGraphicDevice()))
		return -1;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return -1;

	m_pVIBufferCom->Render();

	return S_OK; 
}

void CCubeBullet::Set_Init(_float3 _pos, _float3 _dir)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _pos);
	m_pTransformCom->LookAt(_pos + _dir);
}

void CCubeBullet::Move(_float fTimeDelta)
{
	m_pTransformCom->Go_Straight(m_Speed * fTimeDelta);
}


HRESULT Client::CCubeBullet::Synchronize_Transform(_float fTimeDelata)
{
	if (!m_pOnlyRotation || !m_pTransformCom)
		return E_FAIL;

	m_pOnlyRotation->Turn(_float3(1.f, 1.f, 0.f), 10.f * fTimeDelata);

	m_pOnlyRotation->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	return S_OK;
}


_bool CCubeBullet::Check_Collide()
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);

	list<CGameObject*> col_list = p_instance->Get_Collision_List(m_pBoxCollider);

	for (auto& obj : col_list)
	{
		if (obj->Get_Type() == OBJ_PLAYER)
		{
			static_cast<CPlayer*>(obj)->Add_Hp(-m_Damage);
			RELEASE_INSTANCE(CGameInstance);
			return true;
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	return false;
}

HRESULT Client::CCubeBullet::SetUp_Component()
{
	CTransform::TRANSFORMDESC desc;
	desc.fRotationPerSec = 90.f;
	desc.fSpeedPerSec = 10.f;

	if(FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransformCom, &desc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRendererCom)))
		return E_FAIL;


	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLLIDER, COM_COLLIDER, (CComponent**)&m_pBoxCollider)))
		return E_FAIL;


	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_CUBE, COM_BUFFER, (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CubeAttack"), COM_TEXTURE, (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, TEXT("AnimationOnly"), (CComponent**)&m_pOnlyRotation)))
		return E_FAIL;


	m_pOnlyRotation->Scaled(_float3(0.3f,0.3f,0.3f));

	m_pBoxCollider->Set_ParentInfo(this);
	m_pBoxCollider->Set_State(CBoxCollider::COLL_SIZE, _float3(0.3f, 0.3f, 0.3f));
	Set_Type(OBJ_INTERACTION);
	m_pBoxCollider->Set_CollStyle(CCollider::COLLSTYLE_TRIGGER);
	return S_OK;
}

Client::CCubeBullet* Client::CCubeBullet::Create(LPDIRECT3DDEVICE9 m_pGraphic_Device)
{
	CCubeBullet* p_bullet = new CCubeBullet(m_pGraphic_Device);

	if (FAILED(p_bullet->NativeConstruct_Prototype()))
	{
		Safe_Release(p_bullet);
		return nullptr;
	}
	return p_bullet;
}

CGameObject* Client::CCubeBullet::Clone(void* pArg)
{
	CCubeBullet* p_bullet = new CCubeBullet(*this);

	if (FAILED(p_bullet->NativeConstruct(pArg)))
	{
		Safe_Release(p_bullet);
		return nullptr;
	}
	return p_bullet;
}

void Client::CCubeBullet::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pOnlyRotation);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);


}

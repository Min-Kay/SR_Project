#include "stdafx.h"
#include "..\Public\Targeting.h"
#include "GameInstance.h"
#include "Player.h"
#include "Missile.h"
#include "Transform.h"
#include "Boss.h"

CTargeting::CTargeting(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CTargeting::CTargeting(const CTargeting& rhs)
	: CGameObject(rhs)
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
HRESULT CTargeting::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;

}

HRESULT CTargeting::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;


	m_Target = *static_cast<TARGET*>(pArg);


	m_pBoss = static_cast<CBoss*>(m_Target.Parent);
	CTransform* BossTr = static_cast<CTransform*>(m_pBoss->Get_Component(COM_TRANSFORM));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, BossTr->Get_State(CTransform::STATE_POSITION));
	m_bcheckCollider = false;

	return S_OK;
}

_int CTargeting::Tick(_float fTimeDelta)
{
	if (Get_Dead())
		return 0;

	if (0 > __super::Tick(fTimeDelta))
		return -1;

	m_fFrame += 36.f * fTimeDelta;
	if (m_fFrame >= 12.f)
	{
		m_fFrame = 1.f;
	}


	m_pBoxColliderCom->Set_Collider();

	ColliderCheck();

	if (m_Target.MainTaret == true)
	{
		MainTarget(fTimeDelta);
	}
	else
	{

		SubTarget(fTimeDelta);
	}






	return _int();
}

_int CTargeting::LateTick(_float fTimeDelta)
{
	if (Get_Dead())
		return 0;

	if (0 > __super::LateTick(fTimeDelta))
		return -1;


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CTargeting::Render()
{
	if (Get_Dead())
		return 0;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;


	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(0)))
		return E_FAIL;

	if (m_bcheckCollider == true)
	{
		if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_uint)m_fFrame)))
			return E_FAIL;
	}



	Set_RanderState();

	m_pVIBufferCom->Render();

	Release_RanderState();
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTargeting::Set_RanderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	return S_OK;
}

HRESULT CTargeting::Release_RanderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	return S_OK;
}


HRESULT CTargeting::MainMoving(_float fTimeDelta)
{

	BezierPos = BezierCurve(m_Target.Pos1, m_Target.Pos2, m_Target.Pos3, fTimeDelta);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, BezierPos);
	return S_OK;
}


HRESULT CTargeting::ColliderCheck()
{
	if (m_bcheckCollider)
		return S_OK;

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	list<CGameObject*> test = p_instance->Get_Collision_List(m_pBoxColliderCom);

	_float3 Look, playerpos, Targetpos;
	for (auto& iter : test)
	{

		CCollider::COLLSTYLE listCollstyle = static_cast<CCollider*>(iter->Get_Component(COM_COLLIDER))->Get_CollStyle();

		if (listCollstyle == CCollider::COLLSTYLE_ENTER && iter != m_Target.Parent && iter != m_pPlayer)
		{
			m_pTarget = (CTransform*)iter->Get_Component(COM_TRANSFORM);
			_float3 Target_Right = m_pTarget->Get_State(CTransform::STATE_RIGHT);
			_float3 Target_Up = m_pTarget->Get_State(CTransform::STATE_UP);
			_float3 Target_Look = m_pTarget->Get_State(CTransform::STATE_LOOK);
			D3DXVec3Normalize(&Target_Right, &Target_Right);
			D3DXVec3Normalize(&Target_Up, &Target_Up);
			D3DXVec3Normalize(&Target_Look, &Target_Look);
			if (m_pTarget)
			{
				m_pTransformCom->Set_State(CTransform::STATE_RIGHT, Target_Right * m_pTransformCom->Get_Scale().x);
				m_pTransformCom->Set_State(CTransform::STATE_UP, Target_Up * m_pTransformCom->Get_Scale().y);
				m_pTransformCom->Set_State(CTransform::STATE_LOOK, Target_Look * m_pTransformCom->Get_Scale().z);

				m_bcheckCollider = true;
			}

			break;
		}


	}
	RELEASE_INSTANCE(CGameInstance)
		return S_OK;
}

_float3 CTargeting::BezierCurve(_float3 P0, _float3 P1, _float3 P2, _float time)
{

	_float3 vTesult1;
	_float3 vTesult2;
	_float3 vTesult3;

	D3DXVec3Lerp(&vTesult1, &P0, &P1, time);
	D3DXVec3Lerp(&vTesult2, &P1, &P2, time);
	D3DXVec3Lerp(&vTesult3, &vTesult1, &vTesult2, time);

	return vTesult3;
}

HRESULT CTargeting::MainTarget(_float fTimeDelta)
{

	if (m_bcheckCollider == false)
	{

		timer += fTimeDelta;
		MainMoving(timer);//???? ???????? ???? ???? ??????

	}
	else
	{
		timer = 0.f;
		MainMoving(1); //???????? ???? ???????? ???????? ???? ?????? ??????
		deletetimer += fTimeDelta;
		if (deletetimer >= 8.f)
		{
			deletetimer = 0.f;
			m_pBoxColliderCom->Set_Dead(true);
			Set_Dead(true);
		}
	}
	return S_OK;
}

HRESULT CTargeting::SubTarget(_float fTimeDelta)
{

	MainMoving(1);
	deletetimer += fTimeDelta;
	if (deletetimer >= 5.f)
	{
		deletetimer = 0.f;
		m_pBoxColliderCom->Set_Dead(true);
		Set_Dead(true);
	}
	return E_NOTIMPL;
}




HRESULT CTargeting::SetUp_Component()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Targeting"), COM_TEXTURE, (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	Set_Type(OBJ_NONE);


	m_pTransformCom->Scaled(_float3(4.f, 4.f, 4.f));

	m_pBoxColliderCom->Set_ParentInfo(this);
	m_pBoxColliderCom->Set_CollStyle(CCollider::COLLSTYLE_TRIGGER);
	m_pBoxColliderCom->Set_State(CBoxCollider::COLL_SIZE, _float3(1.5f, 1.5f, 1.5f));
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_OBJ, m_pBoxColliderCom);
	m_pPlayer = static_cast<CPlayer*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("Layer_Player")));
	m_pMissile = static_cast<CMissile*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("Missile")));
	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

CTargeting* CTargeting::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTargeting* pTargeting = new CTargeting(pGraphic_Device);
	if (FAILED(pTargeting->NativeConstruct_Prototype()))
	{
		MSGBOX("Fail Create Missle")
			Safe_Release(pTargeting);
		return nullptr;
	}
	return pTargeting;


}

CGameObject* CTargeting::Clone(void* pArg)
{
	CTargeting* pTargeting = new CTargeting(*this);
	if (FAILED(pTargeting->NativeConstruct(pArg)))
	{
		MSGBOX("Fail Create CTargeting")
			Safe_Release(pTargeting);
		return nullptr;
	}
	return pTargeting;
}

void CTargeting::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBoxColliderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}

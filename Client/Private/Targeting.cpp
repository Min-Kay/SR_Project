#include "stdafx.h"
#include "..\Public\Targeting.h"
#include "GameInstance.h"
#include "Player.h"
#include "Missile.h"
#include "Transform.h"

CTargeting::CTargeting(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CTargeting::CTargeting(const CTargeting & rhs)
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

HRESULT CTargeting::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;


	m_Target = *static_cast<TARGET*>(pArg);

	m_pBoss = static_cast<CBoss*>(m_Target.Parent);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(2.f, 2.f, 2.f));
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


	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(1)))
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



HRESULT CTargeting::FaceOn_Camera()
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

HRESULT CTargeting::MainMoving(_float fTimeDelta)
{

	BezierPos = BezierCurve(m_Target.Pos1, m_Target.Pos2, m_Target.Pos3, fTimeDelta);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, BezierPos);
	return S_OK;
}



HRESULT CTargeting::ColliderCheck()
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	list<CGameObject*> test = p_instance->Get_Collision_List(m_pBoxColliderCom);
	_float3 Look, playerpos, Targetpos;
	for (auto & iter : test)
	{
		
		if (OBJ_STATIC == iter->Get_Type() && iter != m_Target.Parent )
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
			else
			{
				RELEASE_INSTANCE(CGameInstance)
					return E_FAIL;
			}

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
		FaceOn_Camera();

		timer += fTimeDelta;
		MainMoving(timer);//처음 플레이어 위치 잡는 이미지

	}
	else
	{
		MainMoving(1); //출동하고 위치 고정시킴 이위치에 이제 미사일 쏠거임
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


	Set_Type(OBJ_STATIC);


	m_pTransformCom->Scaled(_float3(1.f, 1.f, 1.f));

	m_pBoxColliderCom->Set_ParentInfo(this);
	m_pBoxColliderCom->Set_CollStyle(CCollider::COLLSTYLE_TRIGGER);
	m_pBoxColliderCom->Set_State(CBoxCollider::COLL_SIZE, _float3(0.5f, 0.5f, 0.5f));
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_OBJ, m_pBoxColliderCom);
	m_pPlayer = static_cast<CPlayer*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("Layer_Player")));
	m_pMissile = static_cast<CMissile*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("Missile")));
	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

CTargeting * CTargeting::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
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

CGameObject * CTargeting::Clone(void * pArg)
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

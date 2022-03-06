#include "stdafx.h"
#include "..\Public\Missile.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "Renderer.h"
#include "VIBuffer_Cube.h"
#include "GameInstance.h"
#include "Player.h"
#include "Arm.h"
#include "Impact.h"
#include "Targeting.h"
#include "Boss.h"

CMissile::CMissile(LPDIRECT3DDEVICE9 m_pGraphic_Device)
	:CEnemy(m_pGraphic_Device)
{
}

CMissile::CMissile(const CMissile& rhs)
	: CEnemy(rhs)
	, m_pTransform(rhs.m_pTransform)
	, m_pTexture(rhs.m_pTexture)
	, m_pVIBuffer(rhs.m_pVIBuffer)
	, m_pRenderer(rhs.m_pRenderer)
	, m_pBoxCollider(rhs.m_pBoxCollider)
{


}
HRESULT CMissile::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMissile::NativeConstruct(void* pArg)
{


	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
	{
		MSGBOX("미사일 셋업오류")
			return E_FAIL;
	}


	m_ArmMissle = *static_cast<ARMMISSLE*>(pArg);


	m_pTransform->Scaled(_float3(2.f, 2.f, 2.f));
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	m_pBoss = static_cast<CBoss*>(m_ArmMissle.pParent);
	switch (m_ArmMissle.ArmMissle)
	{
	case ARMMISSLE_LEFT:
		m_Arm = static_cast<CArm*>(static_cast<CEnemy*>((p_instance->Get_GameObject_End(g_CurrLevel, TEXT("Arm_Left")))));
		break;
	case ATMMISSLE_RIGHT:
		m_Arm = static_cast<CArm*>(static_cast<CEnemy*>((p_instance->Get_GameObject_End(g_CurrLevel, TEXT("Arm_Right")))));
		break;
	}
	CTransform* m_ArmTrans = (CTransform*)m_Arm->Get_Component(COM_TRANSFORM);
	CTransform* BossTrans = (CTransform*)m_pBoss->Get_Component(COM_TRANSFORM);
	BossPos = BossTrans->Get_State(CTransform::STATE_UP);
	RandPos1 = _float3(BossPos.x + rand() % 10 - 5, BossPos.y + rand() % 10 + 35, BossPos.z + rand() % 10 - 5);
	MissilePos = m_ArmTrans->Get_State(CTransform::STATE_POSITION);
	CTransform* PlayerTrans = (CTransform*)m_pPlayer->Get_Component(COM_TRANSFORM);
	PlayerPos = PlayerTrans->Get_State(CTransform::STATE_POSITION);
	//BossPos.y += 5.f;
	m_pTransform->Set_State(CTransform::STATE_POSITION, MissilePos);
	RELEASE_INSTANCE(CGameInstance);


	//RandPos2 = _float3(PlayerPos.x - rand() % 30 - 10, PlayerPos.y, PlayerPos.z);


	m_bTargetCollider = false;
	return S_OK;
}

_int CMissile::Tick(_float fTimeDelta)
{
	if (Get_Dead() || fTimeDelta <= 0.f)
		return 0;

	if (0 > __super::Tick(fTimeDelta))
		return -1;


	if (m_bDEAD)
	{
		m_pBoxCollider->Set_Dead(true);
		Set_Dead(true);
	}

	m_pBoxCollider->Set_Collider();

	if (Get_Portaliing())
	{
		m_pTransform->Go_Straight(fTimeDelta * 10.f);
		return 0;
	}


	if (false == m_bTargetCollider)
	{
		m_fTargetTimer += fTimeDelta;


		CTransform* BossTrans = (CTransform*)m_pBoss->Get_Component(COM_TRANSFORM);
		BossPos = BossTrans->Get_State(CTransform::STATE_UP);


		m_fFront_BezierPos = BezierCurve(BossPos, RandPos1, PlayerPos, RandPos2, m_fTargetTimer);
		Set_Bezier(m_fFront_BezierPos);

	}

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);

	if (Count == 0)
	{
		CTargeting::TARGET targeting;
		targeting.targetPos = m_fFront_BezierPos;
		targeting.MainTarget = true;
		targeting.SubTargetRangeX = 0.0f;
		targeting.SubTargetRangeY = 0.0f;
		if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("Target"), TEXT("Prototype_GameObject_Targeting"), &targeting)))
		{
			RELEASE_INSTANCE(CGameInstance);
			return -1;
		}

		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				CTargeting::TARGET targeting;
				targeting.targetPos = m_fFront_BezierPos;
				targeting.MainTarget = false;
				targeting.SubTargetRangeX = i * 3 - 3.f;
				targeting.SubTargetRangeY = j * 3 - 3.f;
				if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("Target_sub"), TEXT("Prototype_GameObject_Targeting"), &targeting)))
				{
					RELEASE_INSTANCE(CGameInstance);
					return -1;
				}

			}
		}
		++Count;

	}
	else
	{
		CTargeting* ptarget = (CTargeting*)p_instance->Get_GameObject(g_CurrLevel, TEXT("Target"));
		if (nullptr != ptarget)
			m_bTargetCollider = ptarget->Get_CheckCollider();

	}


	if (m_bTargetCollider)
	{
		if (Count == 1)
		{
			MissilePos = m_pTransform->Get_State(CTransform::STATE_POSITION);
			++Count;
		}
		m_fMissileTimer += fTimeDelta * 0.5f;
		if (m_fMissileTimer / 2 >= 1.0f)
		{
			m_bDEAD = true;
			m_fMissileTimer = 0;
			RELEASE_INSTANCE(CGameInstance);
			return 0;
		}
		else
		{
			m_fBezierPos = BezierCurve(MissilePos, RandPos1, PlayerPos, RandPos2, m_fMissileTimer);
			m_pTransform->Set_State(CTransform::STATE_POSITION, m_fBezierPos);
			m_pTransform->LookAt(PlayerPos);

		}
		m_fTargetTimer = 0.f;
	}
	else
	{

		CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
		if (m_ArmMissle.ArmMissle == ARMMISSLE_LEFT && m_ArmMissle.Left)
			m_Arm = static_cast<CArm*>(static_cast<CEnemy*>((p_instance->Get_GameObject_End(g_CurrLevel, TEXT("Arm_Left")))));
		else if (m_ArmMissle.ArmMissle == ATMMISSLE_RIGHT && m_ArmMissle.Right)
			m_Arm = static_cast<CArm*>(static_cast<CEnemy*>((p_instance->Get_GameObject_End(g_CurrLevel, TEXT("Arm_Right")))));

		CTransform* m_ArmTrans = (CTransform*)m_Arm->Get_Component(COM_TRANSFORM);
		m_pTransform->Set_State(CTransform::STATE_POSITION, m_ArmTrans->Get_State(CTransform::STATE_POSITION));
		RELEASE_INSTANCE(CGameInstance);
	}


	if (m_pPlayer)
	{

		list<CGameObject*> test = p_instance->Get_Collision_List(m_pBoxCollider);

		for (auto& iter : test)
		{
			if (OBJ_PLAYER == iter->Get_Type())
			{
				//m_bDEAD = true;
				//m_pPlayer->Add_Hp(-50);
				//MSGBOX("충돌!!")
			}
			else if (OBJ_STATIC == iter->Get_Type())
			{

				//m_bDEAD = true;

				//MSGBOX("충돌!!")
				//Set_Portaling(true);

			}


		}
	}

	

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CMissile::LateTick(_float fTimeDelta)
{
	if (Get_Dead() || fTimeDelta <= 0.f)
		return 0;

	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}



HRESULT CMissile::Render()
{
	if (Get_Dead())
		return 0;


	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	if (FAILED(m_pTransform->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(m_pTexture->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pVIBuffer->Render();

	return S_OK;

}


HRESULT CMissile::Set_Bezier(_float3  Pos)
{

	m_fFront_BezierPos = Pos;
	return S_OK();
}

HRESULT CMissile::SetUp_Component()
{
	CTransform::TRANSFORMDESC desc;
	desc.fRotationPerSec = 90.f;
	desc.fSpeedPerSec = 5.f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransform, &desc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLLIDER, COM_COLLIDER, (CComponent**)&m_pBoxCollider)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_CUBE, COM_BUFFER, (CComponent**)&m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Missile"), COM_TEXTURE, (CComponent**)&m_pTexture)))
		return E_FAIL;


	m_pBoxCollider->Set_ParentInfo(this);
	Set_Type(OBJ_ENEMY);
	m_pBoxCollider->Set_CollStyle(CCollider::COLLSTYLE_TRIGGER);
	m_pBoxCollider->Set_State(CBoxCollider::COLL_SIZE, _float3(1.f, 1.f, 1.f));
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_OBJ,m_pBoxCollider);

	m_pPlayer = static_cast<CPlayer*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("Layer_Player")));
	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

_float3 CMissile::BezierCurve(_float3 P0, _float3 P1, _float3 P2, _float3 P3, _float time)
{

	_float3 vTesult1;
	_float3 vTesult2;
	_float3 vTesult3;
	//	_float3 vTesult4;
	//	_float3 vTesult5;
	_float3 vTesult6;

	D3DXVec3Lerp(&vTesult1, &P0, &P1, time);
	D3DXVec3Lerp(&vTesult2, &P1, &P2, time);
	//	D3DXVec3Lerp(&vTesult3, &P2, &P3, time);
	D3DXVec3Lerp(&vTesult6, &vTesult1, &vTesult2, time);
	//	D3DXVec3Lerp(&vTesult4, &vTesult1, &vTesult2, time);
	//	D3DXVec3Lerp(&vTesult5, &vTesult2, &vTesult3, time);
	//	D3DXVec3Lerp(&vTesult6, &vTesult4, &vTesult5, time);

	return vTesult6;
}

CMissile* CMissile::Create(LPDIRECT3DDEVICE9 m_pGraphic_Device)
{
	CMissile* pMissile = new CMissile(m_pGraphic_Device);
	if (FAILED(pMissile->NativeConstruct_Prototype()))
	{
		MSGBOX("Fail Create Missle")
			Safe_Release(pMissile);
		return nullptr;
	}
	return pMissile;
}

CGameObject* CMissile::Clone(void* pArg)
{
	CMissile* pMissile = new CMissile(*this);
	if (FAILED(pMissile->NativeConstruct(pArg)))
	{
		MSGBOX("Fail Create Missle_Clone")
			Safe_Release(pMissile);
		return nullptr;
	}
	return pMissile;
}

void CMissile::Free()
{
	__super::Free();
	Safe_Release(m_pTransform);
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
}

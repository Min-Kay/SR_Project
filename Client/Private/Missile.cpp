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

	SetUp_First();

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->Play_Sound(TEXT("Missile_Move.wav"), CSoundMgr::ADDITIONAL_EFFECT2, 1.f);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CMissile::Tick(_float fTimeDelta)
{
	if (Get_Dead() || fTimeDelta <= 0.f)
		return 0;

	if (0 > __super::Tick(fTimeDelta))
		return -1;

	if (Check_Dead())
		return 0;

	m_pBoxCollider->Set_Collider();

	Check_ColliderTarget();

	if (Get_Portaling())
	{
		m_pTransform->Go_Straight(fTimeDelta *5.f);
		Set_Portaling(true);
	}
	else
	{
		First_Bezier(fTimeDelta);

		Targeting_Main_Sub();

		Missle_Move(fTimeDelta);
	}

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
	return S_OK;
}

void CMissile::Impact(_float3 _Pos)
{
	CImpact::IMPACT Impact1;
	ZeroMemory(&Impact1, sizeof(Impact1));
	Impact1.Pos = _Pos;
	Impact1.Size = _float3(0.05f, 0.05f, 0.05f);
	Impact1.randomPos = 5;
	Impact1.Speed = 5;
	Impact1.deleteCount = 1;//rand() % 5 + 2;
	Impact1.DeleteImpact = false;

	Impact1.Gradation = CImpact::GRADATION_DOWN;
	Impact1.Color = D3DXCOLOR(1.0f, 0.9f, 0.0f, 0.0f);
	Impact1.ChangeColor = D3DXCOLOR(0.0f, 0.05f, 0.0f, 0.0f);

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	for (int i = 0; i < rand() % 5 + 10; ++i)
	{
		if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("Impact"), TEXT("Prototype_GameObject_Impact"), &Impact1)))
		{
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
	}
	RELEASE_INSTANCE(CGameInstance);
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
	m_pBoxCollider->Set_State(CBoxCollider::COLL_SIZE, _float3(1.5f, 1.5f, 1.5f));

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_OBJ, m_pBoxCollider);
	m_pPlayer = static_cast<CPlayer*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("Layer_Player")));
	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

HRESULT CMissile::SetUp_First()
{
	Add_RandomPos = _float3(rand() % 10 - 5.f, rand() % 10 - 5.f, rand() % 10 - 5.f);
	m_Damage = 20;
	m_pTransform->Scaled(_float3(2.f, 2.f, 2.f));
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	m_pBoss = static_cast<CBoss*>(static_cast<CEnemy*>((p_instance->Get_GameObject_End(g_CurrLevel, TEXT("Boss")))));
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
	m_pTransform->Set_State(CTransform::STATE_POSITION, MissilePos);
	RELEASE_INSTANCE(CGameInstance);
	m_bTargetCollider = false;
	return S_OK;
}

_bool CMissile::Check_Dead()
{
	if (m_bDEAD)
	{
		m_pBoxCollider->Set_Dead(true);
		Set_Dead(true);
		return true;
	}

	return false;
}

HRESULT CMissile::Check_ColliderTarget()
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	if (m_pPlayer)
	{
		list<CGameObject*> test = p_instance->Get_Collision_Object_List(m_pBoxCollider);

		test.sort([](CGameObject* a, CGameObject* b) { return a->Get_Type() > b->Get_Type();  });
		for (auto& iter : test)
		{
			if (OBJ_PLAYER == iter->Get_Type())
			{
				m_pPlayer->Set_Shake(0.5f, 1.f);
				m_pPlayer->Add_Hp(-m_Damage);
				m_bDEAD = true;
				Impact(m_pTransform->Get_State(CTransform::STATE_POSITION));

				p_instance->StopSound(CSoundMgr::WEAPON_EFFECT3);
				p_instance->Play_Sound(rand() % 2 == 0 ? TEXT("Explosion_Missile_0.wav") : TEXT("Explosion_Missile_1.wav"), CSoundMgr::WEAPON_EFFECT3, 1.f);

			}
			else if (Get_Portaling() && iter == m_pBoss)
			{
				m_pPlayer->Set_Shake(0.5f, 1.f);
				if (m_pBoss->Get_OnShield())
					m_pBoss->Add_ShieldHp(-m_Damage);
				else
					m_pBoss->Add_HP(-m_Damage);
				m_bDEAD = true;
				Impact(m_pTransform->Get_State(CTransform::STATE_POSITION));
				p_instance->StopSound(CSoundMgr::WEAPON_EFFECT3);
				p_instance->Play_Sound(rand() % 2 == 0 ? TEXT("Explosion_Missile_0.wav") : TEXT("Explosion_Missile_1.wav"), CSoundMgr::WEAPON_EFFECT3, 1.f);
			}
			else if(OBJ_STATIC == iter->Get_Type())
			{
				m_pPlayer->Set_Shake(0.5f, 1.f);
				m_bDEAD = true;
				Impact(m_pTransform->Get_State(CTransform::STATE_POSITION));
				p_instance->StopSound(CSoundMgr::WEAPON_EFFECT3);
				p_instance->Play_Sound(rand() % 2 == 0 ? TEXT("Explosion_Missile_0.wav") : TEXT("Explosion_Missile_1.wav"), CSoundMgr::WEAPON_EFFECT3, 1.f);	RELEASE_INSTANCE(CGameInstance);
			}
		}
	}
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CMissile::First_Bezier(_float fTimeDelta)
{
	//처음 타게팅 이미지 위치 베지어곡선
	if (false == m_bTargetCollider)
	{
		m_fTargetTimer += fTimeDelta;

		CTransform* BossTrans = (CTransform*)m_pBoss->Get_Component(COM_TRANSFORM);
		BossPos = BossTrans->Get_State(CTransform::STATE_UP);

		m_fFront_BezierPos = BezierCurve(BossPos, RandPos1, PlayerPos, RandPos2, m_fTargetTimer);
		Set_Bezier(m_fFront_BezierPos);

	}
	return S_OK;
}

HRESULT CMissile::Targeting_Main_Sub()
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	//타겟에게 날라가는 타게팅이미지 메인 타케팅이미지와 주변에 그릴 서브 이미지까지2개
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
	else//한번 계산하고 다음에 오면 타겟의 충돌이있나 확인
	{
		pTarget = (CTargeting*)p_instance->Get_GameObject_End(g_CurrLevel, TEXT("Target"));
		if (nullptr != pTarget)
			m_bTargetCollider = pTarget->Get_CheckCollider();

		 targetTrans = static_cast<CTransform*>(pTarget->Get_Component(COM_TRANSFORM));

	}
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CMissile::Missle_Move(_float fTimeDelta)
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	//만약 타겟이 충돌(벽에 )했으면 날린다
	if (m_bTargetCollider)
	{

		m_fMissileTimer += fTimeDelta * 0.5f;
		if (m_fMissileTimer * 0.5f >= 1.0f) //시간 시나면 사라짐
		{
			if (!Get_Portaling())//포탈 통과아니라면 시간이 지나서사라진다
			{
				m_bDEAD = true;
				m_fMissileTimer = 0;
				RELEASE_INSTANCE(CGameInstance);
				return 0;

			}
		}
		else//아니면 베지어로 이동
		{
			m_pTransform->Turn(_float3(1.f, 1.f, 0.f), fTimeDelta * 10.f);
			m_fBezierPos = BezierCurve(MissilePos, RandPos1, targetTrans->Get_State(CTransform::STATE_POSITION), RandPos2, m_fMissileTimer * 0.8f);
			m_pTransform->Set_State(CTransform::STATE_POSITION, m_fBezierPos);
			_float3 MissleLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
			D3DXVec3Normalize(&MissleLook, &MissleLook);
			Impact(m_pTransform->Get_State(CTransform::STATE_POSITION) - MissleLook*0.5f);
		}
		m_fTargetTimer = 0.f;//처음 타겟시간 초기화
	}

	else //충돌전 미사일 손에서 대기?
	{

		CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
		if (m_ArmMissle.ArmMissle == ARMMISSLE_LEFT && m_ArmMissle.Left)
			m_Arm = static_cast<CArm*>(static_cast<CEnemy*>((p_instance->Get_GameObject_End(g_CurrLevel, TEXT("Arm_Left")))));
		else if (m_ArmMissle.ArmMissle == ATMMISSLE_RIGHT && m_ArmMissle.Right)
			m_Arm = static_cast<CArm*>(static_cast<CEnemy*>((p_instance->Get_GameObject_End(g_CurrLevel, TEXT("Arm_Right")))));

		CTransform* m_ArmTrans = (CTransform*)m_Arm->Get_Component(COM_TRANSFORM);
		m_pTransform->Set_State(CTransform::STATE_POSITION, m_ArmTrans->Get_State(CTransform::STATE_POSITION));


		//Impact(_float3 _Pos)
		RELEASE_INSTANCE(CGameInstance);
	}
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}



_float3 CMissile::BezierCurve(_float3 P0, _float3 P1, _float3 P2, _float3 P3, _float time)
{

	_float3 vTesult1;
	_float3 vTesult2;
	_float3 vTesult3;

	D3DXVec3Lerp(&vTesult1, &P0, &P1, time);
	D3DXVec3Lerp(&vTesult2, &P1, &P2, time);

	D3DXVec3Lerp(&vTesult3, &vTesult1, &vTesult2, time);


	return vTesult3;
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

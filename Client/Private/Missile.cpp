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
#include "Shader.h"

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
	,m_pShader(rhs.m_pShader)
{
	Safe_AddRef(m_pTexture);
	Safe_AddRef(m_pTransform);
	Safe_AddRef(m_pRenderer);
	Safe_AddRef(m_pVIBuffer);
	Safe_AddRef(m_pShader);
	Safe_AddRef(m_pBoxCollider);
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
	m_pBoss =(CBoss*)m_ArmMissle.pParent;
	 CTransform* BossTR= static_cast<CTransform*>(m_pBoss->Get_Component(COM_TRANSFORM));
	m_pTransform->Set_State(CTransform::STATE_POSITION, BossTR->Get_State(CTransform::STATE_POSITION));
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

	m_fMissileTimer += fTimeDelta;

	m_pBoxCollider->Set_Collider();

	Check_ColliderTarget();

	if (Get_Portaling())//포탈 통과했으면
	{
		m_pTransform->Go_Straight(fTimeDelta *8.f);
		Set_Portaling(true);
	}
	else //일반상태
	{
		Missle_Move(m_fMissileTimer);
	}

	
	if (m_bDEADcount == true && !Get_Portaling())
	{
		m_fdeadCounter += fTimeDelta;
		
		if (m_fdeadCounter >= 1.f)
		{
			m_bDEAD = true;
		}
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

	m_pTransform->Bind_OnShader(m_pShader);
	m_pShader->SetUp_ValueOnShader("g_ColorStack", &g_ControlShader, sizeof(_float));
	
	m_pTexture->Bind_OnShader(m_pShader, "g_Texture", 0);

	m_pShader->Begin_Shader(SHADER_SETCOLOR_CUBE);
	m_pVIBuffer->Render();
	m_pShader->End_Shader();

	return S_OK;

}



void CMissile::Impact(_float3 _Pos)
{
	CImpact::IMPACT Impact1;
	ZeroMemory(&Impact1, sizeof(Impact1));
	Impact1.Position = _Pos;
	Impact1.Size = _float3(0.05f, 0.05f, 0.05f);
	Impact1.RandomDirection = 5;
	Impact1.SpreadSpeed = 5;
	Impact1.DeleteTime = 1.f;//rand() % 5 + 2;
	Impact1.Change = true;
	Impact1.Color = _float4(1.f, 0.9f, 0.f, 0.f);
	Impact1.EndColor = _float4(0.0f, 0.05f, 0.0f, 0.0f);

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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_SHADER_CUBE, COM_SHADER, (CComponent**)&m_pShader)))
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
	m_Damage = 20;
	m_pTransform->Scaled(_float3(2.f, 2.f, 2.f));
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
		list<CGameObject*> test = p_instance->Get_Collision_List(m_pBoxCollider);

		//test.sort([](CGameObject* a, CGameObject* b) { return a->Get_Type() > b->Get_Type();  });
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
			else if(OBJ_STATIC == iter->Get_Type() /*&& iter != m_ArmMissle.mainTarget && iter != m_ArmMissle.pTargeting && iter != m_pBoss*/)
			{
				m_pPlayer->Set_Shake(0.5f, 1.f);
				m_bDEADcount = true;
				Impact(m_pTransform->Get_State(CTransform::STATE_POSITION));
				p_instance->StopSound(CSoundMgr::WEAPON_EFFECT3);
				p_instance->Play_Sound(rand() % 2 == 0 ? TEXT("Explosion_Missile_0.wav") : TEXT("Explosion_Missile_1.wav"), CSoundMgr::WEAPON_EFFECT3, 1.f);	
			}
		}
	}
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}


HRESULT CMissile::Missle_Move(_float fTimeDelta)
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	
			m_pTransform->Turn(_float3(1.f, 1.f, 0.f), fTimeDelta * 10.f);

			m_fBezierPos = BezierCurve(m_ArmMissle.Pos1, m_ArmMissle.Pos2, m_ArmMissle.Pos3, fTimeDelta);
			m_pTransform->Set_State(CTransform::STATE_POSITION, m_fBezierPos);

			_float3 MissleLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
			D3DXVec3Normalize(&MissleLook, &MissleLook);
			Impact(m_pTransform->Get_State(CTransform::STATE_POSITION) - MissleLook*0.5f);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}



_float3 CMissile::BezierCurve(_float3 P0, _float3 P1, _float3 P2, _float time)
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
	Safe_Release(m_pShader);

}

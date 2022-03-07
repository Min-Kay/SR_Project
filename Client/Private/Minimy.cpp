#include "stdafx.h"
#include "Minimy.h"

#include "Transform.h"
#include "Renderer.h"
#include "BoxCollider.h"
#include "Texture.h"
#include "VIBuffer_Cube.h"
#include "GameInstance.h"
#include "Impact.h"
#include "Shield.h"


CMinimy::CMinimy(LPDIRECT3DDEVICE9 m_pGraphic_Device)
	:CEnemy(m_pGraphic_Device)
{
}

CMinimy::CMinimy(const CMinimy& rhs)
	:CEnemy(rhs)
	,m_pTransform(rhs.m_pTransform)
	,m_pCollider(rhs.m_pCollider)
	,m_pOnlyRotation(rhs.m_pOnlyRotation)
	,m_pTexture(rhs.m_pTexture)
	,m_pBuffer(rhs.m_pBuffer)
	,m_pRenderer(rhs.m_pRenderer)
{
	Safe_AddRef(m_pTransform);
	Safe_AddRef(m_pCollider);
	Safe_AddRef(m_pOnlyRotation);
	Safe_AddRef(m_pBuffer);
	Safe_AddRef(m_pRenderer);
	Safe_AddRef(m_pTexture);


}

HRESULT CMinimy::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMinimy::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	return S_OK;
}

_int CMinimy::Tick(_float fTimeDelta)
{
	if (Get_Dead() || fTimeDelta <= 0.f)
		return 0;

	if (0 > __super::Tick(fTimeDelta))
		return -1;

	if(Check_HP() && m_State != MINIMY_DIE)
	{
		m_State = MINIMY_DIE;
		Set_Type(OBJ_INTERACTION);
	}

	State_Machine(fTimeDelta);

	Synchronize_Transform();

	return 0;
}

_int CMinimy::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return 0;
}

HRESULT CMinimy::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pOnlyRotation->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(m_pTexture->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pBuffer->Render();

	return S_OK;
}

HRESULT CMinimy::SetUp_Component()
{
	CTransform::TRANSFORMDESC desc;
	desc.fRotationPerSec = 90.f;
	desc.fSpeedPerSec = 5.f;

	if(FAILED(__super::Add_Component(LEVEL_STATIC,PROTO_TRANSFORM,COM_TRANSFORM,(CComponent**)&m_pTransform,&desc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Minimy"), COM_TEXTURE, (CComponent**)&m_pTexture)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLLIDER, COM_COLLIDER, (CComponent**)&m_pCollider)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRenderer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, TEXT("OnlyRotation"), (CComponent**)&m_pOnlyRotation)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_CUBE, COM_BUFFER, (CComponent**)&m_pBuffer)))
		return E_FAIL;

	Set_Type(OBJ_ENEMY);

	m_pTransform->Scaled(m_Size);
	m_pOnlyRotation->Scaled(m_Size);


	m_pCollider->Set_CollStyle(CCollider::COLLSTYLE_ENTER);
	m_pCollider->Set_ParentInfo(this);

	m_pCollider->Set_State(CBoxCollider::COLL_SIZE, m_Size);

	m_Hp = 30;
	m_Damage = 10;
	m_EnemyType = ENEMY_CUBEMONSTER;
	m_CanPortal = true;

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_OBJ, m_pCollider); 
	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

void CMinimy::Synchronize_Transform()
{
	m_pOnlyRotation->Set_State(CTransform::STATE_POSITION, m_pTransform->Get_State(CTransform::STATE_POSITION));
	m_pCollider->Set_Collider();
}

void CMinimy::Gravity_Blowing(_float fTimeDelta, _bool _watchPlayer)
{
	if (fTimeDelta <= 0.f)
		return;

	m_fTimer += fTimeDelta;

	m_pTransform->Gravity(0.3f, fTimeDelta);
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	list<CCollision_Manager::COLLPOINT> collList = p_instance->Get_Ray_Collision_List(-m_pTransform->Get_State(CTransform::STATE_UP), m_pTransform->Get_State(CTransform::STATE_POSITION), 100, true);

	if (collList.empty() || collList.size() == 1)
	{
		RELEASE_INSTANCE(CGameInstance);
		return;
	}


	auto iter = collList.begin();

	for (; iter != collList.end();)
	{
		if (iter->CollObj->Get_Type() != OBJ_STATIC)
			++iter;
		else
			break;
	}

	if (iter == collList.end())
	{
		RELEASE_INSTANCE(CGameInstance);
		return;
	}

	if (_watchPlayer)
		m_pOnlyRotation->LookAt(m_pPlayerTr->Get_State(CTransform::STATE_POSITION));

	m_pTransform->Set_State(CTransform::STATE_POSITION, (*iter).Point + _float3(0.f, 2.f, 0.f) /*+ _float3(0.f, 1.f, 0.f) * sinf(D3DXToDegree(m_fTimer * 0.01f) * 0.5f)*/);

	RELEASE_INSTANCE(CGameInstance);
}


void CMinimy::Add_HP(_int _add)
{
	__super::Add_HP(_add);
}

void CMinimy::State_Machine(_float fTimeDelta)
{
	switch (m_State)
	{
	case MINIMY_IDLE:
		Idle(fTimeDelta);
		break;
	case MINIMY_MOVE:
		Move(fTimeDelta);
		break;
	case MINIMY_ATTACK:
		Attack(fTimeDelta);
		break;
	case MINIMY_DIE:
		Die(fTimeDelta);
		break;
	}
}

_bool CMinimy::Sizing(_float fTimeDelta)
{
	m_fTimer += fTimeDelta;
	if (!m_Sizing)
	{
		Impact();
		m_pTransform->Turn(_float3(0.f,1.f,1.f), fTimeDelta);
		m_pTransform->Scaled(_float3(m_fTimer, m_fTimer, m_fTimer));
		if (m_fTimer >= m_Size.x)
		{
			m_pTransform->Scaled(m_Size);
			m_Sizing = true;
			m_fTimer = 0.f;

		}
		return false;
	}

	return true;
	
}

void CMinimy::Idle(_float fTimeDelta)
{
	if (Sizing(fTimeDelta))
		Gravity_Blowing(fTimeDelta);
}

void CMinimy::Move(_float fTimeDelta)
{
	Gravity_Blowing(fTimeDelta);
}

void CMinimy::Attack(_float fTimeDelta)
{
}

void CMinimy::Die(_float fTimeDelta)
{
	m_pTransform->Gravity(1.0f, fTimeDelta);
	m_pTransform->Add_Force(fTimeDelta);


	if(m_pTransform->Get_Force() >= 20.f)
	{
		CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
		list<CGameObject*> colllist = p_instance->Get_Collision_Object_List(m_pCollider);

		if (colllist.empty())
		{
			RELEASE_INSTANCE(CGameInstance);
			return;
		}

		for (auto& obj : colllist)
		{
			if (obj->Get_Type() == OBJ_ENEMY)
			{
				if (static_cast<CEnemy*>(obj)->Get_EnemyType() == ENEMY_SHIELD)
				{
					static_cast<CShield*>(obj)->Add_ShieldHp(-(m_Damage * 10));
				}
			}
			Impact();
			m_pCollider->Set_Dead(true);
			Set_Dead(true);
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
		RELEASE_INSTANCE(CGameInstance);
	}
}


void CMinimy::Impact()
{
	CImpact::IMPACT Impact1;
	ZeroMemory(&Impact1, sizeof(Impact1));
	Impact1.Pos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	Impact1.Size = _float3(0.08f, 0.08f, 0.08f);
	Impact1.randomPos = 5;
	Impact1.Speed = 5;
	Impact1.deleteCount = 1;//rand() % 5 + 2;
	Impact1.DeleteImpact = false;
	Impact1.Color = D3DXCOLOR(1.0f, 0.9f, 0.0f, 0.0f);

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


void CMinimy::Screw(_float fTimeDelta)
{
}

void CMinimy::Company_Fire(_float fTimeDelta)
{
}

CMinimy* CMinimy::Create(LPDIRECT3DDEVICE9 m_pGraphic_Device)
{
	CMinimy* p_minimy = new CMinimy(m_pGraphic_Device);

	if(FAILED(p_minimy->NativeConstruct_Prototype()))
	{
		Safe_Release(p_minimy);
		return nullptr;
	}

	return p_minimy;
}

CGameObject* CMinimy::Clone(void* pArg)
{

	CMinimy* p_minimy = new CMinimy(*this);

	if (FAILED(p_minimy->NativeConstruct(pArg)))
	{
		Safe_Release(p_minimy);
		return nullptr;
	}

	return p_minimy;
}

void CMinimy::Free()
{
	__super::Free();
	Safe_Release(m_pTransform);
	Safe_Release(m_pCollider);
	Safe_Release(m_pOnlyRotation);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pTexture);

}
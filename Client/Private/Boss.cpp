#include "stdafx.h"
#include "Boss.h"

#include "Texture.h"
#include "Transform.h"
#include "Renderer.h"
#include "VIBuffer_Cube.h"
#include "BoxCollider.h"
#include "GameInstance.h"

#include "Arm.h"
#include "Player.h"
#include "Texture.h"


CBoss::CBoss(LPDIRECT3DDEVICE9 m_pGraphic_Device)
	:CEnemy(m_pGraphic_Device)
{
}

CBoss::CBoss(const CBoss& rhs)
	:CEnemy(rhs)
	,m_pTransform(rhs.m_pTransform)
	, m_pOnlyRotation(rhs.m_pOnlyRotation)
	,m_pTexture(rhs.m_pTexture)
	,m_pRenderer(rhs.m_pRenderer)
	,m_pCollider(rhs.m_pCollider)
	,m_pBuffer(rhs.m_pBuffer)
{
	Safe_AddRef(m_pOnlyRotation);
	Safe_AddRef(m_pTransform);
	Safe_AddRef(m_pTexture);
	Safe_AddRef(m_pRenderer);
	Safe_AddRef(m_pCollider);
	Safe_AddRef(m_pBuffer);

}

HRESULT CBoss::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CBoss::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	return S_OK;
}

_int CBoss::Tick(_float fTimeDelta)
{
	if (Get_Dead() || fTimeDelta <= 0.f)
		return 0;

	if (0 > __super::Tick(fTimeDelta))
		return -1;

	m_pCollider->Set_Collider();

	State_Machine(fTimeDelta);

	Synchronize_Transform();

	return 0;
}

_int CBoss::LateTick(_float fTimeDelta)
{
	if (Get_Dead() )
		return 0;

	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return 0;
}

HRESULT CBoss::Render()
{
	if (Get_Dead())
		return 0;

	if (FAILED(m_pOnlyRotation->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(m_pTexture->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pBuffer->Render(); 

	if (FAILED(__super::Render()))
		return E_FAIL;
	return S_OK; 
}

HRESULT CBoss::SetUp_Component()
{
	CTransform::TRANSFORMDESC desc;
	desc.fRotationPerSec = 90.f;
	desc.fSpeedPerSec = 5.f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, TEXT("OnlyRotation"), (CComponent**)&m_pOnlyRotation, &desc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransform, &desc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLLIDER, COM_COLLIDER, (CComponent**)&m_pCollider)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRenderer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_CUBE, COM_BUFFER, (CComponent**)&m_pBuffer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CubeMonster"), COM_TEXTURE, (CComponent**)&m_pTexture)))
		return E_FAIL;


	m_pTransform->Scaled(_float3(5.f, 5.f, 5.f));
	m_pOnlyRotation->Scaled(_float3(5.f, 5.f, 5.f));


	_float3 vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
	_float3 vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	D3DXVec3Normalize(&vRight, &vRight);


	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);

	m_pPlayer = static_cast<CPlayer*>(p_instance->Get_GameObject_End(g_CurrLevel, TEXT("Layer_Player")));

	
	if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("Arm"), TEXT("Prototype_GameObject_Arm"))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_LeftArm = static_cast<CArm*>(static_cast<CEnemy*>((p_instance->Get_GameObject_End(g_CurrLevel, TEXT("Arm")))));
	m_LeftArm->Set_Parent(this);
	m_LeftArm->Set_Position(vPos - vRight * 3.f);
	m_LeftArm->Set_Player(m_pPlayer);
	m_LeftArmTr = static_cast<CTransform*>(m_LeftArm->Get_Component(COM_TRANSFORM));


	if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("Arm"), TEXT("Prototype_GameObject_Arm"))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_RightArm = static_cast<CArm*>(static_cast<CEnemy*>((p_instance->Get_GameObject_End(g_CurrLevel, TEXT("Arm")))));
	m_RightArm->Set_Parent(this);
	m_RightArm->Set_Position(vPos + vRight * 3.f);
	m_RightArm->Set_Player(m_pPlayer);
	m_RightArmTr = static_cast<CTransform*>(m_RightArm->Get_Component(COM_TRANSFORM));


	Set_Type(OBJ_ENEMY);

	m_pCollider->Set_ParentInfo(this);
	m_pCollider->Set_CollStyle(CCollider::COLLSTYLE_ENTER);
	m_pCollider->Set_State(CBoxCollider::COLL_SIZE, _float3(5.f, 5.f, 5.f));

	p_instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_OBJ, m_pCollider);
	RELEASE_INSTANCE(CGameInstance);

	if (!m_pPlayer || !m_RightArm || !m_LeftArm)
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_Hp = 100;
	m_Damage = 10;

	return S_OK; 

}

_bool CBoss::InitArmPosition(_float fTimeDelta)
{
	if (!m_LeftArmTr || !m_RightArmTr)
		return false;

	if(!initPos)
	{
		_float3 vPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
		_float3 vUp = m_pTransform->Get_State(CTransform::STATE_UP);
		_float3 vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
		_float3 vScale = m_pTransform->Get_Scale() * 0.5f;

		D3DXVec3Normalize(&vUp, &vUp);
		D3DXVec3Normalize(&vRight, &vRight);

		m_LeftTimer = 0.f;
		m_RightTimer = 0.f;

		Set_ArmPos(ARM_LEFT,m_LeftArmTr->Get_State(CTransform::STATE_POSITION), vPos + vUp * 10.f - vRight * 20.f, vPos + vUp - vRight * 5.f);
		Set_ArmPos(ARM_RIGHT, m_RightArmTr->Get_State(CTransform::STATE_POSITION), vPos + vUp * 10.f + vRight * 20.f, vPos + vUp + vRight * 5.f);

		idlePos = false;
		initPos = true;
	}

	if(!idlePos)
	{
		_bool leftReach  = Move_By_Bazier(ARM_LEFT,fTimeDelta);
		_bool rightReadch = Move_By_Bazier(ARM_RIGHT, fTimeDelta);

		if (leftReach && rightReadch)
		{
			initPos = false;
			idlePos = true;
			return true;
		}
	}
	return false;
}

void CBoss::Synchronize_Transform()
{
	m_pOnlyRotation->Set_State(CTransform::STATE_POSITION, m_pTransform->Get_State(CTransform::STATE_POSITION));
}

void CBoss::Init_Idle()
{
	m_LeftTimer = 0.f;
	m_RightTimer = 0.f;
	initPos = false;
	idlePos = true;
}

void CBoss::Reset_Arm_Direction(ARM _arm)
{
	_float3 vRight, vLook, vUp , vScale;
	vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
	vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
	vUp = m_pTransform->Get_State(CTransform::STATE_UP);

	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vUp, &vUp);

	switch (_arm)
	{
	case ARM_LEFT:
		vScale = m_LeftArmTr->Get_Scale();

		m_LeftArmTr->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
		m_LeftArmTr->Set_State(CTransform::STATE_LOOK, vLook * vScale.y);
		m_LeftArmTr->Set_State(CTransform::STATE_UP, vUp * vScale.z);
		m_LeftArm->Synchronize_Direction();
		m_LeftArm->Set_Rolling(false);
		break;
	case ARM_RIGHT:
		vScale = m_RightArmTr->Get_Scale();

		m_RightArmTr->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
		m_RightArmTr->Set_State(CTransform::STATE_LOOK, vLook * vScale.y);
		m_RightArmTr->Set_State(CTransform::STATE_UP, vUp * vScale.z);
		m_RightArm->Synchronize_Direction();
		m_RightArm->Set_Rolling(false);
		break;
	}
}

void CBoss::Set_ArmPos(ARM _arm, _float3 _start, _float3 _mid, _float3 _end)
{
	switch (_arm)
	{
	case ARM_LEFT:
		leftArmBazier[0] = _start;
		leftArmBazier[1] = _mid;
		leftArmBazier[2] = _end;
		break;
	case ARM_RIGHT:
		rightArmBazier[0] = _start;
		rightArmBazier[1] = _mid;
		rightArmBazier[2] = _end;
		break;
	}
}

void CBoss::Blowing(_float fTimeDelta)
{
	m_fTimer += fTimeDelta;

	
}

_bool CBoss::Move_By_Bazier(ARM _arm , _float fTimeDelta)
{
	switch(_arm)
	{
	case ARM_LEFT:
		if (m_LeftTimer >= 1.f ||  2.f >= D3DXVec3Length(&(m_LeftArmTr->Get_State(CTransform::STATE_POSITION) - leftArmBazier[2])))
		{
			m_LeftTimer = 0.f;
			Reset_Arm_Direction(ARM_LEFT);
			

			return true;
		}

		m_LeftTimer += fTimeDelta * 0.7f;
		m_LeftArmTr->Set_State(CTransform::STATE_POSITION, pow(1 - m_LeftTimer, 2) * leftArmBazier[0] + 2 * m_LeftTimer * (1 - m_LeftTimer) * leftArmBazier[1] + pow(m_LeftTimer, 2) * leftArmBazier[2]);
		break;
	case ARM_RIGHT:
		if (m_RightTimer >= 1.f || 2.f >= D3DXVec3Length(&(m_RightArmTr->Get_State(CTransform::STATE_POSITION) - rightArmBazier[2])))
		{
			m_RightTimer = 0.f;

			Reset_Arm_Direction(ARM_RIGHT);

			return true;
		}

		m_RightTimer += fTimeDelta * 0.7f;
		m_RightArmTr->Set_State(CTransform::STATE_POSITION, pow(1 - m_RightTimer, 2) * rightArmBazier[0] + 2 * m_RightTimer * (1 - m_RightTimer) * rightArmBazier[1] + pow(m_RightTimer, 2) * rightArmBazier[2]);
		break;
	}
	return false;

}

void CBoss::State_Machine(_float fTimeDelta)
{
	m_fTimer += fTimeDelta;

	switch (m_State)
	{
	case BOSS_IDLE:
		Idle(fTimeDelta);
		break;
	case BOSS_ATTACK:
		Phase(fTimeDelta);
		break;
	case BOSS_PHASECHANGE:
		break; 
	case BOSS_DIE:
		Die(fTimeDelta);
		break;
	}
}

void CBoss::Idle(_float fTimeDelta)
{
	// 멍때리기

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);

	if(p_instance->Get_Key_Press(DIK_P))
	{
		_float3 vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
		D3DXVec3Normalize(&vLook, &vLook);
		m_LeftArmTr->Go_Straight(fTimeDelta);
		m_RightArmTr->Go_Straight(fTimeDelta);

		if (m_LeftArm->Get_OnCollide())
			m_LeftArm->Set_Rolling(true);

		if (m_RightArm->Get_OnCollide())
			m_RightArm->Set_Rolling(true);
	}

	if (p_instance->Get_Key_Down(DIK_O))
	{
		m_test = !m_test;
		
	}

	RELEASE_INSTANCE(CGameInstance);

	if (m_test && InitArmPosition(fTimeDelta))
	{
		Blowing(fTimeDelta);
	}
}

void CBoss::Phase(_float fTimeDelta)
{
	switch (m_Phase)
	{
	case BOSS_PHASEONE:
		Attack(fTimeDelta);
		break;
	case BOSS_PHASETWO:
		//페이즈 2
		break;
	}
}


void CBoss::Attack(_float fTimeDelta)
{
	// 1페이즈 패턴 구현
	switch (m_AttState)
	{
	case BOSSATT_PUNCH:
		Attack_Punch(fTimeDelta);
		break;
	case BOSSATT_MISSILE:
		Attack_Missile(fTimeDelta);
		break;
	case BOSSATT_MIXED:
		Attack_Mixed(fTimeDelta);
		break;
	}
}

void CBoss::Die(_float fTimeDelta)
{
	// 사망
}

void CBoss::Attack_Missile(_float fTimeDelta)
{
	// 미사일
}

void CBoss::Attack_Punch(_float fTimeDelta)
{
	// 펀치

}

void CBoss::Attack_Mixed(_float fTimeDelta)
{
	 // 주먹 와리가리
}

CBoss* CBoss::Create(LPDIRECT3DDEVICE9 m_pGraphic_Device)
{
	CBoss* p_boss = new CBoss(m_pGraphic_Device);
	if(FAILED(p_boss->NativeConstruct_Prototype()))
	{
		Safe_Release(p_boss);
		return nullptr;
	}
	return p_boss;
}

CGameObject* CBoss::Clone(void* pArg)
{
	CBoss* p_boss = new CBoss(*this);
	if (FAILED(p_boss->NativeConstruct(pArg)))
	{
		Safe_Release(p_boss);
		return nullptr;
	}
	return p_boss;
}

void CBoss::Free()
{
	__super::Free();
	Safe_Release(m_pOnlyRotation);
	Safe_Release(m_pTransform);
	Safe_Release(m_pTexture);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pCollider);
	Safe_Release(m_pRenderer);

}


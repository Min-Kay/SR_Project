#include "stdafx.h"
#include "Arm.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "VIBuffer_Cube.h"
#include "Renderer.h"
#include "Texture.h"
#include "GameInstance.h"

CArm::CArm(LPDIRECT3DDEVICE9 m_pGraphic_Device)
	:CEnemy(m_pGraphic_Device)
{
}

CArm::CArm(const CArm& rhs)
	:CEnemy(rhs)
	,m_pTransform(rhs.m_pTransform)
	, m_pOnlyRotation(rhs.m_pOnlyRotation)
	,m_Collider(rhs.m_Collider)
	,m_pTexture(rhs.m_pTexture)
	,m_pRenderer(rhs.m_pRenderer)
	,m_pBuffer(rhs.m_pBuffer)
{
	Safe_AddRef(m_pOnlyRotation);
	Safe_AddRef(m_pTransform);
	Safe_AddRef(m_Collider);
	Safe_AddRef(m_pTexture);
	Safe_AddRef(m_pRenderer);
	Safe_AddRef(m_pBuffer);
}

HRESULT CArm::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CArm::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	return S_OK;
}

_int CArm::Tick(_float fTimeDelta)
{
	if (Get_Dead() || fTimeDelta <= 0.f)
		return 0;

	Mode(fTimeDelta);
	m_Collider->Set_Collider();
	Synchronize_Transform();

	if (0 > __super::Tick(fTimeDelta))
		return -1;
	return 0;
}

_int CArm::LateTick(_float fTimeDelta)
{
	if (Get_Dead())
		return 0;

	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return 0;
}

HRESULT CArm::Render()
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

HRESULT CArm::SetUp_Component()
{
	CTransform::TRANSFORMDESC desc;
	desc.fRotationPerSec = 90.f;
	desc.fSpeedPerSec = 5.f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, TEXT("OnlyRotation"), (CComponent**)&m_pOnlyRotation, &desc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransform, &desc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLLIDER, COM_COLLIDER, (CComponent**)&m_Collider)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_CUBE, COM_BUFFER, (CComponent**)&m_pBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CubeMonster"), COM_TEXTURE, (CComponent**)&m_pTexture)))
		return E_FAIL;

	Set_Type(OBJ_ENEMY);

	
	m_pTransform->Scaled(_float3(7.f, 7.f, 7.f));
	m_pOnlyRotation->Scaled(_float3(7.f, 7.f, 7.f));

	m_Collider->Set_ParentInfo(this);
	m_Collider->Set_CollStyle(CCollider::COLLSTYLE_ENTER);
	m_Collider->Set_State(CBoxCollider::COLL_SIZE, _float3(7.f, 7.f, 7.f));
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_OBJ, m_Collider);
	RELEASE_INSTANCE(CGameInstance);


	m_Damage = 10;

	return S_OK; 
}

void CArm::Set_Position(_float3 _pos)
{
	m_pTransform->Set_State(CTransform::STATE_POSITION, _pos);
}

void CArm::Move_Dir(_float3 dir, _float range)
{
	m_pTransform->Set_State(CTransform::STATE_POSITION, m_pTransform->Get_State(CTransform::STATE_POSITION) + dir * range);
}

void CArm::Set_Parent(CBoss* parent)
{
	m_Parent = parent;
}

void CArm::Set_Player(CPlayer* _player)
{
	m_Player = _player; 
}

void CArm::Synchronize_Transform()
{
	m_pOnlyRotation->Set_State(CTransform::STATE_POSITION,m_pTransform->Get_State(CTransform::STATE_POSITION));
}

void CArm::Add_Position(_float3 _add)
{
	m_pTransform->Set_State(CTransform::STATE_POSITION, m_pTransform->Get_State(CTransform::STATE_POSITION) + _add);
}

void CArm::Mode(_float fTimeDelta)
{
	switch (m_State)
	{
	case ARM_IDLE:
		break;
	case ARM_ATTACK:
		m_Timer += fTimeDelta;
		if(m_Timer > m_AttackTick)
		{
		// 충돌처리 및 데미지 연산 
			m_Timer = 0.f; 
		}
		break;
	}
}

CArm* CArm::Create(LPDIRECT3DDEVICE9 m_pGraphic_Device)
{
	CArm* p_arm = new CArm(m_pGraphic_Device);
	if(FAILED(p_arm->NativeConstruct_Prototype()))
	{
		Safe_Release(p_arm);
		return nullptr; 
	}
	return p_arm;
}

CGameObject* CArm::Clone(void* pArg)
{
	CArm* p_arm = new CArm(*this);
	if (FAILED(p_arm->NativeConstruct(pArg)))
	{
		Safe_Release(p_arm);
		return nullptr;
	}
	return p_arm;
}

void CArm::Free()
{
	__super::Free();
	Safe_Release(m_pOnlyRotation);
	Safe_Release(m_Collider);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pTransform);
	Safe_Release(m_pTexture);

}

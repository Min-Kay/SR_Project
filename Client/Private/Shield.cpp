#include "stdafx.h"
#include "Shield.h"

#include "Transform.h"
#include "Renderer.h"
#include "Texture.h"
#include "VIBuffer_Cube.h"
#include "BoxCollider.h"
#include "GameInstance.h"
#include "Impact.h"


CShield::CShield(LPDIRECT3DDEVICE9 m_pGraphic_Device)
	:CEnemy(m_pGraphic_Device)
{
}

CShield::CShield(const CShield& rhs)
	:CEnemy(rhs)
	,m_pTransform(rhs.m_pTransform)
	,m_pCollider(rhs.m_pCollider)
	,m_pTexture(rhs.m_pTexture)
	,m_pBuffer(rhs.m_pBuffer)
	,m_pRenderer(rhs.m_pRenderer)
{
	Safe_AddRef(m_pBuffer);
	Safe_AddRef(m_pCollider);
	Safe_AddRef(m_pTransform);
	Safe_AddRef(m_pTexture);
	Safe_AddRef(m_pRenderer);

}

HRESULT CShield::NativeConstruct_Prototype()
{
	if(FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CShield::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	return S_OK;
}

_int CShield::Tick(_float fTimeDelta)
{
	if (!m_Valid)
		return 0;

	if (fTimeDelta <= 0)
		return 0;

	if (Get_Dead())
		return 0;

	Synchronize_Transform();
	m_pCollider->Set_Collider();


	if (0 > __super ::Tick(fTimeDelta))
		return -1;
	return 0;
}

_int CShield::LateTick(_float fTimeDelta)
{
	if (!m_Valid)
		return 0;

	if (0 > __super ::LateTick(fTimeDelta))
		return -1;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return 0;
}

HRESULT CShield::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransform->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(m_pTexture->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pBuffer->Render();

	return S_OK;
}

HRESULT CShield::SetUp_Component()
{
	CTransform::TRANSFORMDESC desc;
	desc.fRotationPerSec = 90.f;
	desc.fSpeedPerSec = 5.f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransform, &desc)))
		return E_FAIL; 

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLLIDER, COM_COLLIDER, (CComponent**)&m_pCollider)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRenderer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_CUBE, COM_BUFFER, (CComponent**)&m_pBuffer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Shield"), COM_TEXTURE, (CComponent**)&m_pTexture)))
		return E_FAIL;

	Set_Type(OBJ_ENEMY);

	m_pCollider->Set_CollStyle(CCollider::COLLSTYLE_ONLYRAY);
	m_pCollider->Set_ParentInfo(this);
	m_pCollider->Set_State(CBoxCollider::COLL_SIZE, _float3(1.f, 1.f, 1.f));

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_STATIC, m_pCollider);
	RELEASE_INSTANCE(CGameInstance);

	m_Hp = 100;

	return S_OK;
}

void CShield::Set_ParentTransform(CTransform* _tr)
{
	m_ParentTr = _tr;
}

void CShield::Add_HP(_int _add)
{
	
}

void CShield::Spawn_Shield()
{
	
}

void CShield::Break_Shield()
{
}

void CShield::Set_Valid(_bool _bool)
{
	m_Valid = _bool;
}

const _bool CShield::Get_Valid() const
{
	return m_Valid;
}

void CShield::Synchronize_Transform()
{
	if (!m_ParentTr)
		return;

	_float3 vRight, vUp, vLook, vPos, vScale;

	vRight = m_ParentTr->Get_State(CTransform::STATE_RIGHT);
	vUp = m_ParentTr->Get_State(CTransform::STATE_UP);
	vLook = m_ParentTr->Get_State(CTransform::STATE_LOOK);
	vPos = m_ParentTr->Get_State(CTransform::STATE_POSITION);
	vScale = m_ParentTr->Get_Scale() * 1.5f;

	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);


	m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
	m_pTransform->Set_State(CTransform::STATE_UP, vUp * vScale.y);
	m_pTransform->Set_State(CTransform::STATE_LOOK, vLook * vScale.z);
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPos);

	m_pCollider->Set_State(CBoxCollider::COLL_SIZE, vScale);

}

void CShield::Sizing_Particles()
{
	CImpact::IMPACT Impact1;
	ZeroMemory(&Impact1, sizeof(Impact1));
	Impact1.Pos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	Impact1.Size = _float3(0.1f, 0.1f, 0.1f);
	Impact1.randomPos = 7;
	Impact1.Speed = 15;
	Impact1.deleteCount = 2;//rand() % 5 + 2;
	Impact1.Color = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	for (int i = 0; i < 10; ++i)
	{
		if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("Impact"), TEXT("Prototype_GameObject_Impact"), &Impact1)))
		{
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
	}
	RELEASE_INSTANCE(CGameInstance);
}

CShield* CShield::Create(LPDIRECT3DDEVICE9 m_pGraphic_Device)
{
	CShield* p_shield = new CShield(m_pGraphic_Device);

	if(FAILED(p_shield->NativeConstruct_Prototype()))
	{
		Safe_Release(p_shield);
		return nullptr;
	}
	return p_shield;
}

CGameObject* CShield::Clone(void* pArg)
{

	CShield* p_shield = new CShield(*this);

	if (FAILED(p_shield->NativeConstruct(pArg)))
	{
		Safe_Release(p_shield);
		return nullptr;
	}
	return p_shield;
}

void CShield::Free()
{
	__super::Free();

	Safe_Release(m_pBuffer);
	Safe_Release(m_pCollider);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pTransform);
}

#include "stdafx.h"
#include "Door_left.h"
#include "VIBuffer_Cube.h"
#include "GameInstance.h"

CDoor_left::CDoor_left(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)	
{

}

CDoor_left::CDoor_left(const CDoor_left & rhs)
	: CGameObject(rhs)
{
}

HRESULT CDoor_left::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDoor_left::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	/* ���� ��ü���� �߰��Ǿ���� ������Ʈ���� ����(or ����)�Ͽ� ��������� �����Ѵ�.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	_float3 vPos = { 1.f, 0.f, 4.f };

	m_pTransformCom->Scaled(_float3(0.5f, 1.f, 0.2f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(120));

	if (m_pBoxColliderCom == nullptr)
	{
		MSGBOX("Empty BoxCollider component in CDoorLeft");
		return E_FAIL;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->Add_Collider(CCollision_Manager::COLLOBJTYPE_STATIC, m_pBoxColliderCom);
	RELEASE_INSTANCE(CGameInstance);



	return S_OK;
}

_int CDoor_left::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;
	/*if (nullptr == m_pBoxColliderCom)
	{
		return -1;
	}*/

	m_pBoxColliderCom->Set_Coilider();

	return _int();
}

_int CDoor_left::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CDoor_left::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pBoxColliderCom->Draw_Box();

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;


	m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT CDoor_left::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 2.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), COM_TRANSFORM, (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), COM_RENDERER, (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), COM_BUFFER, (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STAGEONE, TEXT("Prototype_Component_Texture_Door_Left"), COM_TEXTURE, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), COM_COLLIDER, (CComponent**)&m_pBoxColliderCom)))
		return E_FAIL;
	m_pBoxColliderCom->Set_ParentInfo(this);
	m_pBoxColliderCom->Set_State(CBoxCollider::COLLIDERINFO::COLL_SIZE, _float3(1.f, 1.f, 1.f));
	return S_OK;
}

HRESULT CDoor_left::Open(_bool IsOpen, _float fTimeDelta)
{
	if (IsOpen)
	{
		++m_Count;

		if (m_Count > 10)
		{
		
			return S_OK;
		}
		
	/*	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 vDir = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
	
		D3DXVec3Normalize(&vDir, &vDir);
		vPos += *D3DXVec3Normalize(&vDir, &vDir) * m_pTransformCom->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);*/
		
		m_pTransformCom->Go_Left(fTimeDelta);

	}

	return S_OK;
}



CDoor_left * CDoor_left::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDoor_left*	pInstance = new CDoor_left(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CDoor_left");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDoor_left::Clone(void* pArg )
{
	/* ���οü�� �����Ͽ� �����Ѵ�. */
	CDoor_left*	pInstance = new CDoor_left(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Clone CDoor_left");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDoor_left::Free()
{
	__super::Free();

	Safe_Release(m_pBoxColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
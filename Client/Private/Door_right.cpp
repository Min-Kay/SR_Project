#include "stdafx.h"
#include "Door_right.h"
#include "VIBuffer_Cube.h"
#include "GameInstance.h"

CDoor_right::CDoor_right(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)	
{

}

CDoor_right::CDoor_right(const CDoor_right & rhs)
	: CGameObject(rhs)
{
}

HRESULT CDoor_right::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDoor_right::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	
	//if (m_pBoxColliderCom == nullptr)
	//{
	//	MSGBOX("Empty BoxCollider component in CDoorLeft");
	//	return E_FAIL;
	//}


	return S_OK;
}

_int CDoor_right::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;
	/*if (nullptr == m_pBoxColliderCom)
	{
		return -1;
	}*/

	/*CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->Add_Collider(m_pBoxColliderCom);
	RELEASE_INSTANCE(CGameInstance);

	m_pBoxColliderCom->Set_Coilider();*/
	
	
	return _int();
}

_int CDoor_right::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CDoor_right::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	//m_pBoxColliderCom->Draw_Box();

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;


	m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT CDoor_right::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.0f;
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Door_Right"), COM_TEXTURE, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Collider */
	/*if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), COM_COLLIDER, (CComponent**)&m_pBoxColliderCom)))
		return E_FAIL;
	m_pBoxColliderCom->Set_Parent(this);
	m_pBoxColliderCom->Get_Parentcom();
	m_pBoxColliderCom->Set_State(CBoxCollider::COLLIDERINFO::COLL_SIZE, _float3(1.f, 1.f, 1.f));*/
	return S_OK;
}

HRESULT CDoor_right::Open(_bool IsOpen, _float fTimeDelta)
{
	if (IsOpen)
	{
		++m_Count;

		if (m_Count > 23)
		{

			return S_OK;
		}
		m_pTransformCom->Go_Right(fTimeDelta);

	}

	return S_OK;
}



CDoor_right * CDoor_right::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDoor_right*	pInstance = new CDoor_right(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CDoor_right");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDoor_right::Clone(void* pArg )
{
	/* 새로운객체를 복제하여 생성한다. */
	CDoor_right*	pInstance = new CDoor_right(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Clone CDoor_right");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDoor_right::Free()
{
	__super::Free();

	//Safe_Release(m_pBoxColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

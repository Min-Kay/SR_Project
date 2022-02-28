#include "stdafx.h"
#include "Tile_Cube.h"
#include "VIBuffer_Cube.h"
#include "GameInstance.h"

CTile_Cube::CTile_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)	
{

}

CTile_Cube::CTile_Cube(const CTile_Cube & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTile_Cube::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTile_Cube::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	/* ���� ��ü���� �߰��Ǿ���� ������Ʈ���� ����(or ����)�Ͽ� ��������� �����Ѵ�.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	Set_Type(OBJ_STATIC);
	/*if (m_pBoxColliderCom == nullptr)
	{
		MSGBOX("Empty BoxCollider component in CDoorLeft");
		return E_FAIL;
	}*/


	return S_OK;
}

_int CTile_Cube::Tick(_float fTimeDelta)
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

_int CTile_Cube::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CTile_Cube::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	//m_pBoxColliderCom->Draw_Box();

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(m_iTextureIndex)))
		return E_FAIL;


	m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT CTile_Cube::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Block"), COM_TEXTURE, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Collider */
	/*if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider"), COM_COLLIDER, (CComponent**)&m_pBoxColliderCom)))
		return E_FAIL;
	m_pBoxColliderCom->Set_Parent(this);
	m_pBoxColliderCom->Get_Parentcom();
	m_pBoxColliderCom->Set_State(CBoxCollider::COLLIDERINFO::COLL_SIZE, _float3(1.f, 1.f, 1.f));*/
	return S_OK;
}



CTile_Cube * CTile_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTile_Cube*	pInstance = new CTile_Cube(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CTile_Cube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTile_Cube::Clone(void* pArg )
{
	/* ���οü�� �����Ͽ� �����Ѵ�. */
	CTile_Cube*	pInstance = new CTile_Cube(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Clone CTile_Cube");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTile_Cube::Free()
{
	__super::Free();

	//Safe_Release(m_pBoxColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
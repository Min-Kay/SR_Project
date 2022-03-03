#include "stdafx.h"
#include "ChangeLevel.h"
#include "VIBuffer_Cube.h"
#include "GameInstance.h"
#include "Player.h"
#include "Level_Loading.h"

CChangeLevel::CChangeLevel(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)	
{

}

CChangeLevel::CChangeLevel(const CChangeLevel & rhs)
	: CGameObject(rhs)
{
}

HRESULT CChangeLevel::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CChangeLevel::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	Set_Type(OBJ_STATIC);


	return S_OK;
}

_int CChangeLevel::Tick(_float fTimeDelta)
{	
	
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	m_iCount += fTimeDelta;


		if (m_pBoxColliderCom)
			m_pBoxColliderCom->Set_Collider();

		CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
		

	
		
		list<CGameObject*> test = p_instance->Get_Collision_List(m_pBoxColliderCom);

		for (auto & iter : test)
		{
				
			if (OBJ_PLAYER == iter->Get_Type())
			{
				m_bLevelOut = true;
			}

		
		}

		if (p_instance->Get_Key_Down(DIK_P))
		{
			m_bLevelOut = true;
		}

	RELEASE_INSTANCE(CGameInstance);
	
	return _int();
}

_int CChangeLevel::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CChangeLevel::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;


	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;



	//m_pBoxColliderCom->Draw_Box();

	SetUp_RenderState();

	m_pVIBufferCom->Render();

	Release_RenderState();
	return S_OK;
}

HRESULT CChangeLevel::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 0.0f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_CUBE, COM_BUFFER, (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ChangeLevel"), COM_TEXTURE, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Box */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLLIDER, COM_COLLIDER, (CComponent**)&m_pBoxColliderCom)))
		return E_FAIL;

	m_pBoxColliderCom->Set_ParentInfo(this);
	m_pBoxColliderCom->Set_State(CBoxCollider::COLLIDERINFO::COLL_SIZE, _float3(1.f, 1.f, 1.f));
	m_pBoxColliderCom->Set_CollStyle(CCollider::COLLSTYLE_TRIGGER);
	m_pBoxColliderCom->Set_Collider();
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_STATIC, m_pBoxColliderCom);
	RELEASE_INSTANCE(CGameInstance);
	
	return S_OK;
}

HRESULT CChangeLevel::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	return S_OK;
}

HRESULT CChangeLevel::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}


CChangeLevel * CChangeLevel::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CChangeLevel*	pInstance = new CChangeLevel(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CChangeLevel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CChangeLevel::Clone(void* pArg )
{
	/* 새로운객체를 복제하여 생성한다. */
	CChangeLevel*	pInstance = new CChangeLevel(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Clone CChangeLevel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CChangeLevel::Free()
{
	__super::Free();
	Safe_Release(m_pBoxColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

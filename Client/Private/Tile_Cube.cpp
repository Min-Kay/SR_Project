#include "stdafx.h"
#include "Tile_Cube.h"
#include "VIBuffer_Cube.h"
#include "GameInstance.h"
#include "Door_left.h"
#include "Door_right.h"
#include "Player.h"
#include "Shader.h"

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

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	Set_Type(OBJ_STATIC);
	m_pBoxColliderCom->Set_Collider();
	
	return S_OK;
}

_int CTile_Cube::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	return _int();
}

_int CTile_Cube::LateTick(_float fTimeDelta)
{
	if (!m_Vaild)
		return 0;

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

	m_pTransformCom->Bind_OnShader(m_pShader);

	m_pShader->SetUp_ValueOnShader("g_ColorStack", &g_ControlShader, sizeof(_float));
	m_pShader->SetUp_ValueOnShader("g_Color", m_Color, sizeof(_float4));
	m_pTextureCom->Bind_OnShader(m_pShader, "g_Texture", m_iTextureIndex);

	m_pShader->Begin_Shader(SHADER_SETCOLOR_CUBE);

	m_pVIBufferCom->Render();

	m_pShader->End_Shader();
	m_pShader->SetUp_ValueOnShader("g_Color", &_float4(0.f, 0.f, 0.f, 0.f), sizeof(_float4));

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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC,PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_CUBE, COM_BUFFER, (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Block"), COM_TEXTURE, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Box */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_SHADER_CUBE, COM_SHADER, (CComponent**)&m_pShader)))
		return E_FAIL;

	/* For.Com_Box */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLLIDER, COM_COLLIDER, (CComponent**)&m_pBoxColliderCom)))
		return E_FAIL;
	m_pBoxColliderCom->Set_ParentInfo(this);

	m_pBoxColliderCom->Set_CollStyle(CCollider::COLLSTYLE_TRIGGER);
	m_pBoxColliderCom->Set_State(CBoxCollider::COLLIDERINFO::COLL_SIZE, _float3(1.f, 1.f, 1.f));
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_STATIC, m_pBoxColliderCom);
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

_bool CTile_Cube::Open_Event(_uint iLevelIndex, const _tchar* pLeftDoorLayerTag, const _tchar* pRightDoorLayerTag)
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (!m_PlayerCollider)
		m_PlayerCollider = static_cast<CBoxCollider*>(pGameInstance->Get_GameObject_End(g_CurrLevel, TEXT("Layer_Player"))->Get_Component(COM_COLLIDER));


	if(pGameInstance->Get_Collide(m_pBoxColliderCom, m_PlayerCollider))
	{
		static_cast<CDoor_left*>(pGameInstance->Get_GameObject(iLevelIndex, pLeftDoorLayerTag))->Set_Open(true);
		static_cast<CDoor_right*>(pGameInstance->Get_GameObject(iLevelIndex, pRightDoorLayerTag))->Set_Open(true);
		pGameInstance->Play_Sound(TEXT("Door_Open_1.mp3"), CSoundMgr::ADDITIONAL_EFFECT1, 1.f);

		RELEASE_INSTANCE(CGameInstance);
		return true;
	}

	RELEASE_INSTANCE(CGameInstance);

	return false;
}

_bool CTile_Cube::Open_Block_Event(_uint iLevelIndex, const _tchar* pLeftDoorLayerTag, const _tchar* pRightDoorLayerTag)
{
	return false;
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (!m_PlayerCollider)
		m_PlayerCollider = static_cast<CBoxCollider*>(pGameInstance->Get_GameObject_End(g_CurrLevel, TEXT("Layer_Player"))->Get_Component(COM_COLLIDER));

	list<CGameObject*> collList = pGameInstance->Get_Collision_Object_List(m_pBoxColliderCom);

	for(auto& i : collList)
	{
		if(i->Get_Type() == OBJ_INTERACTION)
		{
			pGameInstance->Play_Sound(TEXT("Press_Cube.mp3"), CSoundMgr::ADDITIONAL_EFFECT2, 1.f);

			static_cast<CDoor_left*>(pGameInstance->Get_GameObject(iLevelIndex, pLeftDoorLayerTag))->Set_Open(true);
			static_cast<CDoor_right*>(pGameInstance->Get_GameObject(iLevelIndex, pRightDoorLayerTag))->Set_Open(true);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) - _float3(0.0f, 0.6f, 0.0f));

			RELEASE_INSTANCE(CGameInstance);
			return true;

		}
	}

	RELEASE_INSTANCE(CGameInstance);

	return false;
}


_bool CTile_Cube::Close_Event(_uint iLevelIndex, const _tchar* pLeftDoorLayerTag, const _tchar* pRightDoorLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if(!m_PlayerCollider)
		m_PlayerCollider = static_cast<CBoxCollider*>(pGameInstance->Get_GameObject_End(g_CurrLevel, TEXT("Layer_Player"))->Get_Component(COM_COLLIDER));


	if (pGameInstance->Get_Collide(m_pBoxColliderCom, m_PlayerCollider))
	{
		static_cast<CDoor_left*>(pGameInstance->Get_GameObject(iLevelIndex, pLeftDoorLayerTag))->Set_Close(true);
		static_cast<CDoor_right*>(pGameInstance->Get_GameObject(iLevelIndex, pRightDoorLayerTag))->Set_Close(true);
		pGameInstance->Play_Sound(TEXT("Door_Close.mp3"), CSoundMgr::ADDITIONAL_EFFECT2, 1.f);

		RELEASE_INSTANCE(CGameInstance);
		return true;
	}

	RELEASE_INSTANCE(CGameInstance);

	return false;
}


_bool CTile_Cube::Save_Point()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (!m_PlayerCollider)
		m_PlayerCollider = static_cast<CBoxCollider*>(pGameInstance->Get_GameObject_End(g_CurrLevel, TEXT("Layer_Player"))->Get_Component(COM_COLLIDER));

	if (pGameInstance->Get_Collide(m_pBoxColliderCom, m_PlayerCollider))
	{
		RELEASE_INSTANCE(CGameInstance);
		return true;
	}

	RELEASE_INSTANCE(CGameInstance);

	return false;
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
	/* 새로운객체를 복제하여 생성한다. */
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

	Safe_Release(m_pBoxColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShader);

}

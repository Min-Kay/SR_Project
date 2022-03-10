#include "stdafx.h"
#include "Ball.h"
#include "VIBuffer_Cube.h"
#include "GameInstance.h"
#include "Player.h"
#include "Shader.h"

CBall::CBall(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CBall::CBall(const CBall & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBall::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBall::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	m_bBallLive = true;
	Set_Type(OBJ_INTERACTION);
	return S_OK;
}

_int CBall::Tick(_float fTimeDelta)
{
	if (FAILED(Get_Dead()))
		return 0;

	if (0 > __super::Tick(fTimeDelta))
		return -1;

	if (m_pBoxColliderCom)
		m_pBoxColliderCom->Set_Collider();
	Move(fTimeDelta);

	return _int();
}

_int CBall::LateTick(_float fTimeDelta)
{
	if (FAILED(Get_Dead()))
		return 0;

	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CBall::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;


	m_pTransformCom->Bind_OnShader(m_pShader);

	m_pShader->SetUp_ValueOnShader("g_ColorStack", &g_ControlShader, sizeof(_float));

	m_pTextureCom->Bind_OnShader(m_pShader, "g_Texture", (_uint)m_fFrame);

	m_pShader->Begin_Shader(SHADER_SETCOLOR_CUBE);
	m_pVIBufferCom->Render();
	m_pShader->End_Shader();

	if(m_bBallLive == false)
		Set_Dead(true);
	return S_OK;
}

HRESULT CBall::Move(_float fTimeDelta)
{
	m_pTransformCom->Go_Straight(fTimeDelta);


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	list<CGameObject*> collList = pGameInstance->Get_Collision_Object_List(m_pBoxColliderCom);

	for (auto& i : collList)
	{
		if (i->Get_Type() == OBJ_STATIC)
		{
			//m_bBallLive = false;
		}
		else if (i->Get_Type() == OBJ_PLAYER)
		{
			//m_bBallLive = false;
		}
		
	}
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CBall::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 2.0f;
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Water"), COM_TEXTURE, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Box */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLLIDER, COM_COLLIDER, (CComponent**)&m_pBoxColliderCom)))
		return E_FAIL;

	/* For.Com_Box */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_SHADER_CUBE, COM_SHADER, (CComponent**)&m_pShader)))
		return E_FAIL;

	m_pBoxColliderCom->Set_ParentInfo(this);
	m_pBoxColliderCom->Set_State(CBoxCollider::COLLIDERINFO::COLL_SIZE, _float3(1.f, 1.f, 1.f));
	m_pBoxColliderCom->Set_CollStyle(CCollider::COLLSTYLE_ENTER);

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_OBJ, m_pBoxColliderCom);
	//m_Player = static_cast<CPlayer*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("Layer_Player")));
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CBall::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	return S_OK;
}

HRESULT CBall::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}


CBall * CBall::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBall*	pInstance = new CBall(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CBall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBall::Clone(void* pArg)
{
	/* 새로운객체를 복제하여 생성한다. */
	CBall*	pInstance = new CBall(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Clone CBall");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBall::Free()
{
	__super::Free();
	Safe_Release(m_pBoxColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShader);
}

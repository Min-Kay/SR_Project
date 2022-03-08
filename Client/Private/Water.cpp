#include "stdafx.h"
#include "Water.h"
#include "VIBuffer_Cube.h"
#include "GameInstance.h"
#include "Player.h"
#include "Shader.h"
CWater::CWater(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)	
{

}

CWater::CWater(const CWater & rhs)
	: CGameObject(rhs)
{
}

HRESULT CWater::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWater::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	Set_Type(OBJ_STATIC);
	return S_OK;
}

_int CWater::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	if (!m_Player)
	{
		CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
		m_Player = static_cast<CPlayer*>(p_instance->Get_GameObject_End(g_CurrLevel,TEXT("Layer_Player")));
		RELEASE_INSTANCE(CGameInstance); 
	}

	m_fFrame += 4.0f * fTimeDelta;

	if (m_fFrame >= 4.0f)
		m_fFrame = 0.f;

	if (m_pBoxColliderCom)
		m_pBoxColliderCom->Set_Collider();

	m_iCount += fTimeDelta;
	if(m_Player)
	{
		CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
		if(p_instance->Get_Collide(m_pBoxColliderCom,static_cast<CBoxCollider*>(m_Player->Get_Component(COM_COLLIDER))))
		{
			if (m_iCount > 1.0)
			{
				m_Player->Add_Hp(-50);
				m_iCount = 0;

			}
		}
		RELEASE_INSTANCE(CGameInstance);
	}

	
	return _int();
}

_int CWater::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CWater::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	//if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	//	return E_FAIL;


	//if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_uint)m_fFrame)))
	//	return E_FAIL;



	////m_pBoxColliderCom->Draw_Box();

	//SetUp_RenderState();

	//m_pVIBufferCom->Render();

	//Release_RenderState();


	m_pTransformCom->Bind_OnShader(m_pShader);

	m_pShader->SetUp_ValueOnShader("g_ColorStack", &g_ControlShader, sizeof(_float));

	m_pTextureCom->Bind_OnShader(m_pShader, "g_Texture", (_uint)m_fFrame);

	m_pShader->Begin_Shader(SHADER_SETCOLOR_CUBE);
	m_pVIBufferCom->Render();
	m_pShader->End_Shader();
	return S_OK;
}

HRESULT CWater::SetUp_Components()
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
	m_pBoxColliderCom->Set_CollStyle(CCollider::COLLSTYLE_TRIGGER);
	m_pBoxColliderCom->Set_Collider();
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_STATIC, m_pBoxColliderCom);
	m_Player = static_cast<CPlayer*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("Layer_Player")));
	RELEASE_INSTANCE(CGameInstance);
	
	return S_OK;
}

HRESULT CWater::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	return S_OK;
}

HRESULT CWater::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}


CWater * CWater::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWater*	pInstance = new CWater(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CWater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CWater::Clone(void* pArg )
{
	/* 새로운객체를 복제하여 생성한다. */
	CWater*	pInstance = new CWater(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Clone CWater");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CWater::Free()
{
	__super::Free();
	Safe_Release(m_pBoxColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShader);
}

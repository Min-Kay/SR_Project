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

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;


	Set_Type(OBJ_STATIC);

	return S_OK;
}

_int CDoor_left::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	if (m_pBoxColliderCom)
		m_pBoxColliderCom->Set_Collider();

	Open(m_Open);
	Close(m_Close);
	
	return _int();
}

_int CDoor_left::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CDoor_left::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnShader(m_pShader)))
		return E_FAIL;

	m_pShader->SetUp_ValueOnShader("g_ColorStack", &g_ControlShader, sizeof(_float));
	m_pTextureCom->Bind_OnShader(m_pShader, "g_Texture", 0);
	m_pShader->Begin_Shader(SHADER_SETCOLOR_CUBE);
	m_pVIBufferCom->Render();
	m_pShader->End_Shader();

	//m_pBoxColliderCom->Draw_Box();



	/*if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(m_iTextureIndex)))
		return E_FAIL;*/





	return S_OK;
}

HRESULT CDoor_left::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Door_Left"), COM_TEXTURE, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Box */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLLIDER, COM_COLLIDER, (CComponent**)&m_pBoxColliderCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_SHADER_CUBE, COM_SHADER, (CComponent**)&m_pShader)))
		return E_FAIL;

	m_pBoxColliderCom->Set_ParentInfo(this);
	m_pBoxColliderCom->Set_State(CBoxCollider::COLLIDERINFO::COLL_SIZE, _float3(1.f, 1.f, 1.f));

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	p_instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_STATIC, m_pBoxColliderCom);
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CDoor_left::Open(_bool IsOpen)
{

	if (IsOpen)
	{

		m_Count += 0.1f;
		if (m_Count > m_MaxCount)
		{
			m_Count = m_MaxCount;
			return S_OK;
		}
		m_pTransformCom->Go_Left(0.02f);

	}
	

	return S_OK;
}

HRESULT CDoor_left::Close(_bool IsClose)
{
	if (IsClose)
	{
		m_Count -= 0.1f;

		if (m_Count < 0)
		{
			m_Count = 0;
			return S_OK;
		}
		m_pTransformCom->Go_Right(0.02f);

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
	/* 새로운객체를 복제하여 생성한다. */
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
	Safe_Release(m_pShader);
}

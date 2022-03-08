#include "stdafx.h"
#include "..\Public\AttackRange.h"
#include "GameInstance.h"
#include "Shader.h"

CAttackRange::CAttackRange(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)	
{

}

CAttackRange::CAttackRange(const CAttackRange & rhs)
	: CGameObject(rhs)
{
}

HRESULT CAttackRange::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CAttackRange::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	Set_Type(OBJ_STATIC);


	return S_OK;
}

_int CAttackRange::Tick(_float fTimeDelta)
{
	if (fTimeDelta <= 0)
		return 0;

	if (!m_valid)
		return 0;

	m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta  );

	if (0 > __super::Tick(fTimeDelta))
		return -1;

	return _int();
}

_int CAttackRange::LateTick(_float fTimeDelta)
{
	if (!m_valid)
		return 0;

	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CAttackRange::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pTransformCom->Bind_OnShader(m_pShader);
	m_pShader->SetUp_ValueOnShader("g_ColorStack", &g_ControlShader, sizeof(_float));
	m_pTextureCom->Bind_OnShader(m_pShader, "g_Texture", 0);

	m_pShader->Begin_Shader(SHADER_SETCOLOR_BLEND);
	m_pVIBufferCom->Render();
	m_pShader->End_Shader();


	return S_OK;
}

HRESULT CAttackRange::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RECT, COM_BUFFER, (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_AttackRange"), COM_TEXTURE, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_SHADER_RECT, COM_SHADER, (CComponent**)&m_pShader)))
		return E_FAIL;

	m_pTransformCom->Scaled(_float3(7.f, 7.f, 7.f));
	m_pTransformCom->Rotation(_float3(1.f,0.f,0.f),D3DXToRadian(90));

	return S_OK;
}

HRESULT CAttackRange::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	return S_OK;
}

HRESULT CAttackRange::Release_RenderState()
{

	return S_OK;
}


CAttackRange * CAttackRange::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAttackRange*	pInstance = new CAttackRange(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CAttackRange");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CAttackRange::Clone(void* pArg )
{
	/* 새로운객체를 복제하여 생성한다. */
	CAttackRange*	pInstance = new CAttackRange(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CAttackRange");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAttackRange::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShader);
}

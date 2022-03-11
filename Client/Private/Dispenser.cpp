#include "stdafx.h"
#include "..\Public\Dispenser.h"
#include "VIBuffer_Cube.h"
#include "GameInstance.h"
#include "Player.h"
#include "Shader.h"
CDispenser::CDispenser(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)	
{

}

CDispenser::CDispenser(const CDispenser & rhs)
	: CGameObject(rhs)
{
}

HRESULT CDispenser::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDispenser::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	/* ���� ��ü���� �߰��Ǿ���� ������Ʈ���� ����(or ����)�Ͽ� ��������� �����Ѵ�.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	Set_Type(OBJ_STATIC);
	return S_OK;
}

_int CDispenser::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;


	return _int();
}

_int CDispenser::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

HRESULT CDispenser::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;



	m_pTransformCom->Bind_OnShader(m_pShader);

	m_pShader->SetUp_ValueOnShader("g_ColorStack", &g_ControlShader, sizeof(_float));
	m_pShader->SetUp_ValueOnShader("g_Color", _float4(1.0f,0.3f,0.2f,0.f), sizeof(_float4));

	m_pTextureCom->Bind_OnShader(m_pShader, "g_Texture", (_uint)m_fFrame);

	m_pShader->Begin_Shader(SHADER_SETCOLOR_CUBE);
	m_pVIBufferCom->Render();
	m_pShader->End_Shader();
	m_pShader->SetUp_ValueOnShader("g_Color", _float4(0.f, 0.f, 0.f, 0.f), sizeof(_float4));

	return S_OK;
}

HRESULT CDispenser::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dispenser"), COM_TEXTURE, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Box */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_SHADER_CUBE, COM_SHADER, (CComponent**)&m_pShader)))
		return E_FAIL;

	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
	m_Player = static_cast<CPlayer*>(p_instance->Get_GameObject(g_CurrLevel, TEXT("Layer_Player")));
	RELEASE_INSTANCE(CGameInstance);
	
	return S_OK;
}


CDispenser * CDispenser::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDispenser*	pInstance = new CDispenser(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CDispenser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDispenser::Clone(void* pArg )
{
	/* ���οü�� �����Ͽ� �����Ѵ�. */
	CDispenser*	pInstance = new CDispenser(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Clone CDispenser");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDispenser::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShader);
}

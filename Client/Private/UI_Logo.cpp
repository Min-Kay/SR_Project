#include "stdafx.h"
#include "UI_Logo.h"
#include "Transform.h"
#include "Renderer.h"
#include "Texture.h"
#include "VIBuffer_Rect.h"

CUI_Logo::CUI_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CUI_Logo::CUI_Logo(const CUI_Logo& rhs)
	:CUI(rhs)
{
}

HRESULT CUI_Logo::NativeConstruct_Prototype()
{
	return __super::NativeConstruct_Prototype();
}

HRESULT CUI_Logo::NativeConstruct(void* pArg)
{
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	Set_UI(g_iWinCX * 0.5f, g_iWinCY * 0.5f,g_iWinCX, g_iWinCY);

	return S_OK;
}

_int CUI_Logo::Tick(_float fTimeDelta)
{
	return _int();
}

_int CUI_Logo::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return _int();
}

HRESULT CUI_Logo::Render()
{
	Bind_UI();

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Logo::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Logo"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CUI_Logo* CUI_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{	
	CUI_Logo* pInstance = new CUI_Logo(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CUI_Logo::Clone(void* pArg)
{
	CUI_Logo* pInstance = new CUI_Logo(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CUI_Logo::Free()
{
	__super::Free();
}

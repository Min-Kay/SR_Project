#include "stdafx.h"
#include "UI_Loading.h"
#include "Transform.h"
#include "Camera.h"
#include "Texture.h"
#include "Renderer.h"
#include "VIBuffer_Rect.h"

CUI_Loading::CUI_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUI(pGraphic_Device)
{
}

CUI_Loading::CUI_Loading(const CUI_Loading& rhs)
	:CUI(rhs)
{
}

HRESULT CUI_Loading::NativeConstruct_Prototype()
{
	return __super::NativeConstruct_Prototype();
}

HRESULT CUI_Loading::NativeConstruct(void* pArg)
{
	//if (FAILED(__super::NativeConstruct(pArg)))
		//return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	//if (FAILED(SetUp_Components()))
		//return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	Set_UI(g_iWinCX * 0.5f, g_iWinCY * 0.5f,g_iWinCX, g_iWinCY);

	return S_OK;
}

_int CUI_Loading::Tick(_float fTimeDelta)
{

	m_fFrame += 12.0f * fTimeDelta;

	if (m_fFrame >= 10.0f)
		m_fFrame = 0.f;

	return _int();
}

_int CUI_Loading::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return _int();
}

HRESULT CUI_Loading::Render()
{
	Bind_UI();

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_uint)m_fFrame)))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Loading::SetUp_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CUI_Loading* CUI_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Loading* pInstance = new CUI_Loading(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CUI_Loading::Clone(void* pArg)
{
	CUI_Loading* pInstance = new CUI_Loading(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CUI_Loading::Free()
{
	__super::Free();
}

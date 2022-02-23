#include "stdafx.h"
#include "Ball.h"
#include "Renderer.h"
#include "GameInstance.h"
CBall::CBall(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CEffect(pGraphic_Device)
{
}

CBall::CBall(const CBall& rhs)
	: CEffect(rhs)
{
}

HRESULT CBall::NativeConstruct_Prototype()
{
	if(FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CBall::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBall::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	m_fFrame += 10.0f * fTimeDelta;

	if (m_fFrame >= 27.0f)
		m_fFrame = 0.f;

	return 0;
}

_int CBall::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA,this);
	return 0;
}

HRESULT CBall::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBall::SetUp_Components()
{
	if (FAILED(__super::SetUp_Components()))
		return E_FAIL;

	Set_Texture(TEXT("Prototype_Component_Texture_Ball"));

	return S_OK;
}

HRESULT CBall::SetUp_RenderState()
{
	if (FAILED(__super::SetUp_RenderState()))
		return E_FAIL;

	FaceOn_Camera(false);

	return S_OK;
}

HRESULT CBall::Release_RenderState()
{
	if (FAILED(__super::Release_RenderState()))
		return E_FAIL;
	return S_OK;
}

CBall* CBall::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBall* p_ball = new CBall(pGraphic_Device);
	if(FAILED(p_ball->NativeConstruct_Prototype()))
	{
		Safe_Release(p_ball);
		return nullptr;
	}
	return p_ball;
}

CGameObject* CBall::Clone(void* pArg)
{
	CBall* p_ball = new CBall(*this);
	if (FAILED(p_ball->NativeConstruct(pArg)))
	{
		Safe_Release(p_ball);
		return nullptr;
	}
	return p_ball;
}

void CBall::Free()
{
	__super::Free();
}

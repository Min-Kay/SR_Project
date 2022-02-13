#include "stdafx.h"
#include "Camera_Sub.h"
#include "Texture.h"
#include "VIBuffer_Rect.h"
#include "Renderer.h"
#include "GameInstance.h"
CCamera_Sub* CCamera_Sub::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Sub* pInstance = new CCamera_Sub(pGraphic_Device);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CCamera_Sub::Clone(void* pArg)
{
	CCamera_Sub* pInstance = new CCamera_Sub(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CCamera_Sub::Free()
{
	__super::Free();
	Safe_Release(m_pSurface);
	Safe_Release(m_pTextureCom);
	Safe_Release(pBackBuffer);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pRender);
	Safe_Release(m_pRenderTransform);
}

CCamera_Sub::CCamera_Sub(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CCamera(pGraphic_Device)
{
}

CCamera_Sub::CCamera_Sub(const CCamera_Sub& rhs)
	:CCamera(rhs)
	, m_pSurface(rhs.m_pSurface)
	, m_pTextureCom(rhs.m_pTextureCom)
	, m_pVIBuffer(rhs.m_pVIBuffer)
	, m_pRender(rhs.m_pRender)
	, pBackBuffer(rhs.pBackBuffer)
	, m_pRenderTransform(rhs.m_pRenderTransform)
{
	Safe_AddRef(m_pSurface);
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pVIBuffer);
	Safe_AddRef(pBackBuffer);
	Safe_AddRef(m_pRenderTransform);
}

HRESULT CCamera_Sub::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_pGraphic_Device->GetRenderTarget(0, &pBackBuffer);

	

	return S_OK;
}

HRESULT CCamera_Sub::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;


	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Camera"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pRenderTransform)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRender)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	CTransform::TRANSFORMDESC desc;
	desc.fRotationPerSec = 1.0f;
	desc.fSpeedPerSec = 10.f;
	m_pRenderTransform->Set_TransformDesc(desc);
	m_pRenderTransform->Scaled(_float3(50.f,50.f,50.f));
	m_pRenderTransform->Set_State(CTransform::STATE_POSITION,_float3(0.f, 0.f, 10.f));


	LPDIRECT3DBASETEXTURE9 texture = *m_pTextureCom->GetTexture();
	(static_cast<LPDIRECT3DTEXTURE9>(texture))->GetSurfaceLevel(0, &m_pSurface);

	return S_OK;
}

_int CCamera_Sub::Tick(_float fTimeDelta)
{
	return _int();
}

_int CCamera_Sub::LateTick(_float fTimeDelta)
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	if (pInstance->Get_DIKeyState(DIK_LEFT) & 0x80)
			m_pTransform->Go_Left(fTimeDelta);
	if (pInstance->Get_DIKeyState(DIK_RIGHT) & 0x80)
		m_pTransform->Go_Right(fTimeDelta);
	if (pInstance->Get_DIKeyState(DIK_UP) & 0x80)
		m_pTransform->Go_Straight(fTimeDelta);
	if (pInstance->Get_DIKeyState(DIK_DOWN) & 0x80)
		m_pTransform->Go_BackWard(fTimeDelta);

	if (pInstance->Get_DIKeyState(DIK_J) & 0x80)
		m_pRenderTransform->Go_Left(fTimeDelta);
	if (pInstance->Get_DIKeyState(DIK_L) & 0x80)
		m_pRenderTransform->Go_Right(fTimeDelta);
	if (pInstance->Get_DIKeyState(DIK_I) & 0x80)
		m_pRenderTransform->Go_Straight(fTimeDelta);
	if (pInstance->Get_DIKeyState(DIK_K) & 0x80)
		m_pRenderTransform->Go_BackWard(fTimeDelta);

	if (pInstance->Get_DIKeyState(DIK_U) & 0x80)
		m_pRenderTransform->Turn(_float3(0.f, 1.0f, 0.f),-fTimeDelta);
	if (pInstance->Get_DIKeyState(DIK_O) & 0x80)
		m_pRenderTransform->Turn(_float3(0.f, 1.0f, 0.f), fTimeDelta);

	RELEASE_INSTANCE(CGameInstance);

	m_pRender->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CCamera_Sub::Render()
{

	if (FAILED(m_pRenderTransform->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pVIBuffer->Render();


	return S_OK;
}

HRESULT CCamera_Sub::BeforeRender()
{
	m_pGraphic_Device->SetRenderTarget(0, m_pSurface);
	m_pGraphic_Device->Clear(0,
		nullptr,
		D3DCLEAR_TARGET,
		D3DCOLOR_ARGB(255, 0, 255, 0),	// 백버퍼 색상
		1.f, // z버퍼의 초기화 값
		0);	 // 스텐실 버퍼의 초기화 값


	return __super::BeforeRender();
}

HRESULT CCamera_Sub::AfterRender()
{

	m_pGraphic_Device->SetRenderTarget(0,pBackBuffer);

	return __super::AfterRender();
}

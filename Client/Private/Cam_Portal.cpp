#include "stdafx.h"
#include "Cam_Portal.h"
#include "Renderer.h"
#include "Texture.h"
#include "VIBuffer_Rect.h"

CCam_Portal* CCam_Portal::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCam_Portal* pInstance = new CCam_Portal(pGraphic_Device);
    
    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        Safe_Release(pInstance);
        return nullptr;
    }
    return pInstance;
}

CGameObject* CCam_Portal::Clone(void* pArg)
{
    CCam_Portal* pInstance = new CCam_Portal(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        Safe_Release(pInstance);
        return nullptr;
    }
    return pInstance;
}

void CCam_Portal::Free()
{
    __super::Free();
    //Safe_Release(m_ExitPortal);
    Safe_Release(m_pSurface);
    Safe_Release(m_pTextureCom);
    Safe_Release(pBackBuffer);
    Safe_Release(m_pVIBuffer);
    Safe_Release(m_pRender);
    Safe_Release(m_pRenderTransform);
}

CCam_Portal::CCam_Portal(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CCamera(pGraphic_Device)
{
}

CCam_Portal::CCam_Portal(const CCam_Portal& rhs)
    : CCamera(rhs)
    , m_pSurface(rhs.m_pSurface)
    , m_pRender(rhs.m_pRender)
    , m_pRenderTransform(rhs.m_pRenderTransform)
    , m_pVIBuffer(rhs.m_pVIBuffer)
    , pBackBuffer(rhs.pBackBuffer)
    , m_pTextureCom(rhs.m_pTextureCom)
{
    Safe_AddRef(m_pSurface);
    Safe_AddRef(m_pTextureCom);
    Safe_AddRef(m_pVIBuffer);
    Safe_AddRef(pBackBuffer);
    Safe_AddRef(m_pRenderTransform);
    Safe_AddRef(m_pRender);

}

HRESULT CCam_Portal::NativeConstruct_Prototype()
{
    if (FAILED(__super::NativeConstruct_Prototype()))
        return E_FAIL;

    m_pGraphic_Device->GetRenderTarget(0, &pBackBuffer);

    return S_OK;

}

HRESULT CCam_Portal::NativeConstruct(void* pArg)
{

    if (FAILED(__super::NativeConstruct(pArg)))
        return E_FAIL;


    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Camera"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform_Portal"), (CComponent**)&m_pRenderTransform)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRender)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer)))
        return E_FAIL;

  /*  CTransform::TRANSFORMDESC desc;
    desc.fRotationPerSec = 1.0f;
    desc.fSpeedPerSec = 10.f;
    m_pRenderTransform->Set_TransformDesc(desc);
    m_pRenderTransform->Scaled(_float3(50.f, 50.f, 50.f));
    m_pRenderTransform->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.f, 10.f));*/

    m_pTextureCom->Add_Texture(g_iWinCX,g_iWinCY);
    LPDIRECT3DBASETEXTURE9 texture = *m_pTextureCom->GetTexture(m_pTextureCom->Get_Textures_Count() - 1);
    (static_cast<LPDIRECT3DTEXTURE9>(texture))->GetSurfaceLevel(0, &m_pSurface);
}

_int CCam_Portal::Tick(_float fTimeDelta)
{
    return _int();
}

_int CCam_Portal::LateTick(_float fTimeDelta)
{

    m_pRender->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

    return _int();
}

HRESULT CCam_Portal::Render()
{

    if (FAILED(m_pRenderTransform->Bind_OnGraphicDevice()))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
        return E_FAIL;

    m_pVIBuffer->Render();

    return S_OK;
}

HRESULT CCam_Portal::BeforeRender()
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

HRESULT CCam_Portal::AfterRender()
{
    m_pGraphic_Device->SetRenderTarget(0, pBackBuffer);

    return __super::AfterRender();
}

void CCam_Portal::Set_ExitPortal(CCam_Portal* _exit)
{
    m_ExitPortal = _exit; 
    
    if (nullptr != m_ExitPortal)
    {
      // Safe_AddRef(m_ExitPortal);

       CTransform* opponent =  static_cast<CCamera*>(m_ExitPortal)->Get_CameraTransform();

      m_pRenderTransform->Set_State(CTransform::STATE_RIGHT, opponent->Get_State(CTransform::STATE_RIGHT));
      m_pRenderTransform->Set_State(CTransform::STATE_UP, opponent->Get_State(CTransform::STATE_UP));
      m_pRenderTransform->Set_State(CTransform::STATE_LOOK, opponent->Get_State(CTransform::STATE_LOOK));
      m_pRenderTransform->Set_State(CTransform::STATE_POSITION, opponent->Get_State(CTransform::STATE_POSITION));


      //CTransform* opponent_Portal = static_cast<CTransform*>(m_ExitPortal->Get_Component(TEXT("Com_Transform_Portal")));

      //opponent_Portal->Set_State(CTransform::STATE_RIGHT, m_pTransform->Get_State(CTransform::STATE_RIGHT));
      //opponent_Portal->Set_State(CTransform::STATE_UP, m_pTransform->Get_State(CTransform::STATE_UP));
      //opponent_Portal->Set_State(CTransform::STATE_LOOK, m_pTransform->Get_State(CTransform::STATE_LOOK));
      //opponent_Portal->Set_State(CTransform::STATE_POSITION, m_pTransform->Get_State(CTransform::STATE_POSITION));
    }
}

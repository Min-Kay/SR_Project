#include "stdafx.h"
#include "Cam_Portal.h"
#include "Renderer.h"
#include "Texture.h"
#include "Portal.h"
#include "VIBuffer_Portal.h"

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


    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Camera"), COM_TEXTURE, (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pRenderTransform)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRender)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_PORTAL, COM_BUFFER, (CComponent**)&m_pVIBuffer)))
        return E_FAIL;

    m_pTextureCom->Add_Texture(g_iWinCX,g_iWinCY);
    LPDIRECT3DBASETEXTURE9 texture = *m_pTextureCom->GetTexture(0);
    (static_cast<LPDIRECT3DTEXTURE9>(texture))->GetSurfaceLevel(0, &m_pSurface);

    __super::Add_Exception(CRenderer::RENDER_SKYBOX);
    __super::Add_Exception(CRenderer::RENDER_UI);

    return S_OK;
}

_int CCam_Portal::Tick(_float fTimeDelta)
{
    return _int();
}

_int CCam_Portal::LateTick(_float fTimeDelta)
{
    if (m_ExitPortal)
    {
       m_pRender->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
    }

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
        D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
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

HRESULT CCam_Portal::Set_Cam_Angle(CTransform* portal, CTransform* target)
{
    _float3 targetToRt = m_pRenderTransform->Get_State(CTransform::STATE_POSITION) - target->Get_State(CTransform::STATE_POSITION);
    _float3 rtLook = -m_pRenderTransform->Get_State(CTransform::STATE_LOOK);

    _float3 result = targetToRt - 2 * D3DXVec3Dot(&targetToRt, &rtLook) * rtLook;
    D3DXVec3Normalize(&result, &result);

    _float3 ptLook  = -portal->Get_State(CTransform::STATE_LOOK);
    D3DXVec3Normalize(&ptLook, &ptLook);

    _float3 vLook, vRight, vUp;
    vLook = ptLook - result;
    D3DXVec3Normalize(&vLook, &vLook);
    vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vLook);
    vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);

    m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
    m_pTransform->Set_State(CTransform::STATE_UP, vUp);
    m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);

    _float angle = D3DXVec3Length(&targetToRt) * 5.f;
    if (angle >= 100.f)
        angle = 100.f;
    else if (angle <= 40.f)
        angle = 40.f;

    m_CameraDesc.fFovy = D3DXToRadian(angle);

    return S_OK;
}

void CCam_Portal::Set_ExitPortal(CPortal* _exit)
{
    if(!_exit)
    {
        __super::Set_Vaild(false);
        m_ExitPortal = nullptr;
    }
    else if (_exit)
    {
        m_ExitPortal = _exit->Get_Cam_Portal();

       CTransform* opponent =  static_cast<CTransform*>(_exit->Get_Component(COM_TRANSFORM));

      _float3 vScale = m_pRenderTransform->Get_Scale();
       
      _float3 vRight, vUp, vLook;

		vRight = *D3DXVec3Normalize(&vRight, &opponent->Get_State(CTransform::STATE_RIGHT)) * vScale.x;

		vUp = *D3DXVec3Normalize(&vUp, &opponent->Get_State(CTransform::STATE_UP)) * vScale.y;

		vLook = *D3DXVec3Normalize(&vLook, &opponent->Get_State(CTransform::STATE_LOOK)) * vScale.z;

      m_pRenderTransform->Set_State(CTransform::STATE_RIGHT, vRight);
      m_pRenderTransform->Set_State(CTransform::STATE_UP, vUp);
      m_pRenderTransform->Set_State(CTransform::STATE_LOOK, vLook);
      
      D3DXVec3Normalize(&vLook,&vLook);

       m_pRenderTransform->Set_State (CTransform::STATE_POSITION, opponent->Get_State(CTransform::STATE_POSITION) - vLook * 0.01f );
        __super::Set_Vaild(true);
    }
}

 
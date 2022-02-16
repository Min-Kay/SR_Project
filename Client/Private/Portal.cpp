#include "stdafx.h"
#include "Portal.h"
#include "Cam_Portal.h"
#include "Transform.h"
#include "Renderer.h"
#include "Texture.h"
#include "VIBuffer_Rect.h"
#include "GameInstance.h"

CPortal::CPortal(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CGameObject(pGraphic_Device)
{
}

CPortal::CPortal(const CPortal& rhs)
    : CGameObject(rhs)
    , m_pRenderer(rhs.m_pRenderer)
    , m_pTexture(rhs.m_pTexture)
    , m_pTransform(rhs.m_pTransform)
    , m_pVIBuffer(rhs.m_pVIBuffer)
{
    Safe_AddRef(m_pRenderer);
    Safe_AddRef(m_pTexture);
    Safe_AddRef(m_pTransform);
    Safe_AddRef(m_pVIBuffer);
}


CPortal* CPortal::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
    CPortal* pInstance = new CPortal(pGraphicDevice);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        Safe_Release(pInstance);
        return nullptr; 
    }

    return pInstance;
}

CGameObject* CPortal::Clone(void* pArg)
{
    CPortal* pInstance = new CPortal(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CPortal::Free()
{
    __super::Free();
   // Safe_Release(m_pCam_Portal);
    
    if(nullptr != m_pCam_Portal)
        m_pCam_Portal->Set_ExitPortal(nullptr);
    Safe_Release(m_pRenderer);
    Safe_Release(m_pTexture);
    Safe_Release(m_pVIBuffer);
    Safe_Release(m_pTransform);
}

HRESULT CPortal::NativeConstruct_Prototype()
{
    if (FAILED(__super::NativeConstruct_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPortal::NativeConstruct(void* pArg)
{
    if (FAILED(__super::NativeConstruct(pArg)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransform)))
        return E_FAIL;
    

    PORTALDESC portalDesc = *static_cast<PORTALDESC*>(pArg);

    _float3		vLook = portalDesc.vAt - portalDesc.vEye;
    D3DXVec3Normalize(&vLook, &vLook);

    _float3		vRight = *D3DXVec3Cross(&vRight, &portalDesc.vAxisY, &vLook);
    D3DXVec3Normalize(&vRight, &vRight);

    _float3		vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);
    D3DXVec3Normalize(&vUp, &vUp);

    m_pTransform->Set_State(CTransform::STATE_UP, vUp);
    m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
    m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);
    m_pTransform->Set_State(CTransform::STATE_POSITION, portalDesc.vEye);

    CCamera::CAMERADESC camDesc;
    ZeroMemory(&camDesc,sizeof(CCamera::CAMERADESC));

    camDesc.fAspect = 3/4.f;
    camDesc.fFar = 300.f;
    camDesc.fNear = 0.1f;
    camDesc.fFovy = D3DXToRadian(90.f);
    camDesc.iLevel = portalDesc.iLevel;
    camDesc.vEye = portalDesc.vEye;
    camDesc.vAxisY = portalDesc.vAxisY;
    _float3 dir = portalDesc.vEye - portalDesc.vAt;
    _float3 nor = *D3DXVec3Normalize(&nor, &dir);
    camDesc.vAt = portalDesc.vEye + nor;

    CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
    tag = portalDesc.portalCam;
    if (nullptr == static_cast<CCam_Portal*>(pInstance->Find_Camera_Object(portalDesc.portalCam)))
    {
        if (FAILED(pInstance->Add_Camera_Object(TEXT("Prototype_GameObject_Camera_Portal"), portalDesc.portalCam, &camDesc)))
            return E_FAIL;

        m_pCam_Portal = static_cast<CCam_Portal*>(pInstance->Find_Camera_Object(tag));
    }
    else
    {
        m_pCam_Portal = static_cast<CCam_Portal*>(pInstance->Find_Camera_Object(tag));

        m_pCam_Portal->Set_State(camDesc);
    }

    /* For.Com_Renderer */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderer)))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer)))
        return E_FAIL;


    if (portalDesc.iPortalColor == 0)
    {
        /* For.Com_Texture */
        if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Portal_Orange"), TEXT("Com_Texture"), (CComponent**)&m_pTexture)))
            return E_FAIL;
    }
    else
    {
        /* For.Com_Texture */
        if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Portal_Blue"), TEXT("Com_Texture"), (CComponent**)&m_pTexture)))
            return E_FAIL;
    }
   

    RELEASE_INSTANCE(CGameInstance);
    return S_OK;
}

_int CPortal::Tick(_float fTimeDelta)
{
    return _int();
}

_int CPortal::LateTick(_float fTimeDelta)
{
    m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA,this);

    return _int();
}

HRESULT CPortal::Render()
{

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    if (FAILED(m_pTransform->Bind_OnGraphicDevice()))
        return E_FAIL;

    if (FAILED(m_pTexture->Bind_OnGraphicDevice()))
        return E_FAIL;

    m_pVIBuffer->Render();

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

    return S_OK;
}

void CPortal::Link_Portal(CPortal* opponent)
{
    m_pOpponent = opponent;

    if (nullptr == m_pOpponent)
    {
        m_pCam_Portal->Set_Vaild(false);
        return;
    }

    m_pCam_Portal->Set_ExitPortal(m_pOpponent);
}

CCam_Portal* CPortal::Get_Cam_Portal()
{
    return m_pCam_Portal;
}

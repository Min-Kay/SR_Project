#include "stdafx.h"
#include "FPS.h"

CFPS::CFPS(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CGameObject(pGraphic_Device)
{
}

CFPS::CFPS(const CFPS& rhs)
    : CGameObject(rhs)
    //, m_pCamera(rhs.m_pCamera)
{
}

HRESULT CFPS::NativeConstruct_Prototype()
{
    return S_OK;
}

HRESULT CFPS::NativeConstruct(void* pArg)
{
    return S_OK;
}

_int CFPS::Tick(_float fTimeDelta)
{
    return _int();
}

_int CFPS::LateTick(_float fTimeDelta)
{
    return _int();
}

HRESULT CFPS::Render()
{
    return S_OK;
}

CFPS* CFPS::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CFPS* pInstance = new CFPS(pGraphic_Device);
    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        Safe_Release(pInstance);
        return nullptr;
    }
    return pInstance;
}

CFPS* CFPS::Clone(void* pArg)
{
    CFPS* pInstance = new CFPS(*this);
    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        Safe_Release(pInstance);
        return nullptr;
    }
    return pInstance;
}

void CFPS::Free()
{
    __super::Free();
   // Safe_Release(m_pCamera);
}

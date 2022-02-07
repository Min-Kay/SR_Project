#include "stdafx.h"
#include "Camera_Static.h"
#include "GameInstance.h"

CCamera_Static::CCamera_Static(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CCamera(pGraphic_Device)
{
}

CCamera_Static::CCamera_Static(const CCamera_Static& rhs)
    :CCamera(rhs)
{
}

CCamera_Static* CCamera_Static::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCamera_Static* pInstance = new CCamera_Static(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
        return nullptr;

    return pInstance;
}

CGameObject* CCamera_Static::Clone(void* pArg)
{
    CCamera_Static* pInstance = new CCamera_Static(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
        return nullptr;
    return pInstance;
}

void CCamera_Static::Free()
{
    __super::Free(); 
}

HRESULT CCamera_Static::NativeConstruct_Prototype()
{
    if (FAILED(__super::NativeConstruct_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CCamera_Static::NativeConstruct(void* pArg)
{
    if (FAILED(__super::NativeConstruct(pArg)))
        return E_FAIL;

    return S_OK;
}

_int CCamera_Static::Tick(_float fTimeDelta)
{
    if (0 > __super::Tick(fTimeDelta))
        return -1;

    return _int();
}

_int CCamera_Static::LateTick(_float fTimeDelta)
{
    if (0 > __super::LateTick(fTimeDelta))
        return -1;

    return _int();
}

HRESULT CCamera_Static::Render()
{
    return S_OK;
}

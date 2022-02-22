#include "stdafx.h"
#include "Camera_Player.h"
#include "GameInstance.h"

CCamera_Player::CCamera_Player(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CCamera(pGraphic_Device)
{
}

CCamera_Player::CCamera_Player(const CCamera_Player& rhs)
    : CCamera(rhs)
{
}

CCamera_Player* CCamera_Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCamera_Player* pInstance = new CCamera_Player(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

CGameObject* CCamera_Player::Clone(void* pArg)
{
    CCamera_Player* pInstance = new CCamera_Player(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CCamera_Player::Free()
{
    __super::Free();
}

HRESULT CCamera_Player::NativeConstruct_Prototype()
{
    if (FAILED(__super::NativeConstruct_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CCamera_Player::NativeConstruct(void* pArg)
{
    if (FAILED(__super::NativeConstruct(pArg)))
        return E_FAIL;

    return S_OK;
}

_int CCamera_Player::Tick(_float fTimeDelta)
{
    if (0 > __super::Tick(fTimeDelta))
        return -1;

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    _long		MouseMove = 0;

    if (MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_X))
    {
        m_pTransform->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * MouseMove * 0.1f);
    }

    if (MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_Y))
    {
        m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * 0.1f);
    }

    RELEASE_INSTANCE(CGameInstance);
    __super::Use_Pick(1);

    return _int();
}

_int CCamera_Player::LateTick(_float fTimeDelta)
{
    if (0 > __super::LateTick(fTimeDelta))
        return -1;

    return _int();
}

HRESULT CCamera_Player::Render()
{
    return S_OK;
}

HRESULT CCamera_Player::BeforeRender()
{
    m_pGraphic_Device->Clear(0,
        nullptr,
        D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DCOLOR_ARGB(255, 0, 0, 255),	// 백버퍼 색상
        1.f, // z버퍼의 초기화 값
        0);	 // 스텐실 버퍼의 초기화 값

    return __super::BeforeRender();
}

HRESULT CCamera_Player::AfterRender()
{
    return __super::AfterRender();
}

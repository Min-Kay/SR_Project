#include "stdafx.h"
#include "Camera_Player.h"
#include "GameInstance.h"
#include "Player.h"
#include "UI_BackUI.h"

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
    Safe_Release(m_pPlayer);
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

    __super::Use_Pick(1);

    if (m_Break)
        return 0;

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    if (!m_BackUI)
    {
        m_BackUI = static_cast<CUI_BackUI*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("BackUI"), 0));
        m_BackUI->Off_Menu();
        m_BackUI->Set_Cam(this);
    }

	_long		MouseMove = 0;

    if (MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_X))
    {

        Locked_Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * MouseMove * 0.1f);
    }

    if (MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_Y))
    {
        Locked_Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * 0.1f);
    }

    if (pGameInstance->Get_Key_Down(DIK_ESCAPE))
    {
        Control_Menu();
    }

    RELEASE_INSTANCE(CGameInstance);

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

HRESULT CCamera_Player::Set_Player(CPlayer* _pPlayer)
{
    m_pPlayer = _pPlayer;
    Safe_AddRef(m_pPlayer);
    return S_OK;
}

HRESULT CCamera_Player::Locked_Turn(_float3& axis, _float fTimeDelta)
{
    _float3		vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
    _float3		vUp = m_pTransform->Get_State(CTransform::STATE_UP);
    _float3		vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);

    _float4x4	RotationMatrix;
    D3DXMatrixRotationAxis(&RotationMatrix, &axis, m_pTransform->Get_TransformDesc().fRotationPerSec * fTimeDelta);

    D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
    D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
    D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

    m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);

    if (m_pPlayer)
    {
        _float3 playerLook = static_cast<CTransform*>(m_pPlayer->Get_Component(COM_TRANSFORM))->Get_State(CTransform::STATE_LOOK);

        if (0 >= D3DXVec3Dot(&playerLook, &vLook))
            return S_OK;
    }

    m_pTransform->Set_State(CTransform::STATE_UP, vUp);
    m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);
    return S_OK;
}

void CCamera_Player::Control_Menu()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    if (isCursorOn)
    {
        pGameInstance->SetMouseMode(false, g_hWnd);
        m_BackUI->Off_Menu();
        Set_Break(false);
        isCursorOn = false;
    }
    else
    {
        pGameInstance->SetMouseMode(true);
        m_BackUI->Open_Menu();
        Set_Break(true);
        isCursorOn = true;
    }
    RELEASE_INSTANCE(CGameInstance);
}

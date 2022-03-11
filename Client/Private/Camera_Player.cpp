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

    CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
    Engine::CUI::UIDESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.WinCX = g_iWinCX;
    desc.WinCY = g_iWinCY;
    desc.PosX = g_iWinCX * 0.5f;
    desc.PosY = g_iWinCY * 0.5f;
    desc.SizeX = 50.f;
    desc.SizeY = 50.f;
    desc.Alpha = Engine::CUI::ALPHA_TEST;
    desc.Func = D3DCMP_GREATER;
    desc.Ref = 70;
    desc.Texture = TEXT("Prototype_Component_Texture_Crosshair");
    desc.Shader_Style = SHADER_SETCOLOR_BLEND;
    desc.Shader_Control = &g_ControlShader;
    if (FAILED(p_instance->Add_GameObject(g_CurrLevel, TEXT("Crosshair"), PROTO_UI, &desc)))
        return E_FAIL;

    RELEASE_INSTANCE(CGameInstance);
    return S_OK;
}

_int CCamera_Player::Tick(_float fTimeDelta)
{
    if (0 > __super::Tick(fTimeDelta))
        return -1;

    // __super::Use_Pick(1);

    if (m_Break)
        return 0;

    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


    if (!m_BackUI)
    {
        m_BackUI = static_cast<CUI_BackUI*>(pGameInstance->Get_GameObject(g_CurrLevel, TEXT("BackUI"), 0));
        if (!m_BackUI)
            return -1;
        m_BackUI->Off_Menu();
        m_BackUI->Set_Cam(this);
        Control_Menu(false);

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
        Control_Menu(!isCursorOn);
    }

    if (pGameInstance->Get_Key_Down(DIK_3))
    {
        g_ControlShader -= 0.05f;

    }

    if (pGameInstance->Get_Key_Down(DIK_4))
    {
        g_ControlShader += 0.05f;

    }

    if(pGameInstance->Get_Key_Up(DIK_E))
    {
        if (isGrabed)
        {
            Drop_Interaction();
        }
        else
            Grab_Interaction();
    }


    RELEASE_INSTANCE(CGameInstance);

    if(m_GrabInteraction)
    {
        _float3 vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
        D3DXVec3Normalize(&vLook, &vLook);

        m_GrabInteractionTr->Set_State(CTransform::STATE_POSITION, m_pTransform->Get_State(CTransform::STATE_POSITION) + vLook * 1.5f);
        m_GrabInteractionTr->Set_Velocity(0);
	    
    }

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


    if (m_pPlayer)
    {
        _float3 playerLook = static_cast<CTransform*>(m_pPlayer->Get_Component(COM_TRANSFORM))->Get_State(CTransform::STATE_LOOK);

        if (0 >= D3DXVec3Dot(&playerLook, &vLook))
            return S_OK;
    }

    m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
    m_pTransform->Set_State(CTransform::STATE_UP, vUp);
    m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);
    return S_OK;
}

void CCamera_Player::Control_Menu(_bool _bool)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    if (!_bool)
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

void CCamera_Player::Grab_Interaction()
{

    _float4		vTargetPos = { 0.f,0.f,0.f,1.f };
    //_float4		vTargetPos = { 0.5f,0.5f,0.f,1.f };

    /* 뷰스페이스 상의 위치로 변환한다. */
    /* 로컬위치 * 월드 * 뷰 */

    _float4x4		ProjMatrix;

    _float3 m_vRayDirCH, m_vRayPosCH;

    const CCamera::CAMERADESC camDesc = m_CameraDesc;
    D3DXMatrixPerspectiveFovLH(&ProjMatrix, camDesc.fFovy, camDesc.fAspect, camDesc.fNear, camDesc.fFar);
    D3DXMatrixInverse(&ProjMatrix, nullptr, &ProjMatrix);
    D3DXVec4Transform(&vTargetPos, &vTargetPos, &ProjMatrix);
    memcpy(&m_vRayDirCH, &(vTargetPos - _float4(0.f, 0.f, 0.f, 1.f)), sizeof(_float3));

    m_vRayPosCH = _float3(0.f, 0.f, 0.f);

    /* 월드스페이스 상의 위치로 변환한다. */
    /* 로컬위치 * 월드 */
    const _float4x4		ViewMatrixInverse = m_pTransform->Get_WorldMatrix();
    D3DXVec3TransformNormal(&m_vRayDirCH, &m_vRayDirCH, &ViewMatrixInverse);
    D3DXVec3TransformCoord(&m_vRayPosCH, &m_vRayPosCH, &ViewMatrixInverse);

    D3DXVec3Normalize(&m_vRayDirCH, &m_vRayDirCH);



    CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
    list<CCollision_Manager::COLLPOINT> collList = p_instance->Get_Ray_Collision_List(m_vRayDirCH,m_vRayPosCH,1.f , true);

    if(collList.empty())
    {
        RELEASE_INSTANCE(CGameInstance);
        return;
    }


    for(auto& i : collList)
    {
        if(i.CollObj->Get_Type() == OBJ_INTERACTION)
        {
            m_GrabInteraction = i.CollObj;
            static_cast<CBoxCollider*>(m_GrabInteraction->Get_Component(COM_COLLIDER))->Set_CollStyle(CCollider::COLLSTYLE_TRIGGER);
            isGrabed = true;
            m_GrabInteractionTr = static_cast<CTransform*>(m_GrabInteraction->Get_Component(COM_TRANSFORM));
            m_GrabInteraction->Set_Grab(true);
            break;
        }
    }


    RELEASE_INSTANCE(CGameInstance);

}

void CCamera_Player::Drop_Interaction()
{
    if (!isGrabed || !m_GrabInteraction)
        return;

    static_cast<CBoxCollider*>(m_GrabInteraction->Get_Component(COM_COLLIDER))->Set_CollStyle(CCollider::COLLSTYLE_ENTER);
    m_GrabInteraction->Set_Grab(false);
    m_GrabInteraction = nullptr;
    m_GrabInteractionTr = nullptr;
    isGrabed = false;
}


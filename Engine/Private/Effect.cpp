#include "Effect.h"
#include "Transform.h"
#include "Renderer.h"
#include "VIBuffer_Rect.h"
#include "Texture.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CGameObject(pGraphic_Device)
{
}

CEffect::CEffect(const CEffect& rhs)
    : CGameObject(rhs)
    ,m_pRendererCom(rhs.m_pRendererCom)
    ,m_pTextureCom(rhs.m_pTextureCom)
    ,m_pTransformCom(rhs.m_pTransformCom)
    ,m_pVIBufferCom(rhs.m_pVIBufferCom)
{
}

HRESULT CEffect::SetUp_Components()
{
    /* For.Com_Transform */
    CTransform::TRANSFORMDESC		TransformDesc;
    ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

    TransformDesc.fSpeedPerSec = 5.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

    if (FAILED(__super::Add_Component(0, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransformCom, &TransformDesc)))
        return E_FAIL;

    /* For.Com_Renderer */
    if (FAILED(__super::Add_Component(0, PROTO_RENDERER,COM_RENDERER, (CComponent**)&m_pRendererCom)))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(0, PROTO_RECT, COM_BUFFER, (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CEffect::SetUp_RenderState()
{
    if (nullptr == m_pGraphic_Device)
        return E_FAIL;

    return S_OK;
}

HRESULT CEffect::Release_RenderState()
{
    if (nullptr == m_pGraphic_Device)
        return E_FAIL;

    return S_OK;
}

HRESULT CEffect::Set_Texture(const _tchar* _tag)
{
    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(0, _tag, COM_TEXTURE, (CComponent**)&m_pTextureCom)))
        return E_FAIL;
    return S_OK;
}

HRESULT CEffect::FaceOn_Camera(_bool fixY)
{
    _float4x4		ViewMatrix;
    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
    D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

    if (nullptr == m_pTransformCom)
        return E_FAIL;
    _float3 vScale = m_pTransformCom->Get_Scale();

    _float3 vRight = (*(_float3*)&ViewMatrix.m[0][0]);
    _float3 vUp = (*(_float3*)&ViewMatrix.m[1][0]);
    _float3 vLook = (*(_float3*)&ViewMatrix.m[2][0]);

    D3DXVec3Normalize(&vRight, &vRight);
    D3DXVec3Normalize(&vUp, &vUp);
    D3DXVec3Normalize(&vLook, &vLook);

    m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
    if (!fixY)
        m_pTransformCom->Set_State(CTransform::STATE_UP, vUp * vScale.y);
    m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook * vScale.z);

    return S_OK;
}

CEffect* CEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CEffect* pInstance = new CEffect(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        Safe_Release(pInstance);
        return nullptr;
    }
    return pInstance;
}

CGameObject* CEffect::Clone(void* pArg)
{
    CEffect* pInstance = new CEffect(*this);

    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        Safe_Release(pInstance);
        return nullptr;
    }
    return pInstance;
}

void CEffect::Free()
{
    __super::Free();
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pRendererCom);
}


HRESULT CEffect::NativeConstruct_Prototype()
{
    if (FAILED(__super::NativeConstruct_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CEffect::NativeConstruct(void* pArg)
{
    if (FAILED(__super::NativeConstruct(pArg)))
        return E_FAIL;

    /* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
    if (FAILED(SetUp_Components()))
        return E_FAIL;

    return S_OK;
}

_int CEffect::Tick(_float fTimeDelta)
{
    if (0 > __super::Tick(fTimeDelta))
        return -1;


    return _int();
}

_int CEffect::LateTick(_float fTimeDelta)
{
    if (0 > __super::LateTick(fTimeDelta))
        return -1;

    if (nullptr == m_pRendererCom)
        return -1;

    return _int();
}

HRESULT CEffect::Render()
{
    if (nullptr == m_pVIBufferCom)
        return E_FAIL;

    SetUp_RenderState();

    if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
        return E_FAIL;


    if (FAILED(m_pTextureCom->Bind_OnGraphicDevice((_uint)m_fFrame)))
        return E_FAIL;

    m_pVIBufferCom->Render();

    Release_RenderState();


    return S_OK;
}

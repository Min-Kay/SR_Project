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

    if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
        return E_FAIL;

    /* For.Com_Renderer */
    if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
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

    /* ���� ��ü���� �߰��Ǿ���� ������Ʈ���� ����(or ����)�Ͽ� ��������� �����Ѵ�.  */
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

    if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
        return E_FAIL;


    return S_OK;
}
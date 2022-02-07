#include "stdafx.h"
#include "Map.h"
#include "GameInstance.h"
#include "VIBuffer_Terrain.h"

CMap::CMap(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CGameObject(pGraphic_Device)
{
}

CMap::CMap(const CMap& rhs)
    :CGameObject(rhs)
{
}

HRESULT CMap::SetUp_Components()
{
    /* For.Com_Transform */
    CTransform::TRANSFORMDESC		TransformDesc;
    ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

    TransformDesc.fSpeedPerSec = 5.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc))) 
        return E_FAIL;

    /* For.Com_Renderer */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom))) 
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom))) 
        return E_FAIL;



    return S_OK;
}

CMap* CMap::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CMap* pInstance = new CMap(pGraphic_Device);

    if (FAILED(pInstance->NativeConstruct_Prototype()))
    {
        MSGBOX("Failed to Creating CMap");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMap::Clone(void* pArg)
{
    /* 새로운객체를 복제하여 생성한다. */
    CMap* pInstance = new CMap(*this);


    if (FAILED(pInstance->NativeConstruct(pArg)))
    {
        MSGBOX("Failed to Creating CMap");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CMap::Free()
{
    __super::Free();
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pRendererCom);

}

HRESULT CMap::NativeConstruct_Prototype()
{
    if (FAILED(__super::NativeConstruct_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CMap::NativeConstruct(void* pArg)
{
    if (FAILED(__super::NativeConstruct(pArg)))
        return E_FAIL;

    /* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
    if (FAILED(SetUp_Components()))
        return E_FAIL;

    return S_OK;
}

_int CMap::Tick(_float fTimeDelta)
{
    if (0 > __super::Tick(fTimeDelta))
        return -1;
    return _int();
}

_int CMap::LateTick(_float fTimeDelta)
{
    if (0 > __super::LateTick(fTimeDelta))
        return -1;

    if (nullptr == m_pRendererCom)
        return -1;

    m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);

    return _int();
}

HRESULT CMap::Render()
{
    if (nullptr == m_pVIBufferCom)
        return E_FAIL;

    if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
        return E_FAIL;

    m_pVIBufferCom->Render();

    return S_OK;
}

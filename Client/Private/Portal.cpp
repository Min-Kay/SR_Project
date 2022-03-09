#include "stdafx.h"
#include "Portal.h"
#include "Cam_Portal.h"
#include "Transform.h"
#include "Renderer.h"
#include "Texture.h"
#include "VIBuffer_Rect.h"
#include "GameInstance.h"
#include "BoxCollider.h"
#include "Enemy.h"
#include "Shader.h"
#include "Player.h"

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
    , m_Collider(rhs.m_Collider)
	, m_pShader(rhs.m_pShader)
{
    Safe_AddRef(m_Collider);
    Safe_AddRef(m_pRenderer);
    Safe_AddRef(m_pTexture);
    Safe_AddRef(m_pTransform);
    Safe_AddRef(m_pVIBuffer);
    Safe_AddRef(m_pShader);
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

    if(nullptr != m_pCam_Portal)
        m_pCam_Portal->Set_ExitPortal(nullptr);
    Safe_Release(m_Collider);
    Safe_Release(m_pRenderer);
    Safe_Release(m_pTexture);
    Safe_Release(m_pVIBuffer);
    Safe_Release(m_pTransform);
    Safe_Release(m_pShader);
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

    if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransform)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_COLLIDER, COM_COLLIDER, (CComponent**)&m_Collider)))
        return E_FAIL;

    CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

    Set_Type(OBJ_PORTAL);

    m_Collider->Set_ParentInfo(this);
    m_Collider->Set_CollStyle(CCollider::COLLSTYLE_TRIGGER);
    m_Collider->Set_State(CBoxCollider::COLL_SIZE, _float3(0.4f,0.4f,0.4f));

    PORTALDESC portalDesc = *static_cast<PORTALDESC*>(pArg);

    _float3		vLook = portalDesc.vAt - portalDesc.vEye;
    D3DXVec3Normalize(&vLook, &vLook);

    _float3		vRight = *D3DXVec3Cross(&vRight, &portalDesc.vAxisY, &vLook);
    if(vRight.x == 0.f && vRight.y == 0.f && vRight.z == 0.f)
    {
        vRight = *D3DXVec3Cross(&vRight, &(_float3(0.f,0.f,1.f)), &vLook);
        portalDesc.vAxisY = _float3(0.f, 0.f, 1.f);
    }
    D3DXVec3Normalize(&vRight, &vRight);

    _float3		vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);
    D3DXVec3Normalize(&vUp, &vUp);

    m_pTransform->Set_State(CTransform::STATE_UP, vUp);
    m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
    m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);
    m_pTransform->Set_State(CTransform::STATE_POSITION, portalDesc.vEye);

    CCamera::CAMERADESC camDesc;
    ZeroMemory(&camDesc,sizeof(CCamera::CAMERADESC));

    camDesc.fAspect = 3/(4.f);
    camDesc.fFar = 100.f;
    camDesc.fNear = 0.1f;
    camDesc.fFovy = D3DXToRadian(30.f);
    camDesc.iLevel = portalDesc.iLevel;
    camDesc.vEye = portalDesc.vEye;
    camDesc.vAxisY = portalDesc.vAxisY;
    _float3 nor = portalDesc.vAt - portalDesc.vEye;
    D3DXVec3Normalize(&nor,&nor);
    camDesc.vAt = portalDesc.vEye - nor;
 
    tag = portalDesc.portalCam;


    m_pCam_Portal = static_cast<CCam_Portal*>(pInstance->Find_Camera_Object(tag));
    if (nullptr == m_pCam_Portal)
    {
        if (FAILED(pInstance->Add_Camera_Object(CAM_PORTAL, portalDesc.portalCam, &camDesc)))
            return E_FAIL;
        m_pCam_Portal = static_cast<CCam_Portal*>(pInstance->Find_Camera_Object(tag));
    }

    m_pCam_Portal->Set_State(camDesc);

    /* For.Com_Renderer */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRenderer)))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RECT, COM_BUFFER, (CComponent**)&m_pVIBuffer)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_SHADER_RECT, COM_SHADER, (CComponent**)&m_pShader)))
        return E_FAIL;

    if (portalDesc.iPortalColor == PORTAL_ORANGE)
    {
        /* For.Com_Texture */
        if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Portal_Orange"), COM_TEXTURE, (CComponent**)&m_pTexture)))
            return E_FAIL;
    }
    else
    {
        /* For.Com_Texture */
        if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Portal_Blue"), COM_TEXTURE, (CComponent**)&m_pTexture)))
            return E_FAIL;
    }

    RELEASE_INSTANCE(CGameInstance);
    return S_OK;
}

_int CPortal::Tick(_float fTimeDelta)
{
    m_Collider->Set_Collider();

	return _int();
}

_int CPortal::LateTick(_float fTimeDelta)
{
    Portaling();
    m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA,this);

    return _int();
}

HRESULT CPortal::Render()
{
   /* if (FAILED(m_pTransform->Bind_OnGraphicDevice()))
        return E_FAIL;

    if (FAILED(m_pTexture->Bind_OnGraphicDevice()))
        return E_FAIL;


	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 20);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


    m_pVIBuffer->Render();


    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);*/

    m_pTransform->Bind_OnShader(m_pShader);

    m_pShader->SetUp_ValueOnShader("g_ColorStack", &g_ControlShader, sizeof(_float));
    m_pTexture->Bind_OnShader(m_pShader, "g_Texture", 0);
    m_pShader->Begin_Shader(SHADER_SETCOLOR_BLEND);
    m_pVIBuffer->Render();
    m_pShader->End_Shader();

    return S_OK;
}

void CPortal::Link_Portal(CPortal* opponent)
{
    m_pOpponent = opponent; 

    if (nullptr == m_pOpponent)
    {
        m_pCam_Portal->Set_Vaild(false);
        Release_Portal();
        return;
    }

    Collide_Added = true;
    CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
    p_instance->Add_Collider(CCollision_Manager::COLLOBJTYPE_STATIC, m_Collider);
    RELEASE_INSTANCE(CGameInstance);
    m_pCam_Portal->Set_ExitPortal(m_pOpponent);
}

CPortal* CPortal::Get_Link_Portal()
{
    return m_pOpponent;
}

CCam_Portal* CPortal::Get_Cam_Portal()
{
    return m_pCam_Portal;
}

void CPortal::Set_Cam_Angle(CTransform* target)
{
    if (nullptr == m_pCam_Portal)
        return; 
    
    m_pCam_Portal->Set_Cam_Angle(m_pTransform,target);
}

void CPortal::Portaling()
{
    if (!m_pOpponent || !Collide_Added)
        return;

    CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
    list<CGameObject*> collList = p_instance->Get_Collision_Object_List(m_Collider);

    if (collList.empty())
    {
        RELEASE_INSTANCE(CGameInstance);
        return;
    }

    for (auto& obj : collList)
    {
        if (obj->Get_Type() == OBJ_STATIC || obj == m_pOpponent)
            continue;

        if (obj->Get_Type() == OBJ_ENEMY && !static_cast<CEnemy*>(obj)->Get_CanPortal())
            continue;

        CTransform* objTr = static_cast<CTransform*>(obj->Get_Component(COM_TRANSFORM));
        if (objTr == nullptr)
            continue;

        objTr->Set_OnCollide(false);

        CTransform* opponentTr = static_cast<CTransform*>(m_pOpponent->Get_Component(COM_TRANSFORM));
        _float3 vOpLook = opponentTr->Get_State(CTransform::STATE_LOOK);
        D3DXVec3Normalize(&vOpLook, &vOpLook);
        _float3 vScale = objTr->Get_Scale();
		objTr->Set_State(CTransform::STATE_POSITION, opponentTr->Get_State(CTransform::STATE_POSITION) - vOpLook * vScale.x);
        objTr->Set_Force(-vOpLook);

    	if (obj->Get_Type() != OBJ_PLAYER)
	    {
    		_float3 vRight, vUp, vLook;
         
          vRight = -opponentTr->Get_State(CTransform::STATE_RIGHT);
          vUp = opponentTr->Get_State(CTransform::STATE_UP);
          vLook = -opponentTr->Get_State(CTransform::STATE_LOOK);
          D3DXVec3Normalize(&vRight, &vRight);
          D3DXVec3Normalize(&vUp, &vUp);
          D3DXVec3Normalize(&vLook, &vLook);

          objTr->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
          objTr->Set_State(CTransform::STATE_UP, vUp * vScale.y);
          objTr->Set_State(CTransform::STATE_LOOK, vLook * vScale.z);

          if (obj->Get_Type() == OBJ_ENEMY)
              static_cast<CEnemy*>(obj)->Set_Portaling(true);
	    }
    }

	RELEASE_INSTANCE(CGameInstance);
}

void CPortal::Release_Portal()
{
    if (!Collide_Added)
        return;

    Collide_Added = false;
    CGameInstance* p_instance = GET_INSTANCE(CGameInstance);
    p_instance->Release_Collider(CCollision_Manager::COLLOBJTYPE_STATIC, m_Collider);
    RELEASE_INSTANCE(CGameInstance);

}

#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CTexture;
class CVIBuffer_Rect;
class CBoxCollider;
END

BEGIN(Client)
class CPlayer;
class CMissile;
class CBoss;
class CTargeting :
    public CGameObject
{
protected:
    explicit CTargeting(LPDIRECT3DDEVICE9 pGraphic_Device);
    explicit CTargeting(const CTargeting& rhs);
    virtual ~CTargeting() = default;

public:
    HRESULT NativeConstruct_Prototype() override;
    HRESULT NativeConstruct(void* pArg) override;
    _int Tick(_float fTimeDelta) override;
    _int LateTick(_float fTimeDelta) override;
    HRESULT Render() override;

    HRESULT    Set_RanderState();
    HRESULT    Release_RanderState();
    HRESULT SetUp_Component();
    HRESULT    MainMoving(_float fTimeDelta);
    HRESULT    ColliderCheck();

    _float3 BezierCurve(_float3 P0, _float3 P1, _float3 P2, _float time);

    _bool    Get_CheckCollider() { return m_bcheckCollider; }

    HRESULT MainTarget(_float fTimeDelta);
    HRESULT SubTarget(_float fTimeDelta);

    typedef struct tagTarget
    {
        _float3    Pos1;
        _float3    Pos2;
        _float3    Pos3;
        _bool    MainTaret;
        void* Parent;

    }TARGET;

private:

    CTexture* m_pTextureCom = nullptr;

    CVIBuffer_Rect* m_pVIBufferCom = nullptr;

    CTransform* m_pTransformCom = nullptr;

    CRenderer* m_pRendererCom = nullptr;

    CBoxCollider* m_pBoxColliderCom = nullptr;


private:
    _float3            m_targetPos;
    CPlayer* m_pPlayer = nullptr;;
    CMissile* m_pMissile = nullptr;
    CBoss* m_pBoss = nullptr;;
    _float            m_fFrame = 0.0f;
    _float            timer = 0.0f;
    _float            deletetimer = 0.0f;
    _float3            m_ColliderPos;
    CTransform* m_pTarget = nullptr;
    _bool            m_bcheckCollider = false;
    TARGET            m_Target;
    _bool            m_fire = false;
    _float3            BezierPos;
public:
    static CTargeting* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    CGameObject* Clone(void* pArg) override;
    void Free() override;
};
END


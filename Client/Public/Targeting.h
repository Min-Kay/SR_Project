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

	HRESULT	Set_RanderState();
	HRESULT	Release_RanderState();
	HRESULT SetUp_Component();
	HRESULT FaceOn_Camera();

	_bool	Get_CheckCollider() { return m_bcheckCollider; }

	typedef struct tagTarget
	{
		_float3	targetPos;
		_bool	MainTarget;
		_float	SubTargetRangeX;
		_float	SubTargetRangeY;
	
	}TARGET;

private:

	CTexture*			m_pTextureCom = nullptr;

	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

	CTransform*			m_pTransformCom = nullptr;

	CRenderer*			m_pRendererCom = nullptr;

	CBoxCollider*		m_pBoxColliderCom = nullptr;


private:
	_float3 m_targetPos;
	CPlayer* m_pPlayer = nullptr;;
	CMissile*	m_pMissile = nullptr;

	_float m_fFrame;
	_float	timer = 0.0f;
	_float3			m_ColliderPos;
	CTransform*		m_pTarget = nullptr;
	_bool			m_bcheckCollider = false;
	TARGET			m_Target;
	_bool			m_fire = false;
public:
	static CTargeting* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};
END


#pragma once
#include "Enemy.h"


BEGIN(Engine)
class CRenderer;
class CBoxCollider;
class CTexture;
class CTransform;
class CVIBuffer_Cube;
END

BEGIN(Client)
class CArm;
class CPlayer;
class CTargeting;
class CBoss;

class CMissile :
	public CEnemy
{
protected:
	explicit CMissile(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	explicit CMissile(const CMissile& rhs);
	virtual ~CMissile() = default;

public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;

	_float3 Get_Bezier() { return m_fFront_BezierPos; }
	HRESULT Set_Bezier(_float3  Pos);

	typedef enum tagArmMissle { ARMMISSLE_LEFT, ATMMISSLE_RIGHT, ATMMISSLE_END }ArmMissle;


	typedef struct tagarmmissle {
		ArmMissle ArmMissle;
		_float	FireCount;
		_bool	Left;
		_bool	Right;
		void* pParent;
	}ARMMISSLE;
private:
	HRESULT SetUp_Component();
	HRESULT SetUp_First();
	_bool Check_Dead();
	HRESULT Check_ColliderTarget();
	HRESULT First_Bezier(_float fTimeDelta);
	HRESULT Targeting_Main_Sub();
	HRESULT Missle_Move(_float fTimeDelta);
	//HRESULT Check_Portaling(_float fTimeDelta);
	_float3		BezierCurve(_float3 P0, _float3 P1, _float3 P2, _float3 P3, _float time);
	void Impact(_float3 _Pos);
protected:
	CVIBuffer_Cube* m_pVIBuffer = nullptr;
	CRenderer* m_pRenderer = nullptr;
	CTexture* m_pTexture = nullptr;
	CTransform* m_pTransform = nullptr;
	CBoxCollider* m_pBoxCollider = nullptr;

private:
	CArm* m_Arm = nullptr;
	CPlayer* m_pPlayer = nullptr;
	CTargeting* m_pTageting = nullptr;
	CBoss* m_pBoss = nullptr;


	_float3        Add_RandomPos;
	ARMMISSLE	m_ArmMissle;
	_float3		PlayerPos;
	_float3		MissilePos;
	_float3		m_fvecdir;
	_int		m_Damage = 10;
	_float		m_Timer = 0.f;
	_float3		RandPos1;
	_float3		RandPos2;
	_float3		BossPos;
	_float3		BossUp;

	_float m_fMissileTimer = 0.f;
	_float m_fTargetTimer = 0.f;
	_float LerpTimeer;
	_float3 m_fBezierPos;
	_float3 m_fFront_BezierPos;
	_float m_ftimer;
	_bool	m_bDEAD = false;

	_bool	m_bTargetCollider = false;
	_uint	Count = 0;

	CTargeting* pTarget = nullptr;
	CTransform* targetTrans = nullptr;

public:
	static CMissile* Create(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};
END

#pragma once
#include "Enemy.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CVIBuffer_Cube;
class CRenderer;
class CBoxCollider;
END

BEGIN(Client)
class CPlayer;
class CArm;
class CAttackRange;

class CBoss :
    public CEnemy
{
protected:
	explicit CBoss(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	explicit CBoss(const CBoss& rhs);
	virtual ~CBoss() = default;

public:
	typedef enum tagBossState {BOSS_IDLE, BOSS_MOVE, BOSS_ATTACK,BOSS_PHASECHANGE, BOSS_DIE}BOSSSTATE;
	typedef enum tagBossPhase {BOSS_PHASEONE, BOSS_PHASETWO}BOSSPHASE;
	typedef enum tagBossAttack { BOSSATT_MISSILE, BOSSATT_PUNCH, BOSSATT_MIXED }BOSSATTACK;


public:
	typedef enum tagArmSide{ARM_LEFT,ARM_RIGHT}ARM;
public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;
	HRESULT SetUp_Component() override;

public:
	void Set_InitPos(_float3 _pos);

private:
	void Synchronize_Transform();
	void Set_BossState(BOSSSTATE _state);

private:
	// 내꺼
	void Init_Idle();

	void Resizing(_float fTimeDelta);
	void Sizing(_float fTimeDelta);
	void Reset_Arm_Direction(ARM _arm);
	_bool InitArmPosition(_float fTimeDelta);
	_bool Move_By_Bazier(ARM _arm, _float fTimeDelta);
	void Set_ArmPos(ARM _arm, _float3 _start, _float3 _mid, _float3 _end);
	void Blowing(_float fTimeDelta);
	void Randomize_Pattern(_float fTimeDelta);
	void Sizing_Particles();


private:
	// 인우형 꺼

	void Init_Attack_Punch();

	// 주먹이 범위 안에 들어왔을 때
	void InRange();

	// 한번만 Range 호출해서 그 순간 플레이어 위치 받아오기
	_int Set_RangePos();

	// Range랑 주먹이랑 충돌했나 안했나 bool값 반환 함수
	_bool IsCollision();

private:
	// 태우형 꺼
	void Init_Attack_Missile();
private:
	void State_Machine(_float fTimeDelta);
	void Idle(_float fTimeDelta);
	void Move(_float fTimeDelta);
	void Phase(_float fTimeDelta);
	void Attack(_float fTimeDelta);
	void Die(_float fTimeDelta);

private:
	void Attack_Missile(_float fTimeDelta);
	void Attack_Punch(_float fTimeDelta);
	void Attack_Mixed(_float fTimeDelta);
private:
	_float m_fTimer = 0.f; 
	_uint m_ImageIndex = 0;


private:
	// 인우형 패턴 변수
	CAttackRange* m_pAttackRange = nullptr;
	CTransform* m_RangeTrans = nullptr;
	_float3				m_playerPos = { 0.f, 0.f, 0.f };
	_bool				m_bRangeValid = false;
	_bool				m_bCalled = true;

private:
	// 태우형 패턴 변수
	_bool  m_bMissile = false;

private:
	// 내꺼
	_float3 m_vScale = _float3(5.f,5.f,5.f);

	_bool m_init = false;

	_float m_AttPatternTimer = 0.f;

	// Idle
	_bool initPos = false;
	_bool idlePos = true;

	_float m_LeftTimer = 0.f;
	_float m_RightTimer = 0.f;

	_float3  leftArmBazier[3];
	_float3  rightArmBazier[3];

	_float m_fUpMidPos = 20.f;
	_float m_fRightMidPos = 30.f;

	_float m_fUpPos = 1.f;
	_float m_fRightPos = 10.f;

	// Move
	_float m_fMoveLength = 30.f;


	// Init Positioning

	_bool m_Resizing = false;
	_bool m_Sizing = false;
	_bool m_Reset = false;

	_float m_TurnTime = 2.f;

	_float3 m_InitPos = _float3(0.f,0.f,0.f);
	_float3 m_SizingAxis = _float3(1.f, 1.f, 0.f);
	

	// Attack_Mixed
	_float m_ChargingTime = 3.f;
	_float m_StrikingTime = 5.f;

	_bool m_Charging = false;
	_bool m_Striking = false;


	// Die
	_bool m_Dying = false;
	

private:
	BOSSPHASE m_Phase = BOSS_PHASEONE;
	BOSSSTATE m_State = BOSS_IDLE;

	BOSSATTACK m_AttState;

private:
	CPlayer* m_pPlayer = nullptr;
	CTransform* m_pPlayerTr = nullptr;


	CArm* m_LeftArm = nullptr;
	CArm* m_RightArm = nullptr;

	CTransform* m_LeftArmTr = nullptr;
	CTransform* m_RightArmTr = nullptr;


private:
	CTransform* m_pOnlyRotation = nullptr;
	CTransform* m_pTransform = nullptr;
	CRenderer* m_pRenderer = nullptr;
	CTexture* m_pTexture = nullptr;
	CBoxCollider* m_pCollider = nullptr;
	CVIBuffer_Cube* m_pBuffer = nullptr;


public:
	static CBoss* Create(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};

END

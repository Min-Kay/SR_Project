#pragma once
#include "Enemy.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CVIBuffer_Cube;
class CRenderer;
class CBoxCollider;
class CUI;
class CShader;
END

BEGIN(Client)
class CPlayer;
class CArm;
class CAttackRange;
class CShield;
class CMissile;
class CTargeting;
class CShield_Effect;

class CBoss :
    public CEnemy
{
protected:
	explicit CBoss(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	explicit CBoss(const CBoss& rhs);
	virtual ~CBoss() = default;

public:
	typedef enum tagBossState {BOSS_IDLE, BOSS_MOVE, BOSS_ATTACK, BOSS_GROGY, BOSS_DIE}BOSSSTATE;
	typedef enum tagBossPhase {BOSS_PHASEONE, BOSS_PHASETWO}BOSSPHASE;
	typedef enum tagBossAttack { BOSSATT_MISSILE, BOSSATT_PUNCH, BOSSATT_ROLLING}BOSSATTACK;
	typedef enum tagBossRage { BOSSRAGE_SUNFLOWER, BOSSRAGE_LASER }BOSSRAGE;



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
	void Add_HP(_int _add) override;
	const _int& Get_InitHP() const;


	void Set_OnShield(_bool _bool);
	const _bool Get_OnShield() const;

	void Add_ShieldHp(_int _add);
	const _int Get_ShieldHp();

	void Set_Grogy();

	const BOSSPHASE& Get_Phase() const { return m_Phase; }

private:
	void Synchronize_Transform();
	void Set_BossState(BOSSSTATE _state);

private:
	// 내꺼
	void Init_Idle();

	void Resizing(_float fTimeDelta, _float3 position);
	void Sizing(_float fTimeDelta);
	void Reset_Arm_Direction(ARM _arm);
	_bool InitArmPosition(_float fTimeDelta, _bool _left = true, _bool _right = true);
	_bool Move_By_Bazier(ARM _arm, _float fTimeDelta);
	void Set_ArmPos(ARM _arm, _float3 _start, _float3 _mid, _float3 _end);
	void Blowing(_float fTimeDelta);
	void Randomize_Pattern(_float fTimeDelta);
	void Sizing_Particles(_float4 _color, _int time, _float _speed);

	void Gravity_Blowing(_float fTimeDelta, _float PosY, _bool _watchPlayer);
	void Arm_Posing(_float fTimeDelta, _bool _left = true, _bool _right = true);

	void Start_Pattern(_tchar* filename);
	void Spawn_Shield();
	void Shield_Effect(_float fTimeDelta);


	void Init_Attack_Rolling();
	void Init_Rage_Laser();
private:

	void Init_Attack_Punch();

	// 주먹이 범위 안에 들어왔을 때
	void InRange();

	// 한번만 Range 호출해서 그 순간 플레이어 위치 받아오기
	_int Set_RangePos();

	// Range랑 주먹이랑 충돌했나 안했나 bool값 반환 함수
	_bool IsCollision();



private:
	// 태우형 꺼
	HRESULT Init_Attack_Missile();

	HRESULT SetUp_UI();
	void Setting_HpUi();

private:
	void State_Machine(_float fTimeDelta);
	void Idle(_float fTimeDelta);
	void Move(_float fTimeDelta);
	void Phase(_float fTimeDelta);
	void Attack(_float fTimeDelta);
	void Rage(_float fTimeDelta);
	void Grogy(_float fTimeDelta);
	void Die(_float fTimeDelta);

private:
	void Attack_Missile(_float fTimeDelta);
	void Attack_Punch(_float fTimeDelta);
	void Attack_Rolling(_float fTimeDelta);

private:
	void Rage_Laser(_float fTimeDelta);
	void Rage_Sunflower(_float fTimeDelta);

private:
	_float m_ShieldTimer = 0.f;

	_float m_fTimer = 0.f; 
	_uint m_ImageIndex = 0;
	_int m_InitHp = 100;
	_bool m_OnShield = false;
	_bool m_OnPattern = false;
	_bool m_SpawnShield = false;
	_bool m_bShieldON = true;

private:
	// 인우형 패턴 변수
	CAttackRange* m_pAttackRange = nullptr;
	CTransform* m_RangeTrans = nullptr;

	_uint				m_TotalPunchCount = 4;
	_uint				m_CurrPunchCount = 0;
	_bool				m_bRangeValid = false;
	_bool				m_bCalled = false;
	_bool				m_Hand = false;
	_bool				m_Shaking[2] = { false , false};


private:
	// 태우형 패턴 변수
	_bool  m_bMissile = false;
	_float m_fMissileCount = 0.f;
	_int m_fFireCount = 0;
	_float m_fFireFrame = 0.f;
	_float m_fWaiting = 0.f;
	_bool    m_bLeft = false;
	_bool    m_bRight = false;
	_bool    m_btargetCollider = true;
	CMissile* m_pMissile = nullptr;
	CTargeting* m_pTargeting_Main = nullptr;
	CTargeting* m_pTargeting_Sub = nullptr;

	ARM m_CurrLaunchArm = ARM_LEFT;
	_float m_LaunchTimer = 0.f;
	_float3 m_InitLaunchPos[2];

	//UI
	_int                m_uChangeHp = 0;
	_int                m_beforeHp = 0;
	_float               m_fHpbarPos = 0.f;

	_int                m_uChangeShieldHp = 0;
	_int                m_beforeShieldHP = 0;
	_int                m_ShieldHpPos = 0;


	CUI* m_BossUI_HP = nullptr;
	CUI* m_BossUI_Shield = nullptr;
	CUI* m_BossUI_Black = nullptr;
	CUI* m_BossUI_HpBar = nullptr;
	CUI* m_BossUI_ShieldHP = nullptr;

	_float m_fBossMaxHp;
	_float m_fMaxShield;
private:
	// 내꺼
	_float3 m_vScale = _float3(5.f,5.f,5.f);

	_bool m_init = false;

	_float m_AttPatternTimer = 0.f;

	_float4 m_Color = _float4(0.f,0.f,0.f,0.f);
	_float4 m_HitColor = _float4(1.f, 1.f, 1.f, 0.5f);
	_float4 m_AmorColor = _float4(0.5f, 0.5f, 0.f, 0.5f);

	_float4 m_InitColor = _float4(0.f, 0.f, 0.f, 0.f);


	// Idle
	_bool initPos[2] = { false,false };
	_bool idlePos[2] = { true,true };

	_float m_LeftTimer = 0.f;
	_float m_RightTimer = 0.f;

	_float3  leftArmBazier[3];
	_float3  rightArmBazier[3];

	_float m_fUpMidPos = 20.f;
	_float m_fRightMidPos = 30.f;

	_float m_fUpPos = 1.f;
	_float m_fRightPos = 10.f;

	// Move
	_float m_fMoveLength = 20.f;


	// Init Positioning

	_bool m_Resizing = false;
	_bool m_Sizing = false;
	_bool m_Reset = false;

	_float m_TurnTime = 2.f;

	_float3 m_InitPos = _float3(0.f,0.f,0.f);
	_float3 m_SizingAxis = _float3(1.f, 1.f, 0.f);
	

	// Attack_Rolling
	_bool m_RollingReady = false;
	_bool m_RollingShake[3] = {false,false,false};
	_float	m_RollingChargingTime = 3.f;
	_float m_RollingChargingGauge = 0.f;
	_bool m_RollingCharged = false;
	_bool m_MoveToPlayer = false;
	_float	m_RollingMovingTime = 5.f;
	_uint	m_RollingHitCount = 0;
	_bool m_RollingHit[2] = { false,false };


	// Rage_Laser
	_bool m_Strike[2] = { false, false };
	_bool m_Aiming = false;
	_bool m_ReachPoint[2] = { false,false };
	_float3 m_TargetPos;
	_float m_AimTime = 2.0f;
	_float m_ShootTime = 0.2f;
	_float m_ShootTimer = 0.f;
	_float m_StrikeTimer = 0.f;
	_float m_StrikeTime = 1.f;
	_bool m_Striking = false;

	_float m_LaserTime = 30.f;


	// Rage_Sunflower


	//Grogy
	_bool m_Grogy = false;

	// Die
	_bool m_Dying = false;




private:
	BOSSPHASE m_Phase = BOSS_PHASEONE;
	BOSSSTATE m_State = BOSS_IDLE;

	BOSSATTACK m_AttState = BOSSATT_PUNCH;
	BOSSRAGE m_RageState = BOSSRAGE_SUNFLOWER;
private:
	CPlayer* m_pPlayer = nullptr;
	CTransform* m_pPlayerTr = nullptr;

	CArm* m_LeftArm = nullptr;
	CArm* m_RightArm = nullptr;

	CTransform* m_LeftArmTr = nullptr;
	CTransform* m_RightArmTr = nullptr;

	CTransform* m_LeftArmRotationTr = nullptr;
	CTransform* m_RightArmRotationTr = nullptr;

	CShield* m_Shield = nullptr;

	list<CShield_Effect*> m_shield_effects_;

private:
	CTransform* m_pOnlyRotation = nullptr;
	CTransform* m_pTransform = nullptr;
	CRenderer* m_pRenderer = nullptr;
	CTexture* m_pTexture = nullptr;
	CBoxCollider* m_pCollider = nullptr;
	CVIBuffer_Cube* m_pBuffer = nullptr;
	CShader* m_pShader = nullptr;

public:
	static CBoss* Create(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};

END

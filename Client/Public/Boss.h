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

class CBoss :
    public CEnemy
{
protected:
	explicit CBoss(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	explicit CBoss(const CBoss& rhs);
	virtual ~CBoss() = default;

public:
	typedef enum tagBossState {BOSS_IDLE, BOSS_ATTACK,BOSS_PHASECHANGE, BOSS_DIE}BOSSSTATE;
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

private:
	void Synchronize_Transform();

private:
	// 내꺼
	void Init_Idle();
	void Reset_Arm_Direction(ARM _arm);
	_bool InitArmPosition(_float fTimeDelta);
	_bool Move_By_Bazier(ARM _arm, _float fTimeDelta);
	void Set_ArmPos(ARM _arm, _float3 _start, _float3 _mid, _float3 _end);
	void Blowing(_float fTimeDelta);


private:
	void State_Machine(_float fTimeDelta);
	void Idle(_float fTimeDelta);
	void Phase(_float fTimeDelta);
	void Attack(_float fTimeDelta);
	void Die(_float fTimeDelta);

private:
	void Attack_Missile(_float fTimeDelta);
	void Attack_Punch(_float fTimeDelta);
	void Attack_Mixed(_float fTimeDelta);

private:
	_float m_fTimer = 0.f; 

private:
	// 인우형 패턴 변수


private:
	// 태우형 패턴 변수


private:
	// 내꺼
	_bool m_test = false;

	//Init Idle
	_bool initPos = false;
	_bool idlePos = true;

	_float m_LeftTimer = 0.f;
	_float m_RightTimer = 0.f;

	_float3  leftArmBazier[3];
	_float3  rightArmBazier[3];



private:
	BOSSPHASE m_Phase = BOSS_PHASEONE;
	BOSSSTATE m_State = BOSS_IDLE;

	BOSSATTACK m_AttState;

private:
	CPlayer* m_pPlayer = nullptr;
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

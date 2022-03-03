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
	typedef enum tagBossPhase {BOSS_PHASEONE, BOSS_PHASETWO}BOSSPHASE;
	typedef enum tagBossState {BOSS_IDLE, BOSS_ATTACK, BOSS_DIE}BOSSSTATE;
	typedef enum tagBossAttack { BOSSATT_MISSILE, BOSSATT_PUNCH, BOSSATT_MIXED }BOSSATTACK;

public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;
	HRESULT SetUp_Component() override;

private:
	void InitArmPosition(_float fTimeDelta);

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

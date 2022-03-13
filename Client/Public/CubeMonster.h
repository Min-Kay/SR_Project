#pragma once
#include "Enemy.h"

BEGIN(Engine)
class CEffect;
class CTransform;
class CTexture;
class CRenderer;
class CVIBuffer_Cube;
class CBoxCollider;
class CShader;
END

BEGIN(Client)
class CPlayer;

class CCubeMonster :
    public CEnemy
{
protected:
	explicit CCubeMonster(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	explicit CCubeMonster(const CCubeMonster& rhs);
	virtual ~CCubeMonster() = default;

public:
	enum STATE {STATE_IDLE, STATE_SEARCH, STATE_ALERT, STATE_CHASE, STATE_ATTACK, STATE_DIE};
	enum MONSTERTYPE {MT_SEARCH, MT_IDLE};

public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;

	HRESULT SetUp_Component() override;

private:
	void State_Machine(_float fTimeDelta);

	void Idle(_float fTimeDelta);
	void Search_Player(_float fTimeDelta);

	void Alert_Company(_float fTimeDelta);
	void Chase_Player(_float fTimeDelta);
	void Attack(_float fTimeDelta);


	void Move(_float fTimeDelta);
	void Blow(_float fTimeDelta);

	const STATE& Get_MonsterState() const;
	void Set_MonsterState(STATE _state);

	void Impact();
	_bool Sizing(_float fTimeDelta);
public:
	void Target_Turn(_float3 dir, _float fTimeDelta);
	virtual void Add_HP(_int _add) override;
private:
	void Dying(_float fTimeDelta);

public:
	void Set_InitPos(_float3 _pos);
	void Set_MonsterType(MONSTERTYPE _type);
private:
	void Charging(_float fTimeDelta);
	void Firing(_float fTimeDelta);
	void Rebounding(_float fTimeDelta);


private:
	STATE m_State = STATE_IDLE;
	MONSTERTYPE m_MonsterType = MT_SEARCH;
	CPlayer* m_Player = nullptr;
	CTransform* m_PlayerPos = nullptr;

private:
	_float m_SearchRange = 5.f;
	_float m_AlertRange = 8.f;

	_float m_CurrAlertRange= 0.f;
	_float m_AttackRange = 7.f;

	_float m_AlertTime = 3.f;

	_float m_Timer = 0.f;

	_float m_IdleSpeed = 0.1f;
	_float m_ChaseSpeed = 0.2f;

	_float m_AttackSpeed = 1.f;

	_float m_Angle = 0.f;

	_bool	m_YMove = false;
	_bool	m_RMove = false;


	_float	m_fTimer = 0.f;

	_bool m_Sizing = false;


private:
	_bool m_isCharging = false;
	_float3 m_vChargingLook;
	_float3 m_vChargingUp;
	_float m_ChargingTimer = 0.f;
	_bool isBound = false;


	_bool m_isFiring = false;
	_bool m_Rebounding = false;

	_bool m_Rebounded = false;

private:
	_float3 m_InitPoint;
	_float3 m_MovePoint;

private:
	_bool	m_Alert = false;

private:
	CEffect* m_Effect = nullptr;
	_float4 m_Color;
protected:
	CTexture* m_pTexture = nullptr;
	CTransform* m_pTransform = nullptr;
	CVIBuffer_Cube* m_pVIBuffer = nullptr;
	CRenderer* m_pRenderer = nullptr;
	CBoxCollider* m_pBoxCollider = nullptr;
	CShader* m_pShader = nullptr;

public:
	static CCubeMonster* Create(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};

END


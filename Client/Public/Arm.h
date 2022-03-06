#pragma once
#include "Enemy.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CBoxCollider;
class CRenderer;
class CVIBuffer_Cube; 
END

BEGIN(Client)
class CBoss;
class CPlayer;

class CArm :
	public CEnemy
{
protected:
	explicit CArm(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	explicit CArm(const CArm& rhs);
	virtual ~CArm() = default;

public:
	typedef enum tagArmPos {ARMPOS_LEFT, ARMPOS_RIGHT}ARMPOS;
	typedef enum tagArmState { ARM_IDLE, ARM_ATTACK }ARMSTATE;

public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;
	HRESULT SetUp_Component() override;

public:
	void Set_ArmPos(ARMPOS _armPos) { m_ArmPos = _armPos; }
	void Set_Position(_float3 _pos);
	void Add_Position(_float3 _add);
	void Move_Dir(_float3 dir, _float range);

public:
	void Set_State(ARMSTATE _state) { m_State = _state; }
	const ARMSTATE Get_State() const { return m_State; }
	void Set_Parent(CBoss* parent);
	void Set_Player(CPlayer* _player);

	void Synchronize_Direction();
	void Set_Rolling(_bool _bool, _float3 axis = _float3(0.f,1.f,0.f));
	void Set_RollingSpeed(_float _speed);

	const _bool& Get_OnCollide() const;

	const _bool& Get_ParentCollide() const;
	void Set_ParentCollide(_bool _bool);
private:
	void Synchronize_Transform();
	void Rolling(_float fTimeDelta);

private:
	void Mode(_float fTimeDelta);

private:
	ARMPOS m_ArmPos = ARMPOS_RIGHT;
	ARMSTATE m_State = ARM_IDLE;
	CBoss* m_Parent = nullptr;
	CTransform* m_ParentTr = nullptr;

	CPlayer* m_Player = nullptr;

private:
	_float m_Timer = 0.f;
	_float m_AttackTick = 1.f;
	_bool m_ParentCollide = false;
private:
	_bool m_Rolling = false;
	_float m_RollingSpeed = 0.3f;
	_float3 m_RollingAxis = _float3(0.f, 1.f, 0.f);

private:
	CTransform* m_pOnlyRotation = nullptr;
	CTransform* m_pTransform = nullptr;
	CRenderer* m_pRenderer = nullptr;
	CTexture* m_pTexture = nullptr;
	CBoxCollider* m_Collider = nullptr;
	CVIBuffer_Cube* m_pBuffer = nullptr;

public:
	static CArm* Create(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};
END

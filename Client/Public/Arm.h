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
	typedef enum tagArmState { ARM_IDLE, ARM_ATTACK }ARMSTATE;

public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;
	HRESULT SetUp_Component() override;

public:
	void Set_Position(_float3 _pos);
	void Add_Position(_float3 _add);
	void Move_Dir(_float3 dir, _float range);

public:
	void Set_State(ARMSTATE _state) { m_State = _state; }
	const ARMSTATE Get_State() const { return m_State; }
	void Set_Parent(CBoss* parent);
	void Set_Player(CPlayer* _player);

private:
	void Synchronize_Transform();


private:
	void Mode(_float fTimeDelta);

private:
	ARMSTATE m_State = ARM_IDLE;
	CBoss* m_Parent = nullptr;
	CPlayer* m_Player = nullptr;

private:
	_float m_Timer = 0.f;
	_float m_AttackTick = 1.f;
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

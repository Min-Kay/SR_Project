#pragma once
#include "Client_Defines.h"
#include "Enemy.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CVIBuffer_Cube;
class CTexture;
class CBoxCollider;
class CShader;
END

BEGIN(Client)
class CPlayer;
class CBoss;

class CMinimy :
    public CEnemy
{
protected:
	explicit CMinimy(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	explicit CMinimy(const CMinimy& rhs);
	virtual ~CMinimy()  = default;

public:
	typedef enum tagMinimyState { MINIMY_IDLE, MINIMY_MOVE, MINIMY_ATTACK, MINIMY_DIE}MINIMYSTATE;
	typedef enum tagMinimyAttack {MATT_SCREW, MATT_COMPANYFIRE}MATT;
public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;
	HRESULT SetUp_Component() override;

private:
	void Gravity_Blowing(_float fTimeDelta);
	void Impact();

public:
	void Add_HP(_int _add) override;
	void Set_Player(CPlayer* _player);
	void Set_Boss(CBoss* _boss);

private:
	
private:
	void State_Machine(_float fTimeDelta);
	_bool Sizing(_float fTimeDelta);

	void Target_Turn(_float3 dir, _float fTimeDelta);
	void Set_InitPos(_float3 _pos);
private:
	void Idle(_float fTimeDelta);
	void Move(_float fTimeDelta);
	void Die(_float fTimeDelta);
	
private:
	CTransform* m_pTransform = nullptr;
	CRenderer* m_pRenderer = nullptr;
	CTexture* m_pTexture = nullptr;
	CBoxCollider* m_pCollider = nullptr;
	CVIBuffer_Cube* m_pBuffer = nullptr;
	CShader* m_pShader = nullptr;

private:
	_bool m_Sizing = false;
	_float3 m_Size = _float3(0.8f,0.8f,0.8f);


private:
	_bool	m_YMove = false;
	_bool	m_RMove = false;
	_float m_Timer = 0.f;
	_float m_SearchRange = 5.f;
	_float m_IdleSpeed = 0.1f;
	_float m_Angle = 0.f;
private:
	_float3 m_InitPoint;
	_float3 m_MovePoint;

private:
	CPlayer* m_pPlayer = nullptr;
	CBoss* m_pBoss = nullptr;

	CTransform* m_pPlayerTr = nullptr;
	CTransform* m_pBossTr = nullptr;

private:
	MINIMYSTATE m_State = MINIMY_IDLE;
	MATT m_AttState = MATT_SCREW;

	_float m_fTimer = 0.f;
	_float m_fForce = 0.f;
	_float4 m_Color = _float4(0.f,0.f,0.f,0.f);
public:
	static CMinimy* Create(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};
END

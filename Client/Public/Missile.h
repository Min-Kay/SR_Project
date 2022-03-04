#pragma once
#include "Client_Defines.h"
#include "GameObject.h"


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
class CTageting;
class CMissile :
	public CGameObject
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

private:
	HRESULT SetUp_Component();
	_float3		TestTturning(_float3 P0, _float3 P1, _float3 P2, _float3 P3, _float time);
private:
	CVIBuffer_Cube* m_pVIBufferCom = nullptr;

	CRenderer*		m_pRendererCom = nullptr;

	CTexture*		m_pTextureCom = nullptr;

	CBoxCollider*	m_pBoxCollider = nullptr;

	CTransform*		m_pTransformCom = nullptr;


private:
	CArm*		m_pArm = nullptr;
	CPlayer*	m_pPlayer = nullptr;
	CTageting*	m_pTageting = nullptr;

	_float3		PlayerPos;
	_float3		MissilePos;
	_float3		m_fvecdir;
	_int m_Damage = 5;
	_float m_Speed = 1.f;
	_float m_Timer = 0.f;
	_float3 RandPos1;
	_float3 RandPos2;

	_float timer = 0.f;
	_float LerpTimeer;
	_float3 testPos;
	_float m_ftimer;

	_uint CreateCount = 0;
	_bool	m_bDEAD = false;
public:
	static CMissile* Create(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};
END

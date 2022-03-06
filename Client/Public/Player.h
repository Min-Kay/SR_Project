#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CCamera;
class CBoxCollider;
END

BEGIN(Client)
class CCamera_Player;
class CPortalControl;
class CGun;

class CPlayer final : public CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Set_Cam(CCamera* cam);

public:
	void Tick_JumpState(_float fTimeDelta);
	void Erase_Portal();
	HRESULT Reset_PlayerPos(_float3 resetPos);
public:
	const _int& Get_Hp() const;
	void Set_Hp(_int _hp);
	void Add_Hp(_int _add);
	typedef struct tagpalyerinfo
	{
		_float3    Pos;
		_int		Hp;

	}PLAYERINFO;
	PLAYERINFO Get_Info() { return m_Info; }

private:

	/* 모델 */
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

	/* 상태 (위치, 크기, 회전) */
	CTransform*			m_pTransformCom = nullptr;

	/* 그려진다. */
	CRenderer*			m_pRendererCom = nullptr;

	CCamera*			m_Camera = nullptr;

	/* 충돌정보*/
	CBoxCollider* m_pBoxColliderCom = nullptr;

private:
	_bool				m_bJump = false;
	_float				m_fMaxForce = 30.f;
	_float				m_fForce = 2.f;
	_float				m_CurrForce = 0.f;

private:
	_bool				m_OnGround = false;

private:
	_uint				m_iCurrIndex = 0; 
	CPortalControl*		m_pPortalCtrl = nullptr;
	CGun*				m_pGun = nullptr;


private:
	_int				m_HP = 100;

	PLAYERINFO			m_Info;

private:
	HRESULT SetUp_Components();

	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

	HRESULT SetUp_Weapons();

public:
	CCamera* Get_Camera();
private:
	_int Player_Control(_float fTimeDelta);
	HRESULT Synchronize_Camera();
	void Check_OnGround();

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
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


private:
	/* 텍스쳐 */
	CTexture*			m_pTextureCom = nullptr;

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
	_float				m_fFrame = 0.f;
	_bool				m_bJump = false;
	_float				m_CurrJumpForce = 0.f;
	_float				m_fJumpForce = 3.f;
	_float				m_fMaxJumpForce = 100.f;
	_bool				m_OnGround = false;

private:
	_uint				m_iCurrIndex = 0; 
	CPortalControl*		m_pPortalCtrl = nullptr;
	CGun*				m_pGun = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_OnTerrain();
	HRESULT Check_Terrain();

	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
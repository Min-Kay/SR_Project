#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CBoxCollider;
class CShader;
END

BEGIN(Client)
class CPlayer;
class CTileCollider;
class CBall final : public CGameObject
{
private:
	explicit CBall(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBall(const CBall& rhs);
	virtual ~CBall() = default;

public:

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	HRESULT FaceOn_Camera();
public:
	HRESULT Move(_float fTimeDelta);
	_bool	Get_BallLive() { return m_bBallLive; }
private:
	/* 텍스쳐 */
	CTexture* m_pTextureCom = nullptr;

	/* 모델 */
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;

	/* 상태 (위치, 크기, 회전) */
	CTransform* m_pTransformCom = nullptr;
	CTransform* m_pTransformCam = nullptr;
	/* 그려진다. */
	CRenderer* m_pRendererCom = nullptr;

	/* 충돌체 */
	CBoxCollider* m_pBoxColliderCom = nullptr;

	CShader* m_pShader = nullptr;

	_float				m_fFrame = 0.f;
	_float				m_iCount = 0.f;
private:
	HRESULT SetUp_Components();


	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

	CTransform* m_pTarget = nullptr;
	CPlayer* m_Player = nullptr;
	CTileCollider* m_pCollider = nullptr;
	_bool		m_bBallLive = true;
public:
	static CBall* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
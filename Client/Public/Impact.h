#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CVIBuffer_Color;
class CBoxCollider;
END

BEGIN(Client)

class CImpact final : public CGameObject
{
private:
	explicit CImpact(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CImpact(const CImpact& rhs);
	virtual ~CImpact() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	/* 텍스쳐 */
	CTexture*			m_pTextureCom = nullptr;

	/* 모델 */
	CVIBuffer_Color*		m_pVIBufferCom = nullptr;

	/* 상태 (위치, 크기, 회전) */
	CTransform*			m_pTransformCom = nullptr;

	/* 그려진다. */
	CRenderer*			m_pRendererCom = nullptr;

	/* 충돌정보*/
	CBoxCollider*      m_pBoxColliderCom = nullptr;


private:
	_float				m_fFrame = 0.f;
	DWORD			m_lColor ;
	CTransform*		m_pTarget = nullptr;
	_float3			m_fLook;
private:
	HRESULT SetUp_Components();

	_float3 testGravity;
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
public:
	static CImpact* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
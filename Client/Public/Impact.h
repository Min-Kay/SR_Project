#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CShader;
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

public:
	typedef struct tagImpact
	{
		_float3		Position;
		_float		SpreadSpeed;
		_float		RandomDirection;
		_float4		Color;
		_bool		Change;
		_float4		EndColor;
		_float		DeleteTime;
		_float3		Size;
	}IMPACT;
private:
	/* 모델 */
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

	/* 상태 (위치, 크기, 회전) */
	CTransform*			m_pTransformCom = nullptr;

	/* 그려진다. */
	CRenderer*			m_pRendererCom = nullptr;

	CShader*			 m_pShader = nullptr;

	CTexture*		m_pTexture = nullptr;

private:
	IMPACT			m_Impact;
	_float			m_fTimer =0.f;
	_float4			m_CurrColor;
	_float3			m_vDir;
	CTransform*		m_pTarget = nullptr;
	//_float3			m_size;
private:
	HRESULT SetUp_Components();
	HRESULT FaceOn_Camera();

	
private:
	random_device rd;
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
public:
	static CImpact* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
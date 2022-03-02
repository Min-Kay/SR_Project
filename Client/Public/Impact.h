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

	_bool Get_Delete() { return m_Impact.DeleteImpact; }

public:
	typedef	enum Colorgradation
	{
		GRADATION_NONE,GRADATION_UP, GRADATION_DOWN, GRADATION_FLASH, GRADATION_END
	}GRADATION;
	typedef struct tagImpact
	{
		_float3    Pos;
		_float		Speed;
		_double    randomPos;
		D3DCOLOR	Color;
		D3DCOLOR	ChangeColor;
		_uint		deleteCount;
		_bool		DeleteImpact;
		_float3		Size;
		GRADATION	Gradation;
	}IMPACT;
private:
	/* 모델 */
	CVIBuffer_Color*		m_pVIBufferCom = nullptr;

	/* 상태 (위치, 크기, 회전) */
	CTransform*			m_pTransformCom = nullptr;

	/* 그려진다. */
	CRenderer*			m_pRendererCom = nullptr;

	/* 충돌정보*/
	CBoxCollider* m_pBoxColliderCom = nullptr;

private:
	IMPACT			m_Impact;
	_float			m_fFrame = 0.f;
	_float			m_fcount =0.f;
	_float3			m_fvecdir;
	CTransform*		m_pTarget = nullptr;
	//_float3			m_size;
private:
	HRESULT SetUp_Components();
	HRESULT FaceOn_Camera();

	
private:
	_uint m_iCurrSpread = 0;
	_uint m_iSpread = 3;
	random_device rd;
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
	HRESULT Gradation_Pattern();
	_uint m_iCount;
public:
	static CImpact* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
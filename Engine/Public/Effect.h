#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;

class ENGINE_DLL CEffect :
    public CGameObject
{
private:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	/* 텍스쳐 */
	CTexture* m_pTextureCom = nullptr;

	/* 모델 */
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;

	/* 상태 (위치, 크기, 회전) */
	CTransform* m_pTransformCom = nullptr;

	/* 그려진다. */
	CRenderer* m_pRendererCom = nullptr;

private:
	_float				m_fFrame = 0.f;

private:
	virtual HRESULT SetUp_Components();
	virtual HRESULT SetUp_RenderState();
	virtual HRESULT Release_RenderState();

public:
	static CEffect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
   
};
END

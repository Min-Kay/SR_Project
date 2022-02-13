#pragma once
#include "Client_Defines.h"
#include "Camera.h"
BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CRenderer;
END

BEGIN(Client)
class CCamera_Sub :
    public CCamera
{
protected:
    explicit CCamera_Sub(LPDIRECT3DDEVICE9 pGraphic_Device);
    explicit CCamera_Sub(const CCamera_Sub& rhs);
    virtual ~CCamera_Sub() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_float fTimeDelta);
	virtual _int LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual HRESULT BeforeRender();
	virtual HRESULT AfterRender();

private:
	CTexture* m_pTextureCom = nullptr;
	CVIBuffer_Rect* m_pVIBuffer = nullptr;
	CRenderer* m_pRender = nullptr;
	CTransform* m_pRenderTransform = nullptr;
	
	LPDIRECT3DSURFACE9 m_pSurface = nullptr; 
	IDirect3DSurface9* pBackBuffer = nullptr;
public:
    static CCamera_Sub* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END


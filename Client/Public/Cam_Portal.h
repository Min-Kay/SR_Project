#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
class CRenderer;
class CVIBuffer_Portal;
class CTexture;
END

BEGIN(Client)
class CPortal;

class CCam_Portal :
    public CCamera
{

protected:
	explicit CCam_Portal(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCam_Portal(const CCam_Portal& rhs);
	virtual ~CCam_Portal() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_float fTimeDelta);
	virtual _int LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual HRESULT BeforeRender();
	virtual HRESULT AfterRender();

public:
	HRESULT Set_Cam_Angle(CTransform* portal, CTransform* target);

public:
	void Set_ExitPortal(CPortal* _exit = nullptr);

private:
	CTexture* m_pTextureCom = nullptr;
	CVIBuffer_Portal* m_pVIBuffer = nullptr;
	CRenderer* m_pRender = nullptr;
	CTransform* m_pRenderTransform = nullptr;

private:
	LPDIRECT3DSURFACE9 m_pSurface = nullptr;
	_uint m_TextureIndex = 0;
	IDirect3DSurface9* pBackBuffer = nullptr;
	
private:
	CCam_Portal* m_ExitPortal = nullptr;

public:
	static CCam_Portal* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

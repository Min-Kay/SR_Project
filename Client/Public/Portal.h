#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CTexture;
class CVIBuffer_Rect;
class CBoxCollider;
END

BEGIN(Client)
class CCam_Portal;

class CPortal :
    public CGameObject
{
protected:
	explicit CPortal(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPortal(const CPortal& rhs);
	virtual ~CPortal() = default;


public:
	typedef enum tagColor { PORTAL_ORANGE, PORTAL_BLUE, PORTAL_END }PORTALCOLOR;

public:
	typedef struct tagPortalDesc {
		_float3		vEye, vAt, vAxisY;
		_float3		vSubAxis;
		
		_uint iLevel;
		_uint iPortalColor;
		const _tchar* portalCam = nullptr;
	}PORTALDESC;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Link_Portal(CPortal* opponent = nullptr);
	CPortal* Get_Link_Portal();
public:
	CCam_Portal* Get_Cam_Portal();
	void Set_Cam_Angle(CTransform* target);

	void Release_Portal();
private:
	void Portaling();

private:
	CTransform* m_pTransform = nullptr;
	CRenderer* m_pRenderer = nullptr;
	CTexture* m_pTexture = nullptr;
	CVIBuffer_Rect* m_pVIBuffer = nullptr;

	CPortal* m_pOpponent = nullptr;
	CCam_Portal* m_pCam_Portal = nullptr;
	CBoxCollider* m_Collider = nullptr; 

	const _tchar* tag = nullptr;

	_bool Collide_Added = false;

public:
	static CPortal* Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END


#pragma once
#include "GameObject.h"
#include "UI.h"
#include "Portal.h"

BEGIN(Engine)
class CUI;
class CCamera;
END

BEGIN(Client)

class CPortalControl :
    public CGameObject
{
protected:
	explicit CPortalControl(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPortalControl(const CPortalControl& rhs);
	virtual ~CPortalControl() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Spawn_Portal(CPortal::PORTALCOLOR iIndex);
	HRESULT Erase_Portal();
	void Set_Camera(CCamera* _cam);

private:
	HRESULT SetUp_UI();
	HRESULT Synchronize_Camera_Angle();
	void	Animate_Gun(_float fTimeDelta);

	void	Spawn_Effect(_uint _index, _float3 _pos);

public:
	void Set_Vaild(_bool _bool);

private:
	CCamera* m_camera = nullptr;
	CPortal* m_pPortal_Orange = nullptr;
	CPortal* m_pPortal_Blue = nullptr;

	CUI* m_pPortal_Orange_UI = nullptr;
	CUI* m_pPortal_Blue_UI = nullptr;
	CUI* m_pPortal_Gun_UI = nullptr;

private:
	_bool m_Vaild = true;
	_float m_fGun_fx = 0.f;
	_float m_fGun_fy = 0.f;
	_float m_fFrWalk = 0.f;
	_float m_fFrShoot = 0.f;



public:
	static CPortalControl* Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END


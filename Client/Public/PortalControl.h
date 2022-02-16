#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)
class CPortal;
class CPortalControl :
    public CGameObject
{
protected:
	explicit CPortalControl(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPortalControl(const CPortalControl& rhs);
	virtual ~CPortalControl() = default;

public:
	enum PortalColor{ PORTAL_ORANGE,PORTAL_BLUE,PORTAL_END};

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Spawn_Portal(_uint iLevelIndex, CTransform* _tr, PortalColor iIndex);
	HRESULT Erase_Portal(_uint iLevelIndex);
	void Set_Player(CTransform* pPlayer);

private:
	CTransform* m_pPlayerTransform = nullptr;
	CPortal* m_pPortal_Orange = nullptr;
	CPortal* m_pPortal_Blue = nullptr;

public:
	static CPortalControl* Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END


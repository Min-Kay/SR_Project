#pragma once
#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)
class CPortalGunUI :
    public CUI
{
private:
	explicit CPortalGunUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPortalGunUI(const CPortalGunUI& rhs);
	virtual ~CPortalGunUI() = default; 

public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;
protected:
	HRESULT SetUp_Components(const _tchar* _texture) override;
public:
	HRESULT Tick_UI(_float fTimeDelta) override;
	HRESULT Set_RenderState() override;

public:
	static CPortalGunUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};
END


#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)
class CPortalControl;
class CCamera_Dynamic final : public CCamera
{
protected:
	explicit CCamera_Dynamic(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_Dynamic(const CCamera_Dynamic& rhs);
	virtual ~CCamera_Dynamic() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual HRESULT BeforeRender() override;
	virtual HRESULT AfterRender() override;

public:
	static CCamera_Dynamic* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END
#pragma once
#include "Client_Defines.h"
#include "Button.h"

BEGIN(Client)
class CButton_Continue :
    public CButton
{
protected:
	explicit CButton_Continue(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CButton_Continue(const CButton_Continue& rhs);
	virtual ~CButton_Continue() = default;
public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;

private:
	HRESULT SetUp_Components() override;

public:
	HRESULT Tick_UI(_float fTimeDelta) override;
	HRESULT Set_RenderState() override;

protected:
	HRESULT OnClick_Action() override;
	void OnEnter_Overlay() override;
	void OnEnter_UnOverlay() override;

public:
	static CButton_Continue* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};
END 

#pragma once
#include "Client_Defines.h"
#include "Button.h"

BEGIN(Client)
class CButton_Exit :
    public CButton
{
protected:
	explicit CButton_Exit(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CButton_Exit(const CButton_Exit& rhs);
	virtual ~CButton_Exit() = default;
public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;
protected:
	HRESULT SetUp_Components() override;
public:
	HRESULT Tick_UI(_float fTimeDelta) override;
	HRESULT Set_RenderState() override;
	
protected:
	HRESULT OnClick_Action() override;
	void OnEnter_Overlay() override;
	void OnEnter_UnOverlay() override;

public:
	static CButton_Exit* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};
END

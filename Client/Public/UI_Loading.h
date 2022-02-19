#pragma once
#include "UI.h"

BEGIN(Client)
class CUI_Loading :
    public CUI
{
private:
	explicit CUI_Loading(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_Loading(const CUI_Loading& rhs);
	virtual ~CUI_Loading() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual HRESULT SetUp_Components() override;

private:
	_float m_fFrame =	0.f;
public:
	static CUI_Loading* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

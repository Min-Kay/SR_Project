#pragma once
#include "UI.h"

BEGIN(Client)
class CUI_Logo :
    public CUI
{
protected:
	explicit CUI_Logo(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_Logo(const CUI_Logo& rhs);
	virtual ~CUI_Logo() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT SetUp_Components() override;

public:
	static CUI_Logo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

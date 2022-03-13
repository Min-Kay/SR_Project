#pragma once
#include "Client_Defines.h"
#include "Level.h"

BEGIN(Engine)
class CUI;
END

BEGIN(Client)
class CLevel_Ending :
    public CLevel
{
public:
	explicit CLevel_Ending(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	virtual ~CLevel_Ending() = default;


public:
	HRESULT NativeConstruct() override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;

private:
	HRESULT Set_Camera();
	HRESULT Set_UI();

private:
	CUI* m_Scroll = nullptr;
	_float m_Timer = 0.f;

public:
	static CLevel_Ending* Create(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	void Free() override;
};
END

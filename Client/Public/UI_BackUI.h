#pragma once
#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CButton;
END

BEGIN(Client)
class CCamera_Player;
class CUI_BackUI :
    public CUI
{
protected:
	explicit CUI_BackUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_BackUI(const CUI_BackUI& rhs);
	virtual ~CUI_BackUI() = default;
public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;

protected:
	HRESULT SetUp_Components(const _tchar* _texture) override;

public:
	void Open_Menu();
	void Off_Menu();
	void Continue_Game();
	void Exit_Game();

private:
	CButton* m_Continue = nullptr;
	CButton* m_Exit = nullptr;

private:
	CCamera_Player* cam = nullptr;

public:
	void Set_Cam(CCamera_Player* _cam);

public:
	HRESULT Tick_UI(_float fTimeDelta) override;
	HRESULT Set_RenderState() override;

public:
	static CUI_BackUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};
END

#pragma once
#include "UI.h"

BEGIN(Engine)
class ENGINE_DLL CButton abstract:
    public CUI
{
protected:
	explicit CButton(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CButton(const CButton& rhs);
	virtual ~CButton() = default;

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

protected:
	virtual HRESULT OnClick_Action()PURE;
	virtual void OnEnter_Overlay()PURE;
	virtual void OnEnter_UnOverlay()PURE;


private:
	HRESULT OnClick();
	_bool OnEnter();

public:
	void Set_HWND(HWND _hwnd) { m_Hwnd = _hwnd; }
	void Set_ParentUI(CUI * _parent) { m_Parent = _parent; Safe_AddRef(m_Parent);  }

protected:
	CUI* m_Parent = nullptr;
	bool isOverlayed = false;
private:
	RECT m_Rect;
	HWND m_Hwnd;

public:
	CGameObject* Clone(void* pArg) PURE;
	void Free() override;
};
END

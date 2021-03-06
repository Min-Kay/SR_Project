#pragma once
#include "Tool_Define.h"
#include "Camera.h"

BEGIN(Tool)

class CCamera_Dynamic final : public CCamera
{
protected:
	explicit CCamera_Dynamic(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_Dynamic(const CCamera_Dynamic& rhs);
	virtual ~CCamera_Dynamic() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_float fTimeDelta);
	virtual _int LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual HRESULT BeforeRender();
	virtual HRESULT AfterRender();

private:
		POINT m_MousePos;
		POINT m_HoldMousePos;
public:
	static CCamera_Dynamic* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
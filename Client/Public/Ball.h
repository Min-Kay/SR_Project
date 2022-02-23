#pragma once
#include "Client_Defines.h"
#include "Effect.h"
class CBall :
    public CEffect
{
protected:
	explicit CBall(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBall(const CBall& rhs);
	virtual ~CBall() = default; 

public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;


protected:
	HRESULT SetUp_Components() override;
	HRESULT SetUp_RenderState() override;
	HRESULT Release_RenderState() override;

public:
	static CBall* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};


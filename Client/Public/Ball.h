#pragma once
#include "GameObject.h"
class CBall :
    public CGameObject
{
protected:
	explicit CBall(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	explicit CBall(const CBall& rhs);
	virtual ~CBall() = default;

public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};


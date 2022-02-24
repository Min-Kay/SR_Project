#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
class CGun :
    public CGameObject
{
protected:
	explicit CGun(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CGun(const CGun& rhs);
	virtual ~CGun() = default; 

public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;


public:
	static CGun* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};


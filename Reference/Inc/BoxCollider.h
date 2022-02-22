#pragma once
#include "Collider.h"
class CBoxCollider :
    public CCollider
{
protected:
	explicit  CBoxCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit  CBoxCollider(const CBoxCollider& rhs);

	virtual ~CBoxCollider() = default;

public:
	HRESULT Update() override;

public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;

public:
	static CBoxCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CComponent* Clone(void* pArg) override;
	void Free() override;

};


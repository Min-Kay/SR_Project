#pragma once
#include "Collider.h"
class CSphereCollider :
    public CCollider
{
protected:
	explicit  CSphereCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit  CSphereCollider(const CSphereCollider& rhs);

	virtual ~CSphereCollider() = default;

public:
	virtual HRESULT Update();

public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;

public:
	static CSphereCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CComponent* Clone(void* pArg) override;
	void Free() override;
};


#pragma once
#include "Component.h"

BEGIN(Engine)
class CGameObject;
class CCollider :
    public CComponent
{
protected:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;


public:
	typedef enum tagType
	{
		COLLTYPE_ENTER,COLLTYPE_TRIGGER, COLLTYPE_END
	}COLLTYPE;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	HRESULT Set_Parent(CGameObject* _obj);
	const CGameObject* Get_Parent() const ;

public:
	HRESULT Set_Type(COLLTYPE _type);
	const COLLTYPE& Get_Type() const;

public:
	virtual HRESULT Update()PURE;

public:
	const list<_float3>& Get_IndexList() const;

protected:
	list<_float3> m_IndexList;
	CGameObject* m_Parent = nullptr;
	COLLTYPE m_type;

public:
	virtual CComponent* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};
END

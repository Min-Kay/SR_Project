#pragma once
#include "Component.h"

BEGIN(Engine)
class CGameObject;
class ENGINE_DLL CCollider :
    public CComponent
{
protected:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;


public:
	typedef enum tagCollType
	{
		COLLTYPE_ENTER,COLLTYPE_TRIGGER, COLLTYPE_END//실제 충돌 , 범위충돌(레이더)
	}COLLTYPE;

	typedef enum tagobjtype {
		COLLOBJTYPE_PLAYER, COLLOBJTYPE_OBJ, COLLOBJTYPE_MAP, COLLOBJTYPE_END
	}COLLOBJTYPE;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	HRESULT Set_Parent(CGameObject* _obj);
	CGameObject* Get_Parent();

public:
	HRESULT Set_CollType(COLLTYPE _type);
	const COLLTYPE& Get_CollType() const;

	HRESULT Set_ObjType(COLLOBJTYPE _type);
	const COLLOBJTYPE& Get_ObjType() const;
public:
	virtual HRESULT Update()PURE;


protected:

	CGameObject* m_Parent = nullptr;
	COLLTYPE m_CoLLtype;
	COLLOBJTYPE m_ObjType;

public:
	virtual CComponent* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};
END


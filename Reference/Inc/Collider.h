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
		COLLSTYLE_ENTER,COLLSTYLE_TRIGGER, COLLSTYLE_END//실제 충돌 , 범위충돌(레이더)
	}COLLSTYLE;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	HRESULT Set_Parent(CGameObject* _obj);
	CGameObject* Get_Parent();

public:
	HRESULT Set_CollStyle(COLLSTYLE _style);
	const COLLSTYLE& Get_CollStyle() const;

public:
	virtual HRESULT Update()PURE;


public:
	const _bool& Get_OnCollide() const;
	void Set_OnCollide(_bool _bool);

public:
	const _bool& Get_Dead() const { return m_Dead; }
	void Set_Dead(_bool _bool) { m_Dead = _bool; }

protected:
	_bool	m_Dead = false;
	CGameObject* m_Parent = nullptr;
	COLLSTYLE m_CollStyle;
	_bool	m_OnCollide = false;

public:
	virtual CComponent* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};
END


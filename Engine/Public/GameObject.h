#pragma once

#include "Base.h"

BEGIN(Engine)

class CComponent;
class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;
public:
	class CComponent* Get_Component(const _tchar* pComponentTag);

public:
	typedef enum tagObj{
		OBJ_NONE, OBJ_PLAYER,OBJ_UI,OBJ_ENEMY,OBJ_INTERACTION, OBJ_STATIC, OBJ_PORTAL, OBJ_END
	}OBJTYPE;

	typedef struct tagCulling
	{
		_float3 Position;
		_float	Radius;
	}OBJCULLING;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_float fTimeDelta);
	virtual _int LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent** ppOut, void* pArg = nullptr);

protected:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;

protected:
	CComponent*	Find_Component(const _tchar* pComponentTag);

protected:
	map<const _tchar*, CComponent*>			m_Components;
	typedef map<const _tchar*, CComponent*>	COMPONENTS;

public:
	HRESULT Compute_Distance(_float3 _obj, _float3 _target);
	const _float Get_Distance() const;

private:
	_float				m_fDis = 0.f;

public:
	void Set_Layer(_uint _layer);
	const _uint Get_Layer() const;

	void Set_Dead(_bool _bool) { m_Dead = _bool; }
	const _bool Get_Dead() const { return m_Dead; }

	void Set_Grab(_bool _bool) { m_Grab = _bool; }
	const _bool Get_Grab() const { return m_Grab; }


public:
	void Set_Type(OBJTYPE _type);
	const OBJTYPE& Get_Type() const;

public:
	OBJCULLING Get_CullingInfo(); 

private:
 	_uint				m_Layer = 0;
	OBJTYPE				m_Type;


private:
	_bool				m_Dead = false;
	_bool				m_Grab = false;
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END
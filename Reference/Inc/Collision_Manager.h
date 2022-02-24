#pragma once
#include "Base.h"

BEGIN(Engine)
class CCollider;
class CBoxCollider;
class CGameObject;

class CCollision_Manager :
	public CBase
{
	DECLARE_SINGLETON(CCollision_Manager)

protected:
	explicit CCollision_Manager();
	virtual ~CCollision_Manager() = default;

public:
	list<CGameObject*> Collision_All();
	HRESULT Collision_Box();
	
public:
	HRESULT Add_Collider(CBoxCollider* collider);
	_float3 Reflect_Direction();
private:
	_bool AABB(CBoxCollider* _MyCollider, CBoxCollider* _OtherCollider);

private:
	list<CBoxCollider*> m_CollList ;
	_float3 m_vPushDir;
	_bool	m_bAABB = false;
public:
	virtual void Free() override;
};
END



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

private:
	_bool AABB(CBoxCollider* _MyCollider, CBoxCollider* _OtherCollider);

private:
	list<CBoxCollider*> m_CollList ;
public:
	virtual void Free() override;
};
END



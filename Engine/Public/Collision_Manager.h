#pragma once
#include "Base.h"
#include "Collider.h"

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
	list<CGameObject*>* Get_Collision_List(CBoxCollider* target);
	list<CGameObject*>* Get_Ray_Collision_List(_float3 dir, _float3 pos, _float dis);
	HRESULT	Collision(CCollider::COLLOBJTYPE _first, CCollider::COLLOBJTYPE _second);
public:
	HRESULT Add_Collider(CCollider::COLLOBJTYPE _type, CBoxCollider* collider);
	HRESULT Release_Collider(CCollider::COLLOBJTYPE _type, CBoxCollider* collider);
	HRESULT Release_ColliderList();

private:
	_bool AABB(CBoxCollider* _MyCollider, CBoxCollider* _OtherCollider, _bool justReturn = true);
	
	_bool RayCollision(_float3 dir, _float3 pos, CBoxCollider* _OtherCollider, _float dis);
private:
	list<CBoxCollider*> m_CollList[CCollider::COLLOBJTYPE_END];

public:
	virtual void Free() override;
};
END



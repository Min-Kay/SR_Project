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
	typedef enum tagobjtype {
		COLLOBJTYPE_OBJ, COLLOBJTYPE_STATIC, COLLOBJTYPE_END
	}COLLOBJTYPE;

	typedef struct tagCollPoint
	{
		CGameObject* CollObj;
		_float3 Point;
		_float3 NormalVec;
	}COLLPOINT;

public:
	list<CGameObject*>* Get_Collision_List(CBoxCollider* target);
	list<COLLPOINT>* Get_Ray_Collision_List(_float3 dir, _float3 pos, _float dis);
	HRESULT	Collision(COLLOBJTYPE _first, COLLOBJTYPE _second);
public:
	HRESULT Add_Collider(COLLOBJTYPE _type, CBoxCollider* collider);
	HRESULT Release_Collider(COLLOBJTYPE _type, CBoxCollider* collider);
	HRESULT Release_ColliderList();

private:
	_bool AABB(CBoxCollider* _MyCollider, CBoxCollider* _OtherCollider, _bool justReturn = true);
	
	_bool RayCollision(_float3 dir, _float3 pos, CBoxCollider* _OtherCollider, _float dis, _float3& pPoint, _float3& pNor);
private:
	list<CBoxCollider*> m_CollList[COLLOBJTYPE_END];

public:
	virtual void Free() override;
};
END



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
	_bool Get_Collide(CBoxCollider* _from, CBoxCollider* _to);
	list<CGameObject*> Get_Collision_List(CBoxCollider* target);
	list<CGameObject*> Get_Collision_Object_List(CBoxCollider* target);

	list<COLLPOINT> Get_Ray_Collision_List(_float3 dir, _float3 pos, _float dis, _bool _sort = false);
	COLLPOINT* Get_Ray_Collision_Object(_float3 dir, _float3 pos, _float dis, _bool _sort = false);

	HRESULT Check_DeadCollider();
	HRESULT	Collision();
public:
	HRESULT Add_Collider(COLLOBJTYPE _type, CBoxCollider* collider);
	HRESULT Release_Collider(COLLOBJTYPE _type, CBoxCollider* collider);
	HRESULT Release_ColliderList();

private:
	_bool AABB(CBoxCollider* _MyCollider, CBoxCollider* _OtherCollider, _bool justReturn = true);
	
	_bool RayCollision(_float3 dir, _float3 pos, CBoxCollider* _OtherCollider, _float dis, _float3& pPoint, _float3& pNor);
private:
	list<CBoxCollider*> m_CollList[COLLOBJTYPE_END];

private:
	_float3 m_Dir[3] = { _float3(0.f,0.f,1.f), _float3(0.f,1.f,0.f), _float3(1.f,0.f,0.f) };
	int index[12][3] = { {1,0,5}, {4,5,0},{2,1,6},{5,6,1},{3,2,7},{6,7,2},{0,3,4},{7,4,3},{5,4,6},{7,6,4},{2,3,1},{0,1,3} };
public:
	virtual void Free() override;
};
END



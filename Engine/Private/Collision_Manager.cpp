#include "Collision_Manager.h"
#include "BoxCollider.h"
#include "SphereCollider.h"

IMPLEMENT_SINGLETON(CCollision_Manager)

CCollision_Manager::CCollision_Manager()
{
}

list<CGameObject*> CCollision_Manager::Collision_All()
{
	list<CGameObject*> objList;
	return objList;
}

list<CGameObject*> CCollision_Manager::Collision_Box()
{
	list<CGameObject*> objList;
	return objList;
}

list<CGameObject*> CCollision_Manager::Collision_Box_Sphere()
{
	list<CGameObject*> objList;
	return objList;
}

list<CGameObject*> CCollision_Manager::Collision_Sphere()
{
	list<CGameObject*> objList;
	return objList;
}

HRESULT CCollision_Manager::Add_Collider(COLLIDER _type,CCollider* collider)
{
	if (_type >= COLL_END)
		return E_FAIL;

	m_CollList[_type].push_back(collider);
	return S_OK;
}


void CCollision_Manager::Free()
{
	for(_uint i = 0; i < COLL_END; ++i)
	{
		m_CollList[i].clear();
	}
}

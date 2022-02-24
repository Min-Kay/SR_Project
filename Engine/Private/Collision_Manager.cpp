#include "Collision_Manager.h"
#include "BoxCollider.h"
#include "Collider.h"

IMPLEMENT_SINGLETON(CCollision_Manager)

CCollision_Manager::CCollision_Manager()
{
}

list<CGameObject*> CCollision_Manager::Collision_All()
{
	list<CGameObject*> objList;
	return objList;
}

HRESULT CCollision_Manager::Collision_Box()
{
	// 들어오는  collider의 max값과 min값을 찾아서 6축을 비교 world 상에서 비교를 해야겠지
	// object collider component를 각각 줄거고 그 component manager를 
	//collision box들을 불러와서 모든 collision을 비교?

	for (auto& pCollider : m_CollList)
	{
		//if (m_CollList)
		//	return E_FAIL;

		for (auto& pCollider2 : m_CollList)
		{
			if (pCollider == pCollider2)
				continue;
			if (FAILED(AABB(pCollider, pCollider2)))
			{
				MSGBOX("Failed to AABB in Collision Manager");
				return E_FAIL;
			}
			int a = 10;
		}
	}


	return S_OK;
}

//
//list<CGameObject*> CCollision_Manager::Collision_Box_Sphere()
//{
//	list<CGameObject*> objList;
//	return objList;
//}
//
//list<CGameObject*> CCollision_Manager::Collision_Sphere()
//{
//	list<CGameObject*> objList;
//	return objList;
//}

HRESULT CCollision_Manager::Add_Collider(CBoxCollider* collider)
{

	m_CollList.push_back(collider);
	return S_OK;
}

_bool CCollision_Manager::AABB(CBoxCollider* _MyCollider, CBoxCollider* _OtherCollider)
{
	_float3 vMyMax = (_MyCollider)->Get_State(CBoxCollider::COLLIDERINFO::COLL_MAX);
	_float3 vMyMin = (_MyCollider)->Get_State(CBoxCollider::COLLIDERINFO::COLL_MIN);

	_float3 vOtherMax = (_OtherCollider)->Get_State(CBoxCollider::COLLIDERINFO::COLL_MAX);
	_float3 vOtherMin = (_OtherCollider)->Get_State(CBoxCollider::COLLIDERINFO::COLL_MIN);


	if ((vMyMin.x <= vOtherMax.x && vMyMax.x >= vOtherMin.x) &&
		(vMyMin.y <= vOtherMax.y && vMyMax.y >= vOtherMin.y) &&
		(vMyMin.z <= vOtherMax.z && vMyMax.z >= vOtherMin.z))
	{

		
		return true;
	}
	else
	{
		//MSGBOX("Collider!!!!");
		return false;
	}
	

}


void CCollision_Manager::Free()
{
	
	for (auto& pBoxCollider : m_CollList)
		Safe_Release(pBoxCollider);

	m_CollList.clear();

}


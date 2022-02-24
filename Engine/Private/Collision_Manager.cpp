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
		if (vMyMin.x <= vOtherMax.x) // 왼쪽 충돌
		{
			_float3 vFirst = _float3(vOtherMin.x, vOtherMax.y, vOtherMax.z) - _float3(vOtherMin.x, vOtherMax.y, vOtherMin.z);
			_float3 vSecond = _float3(vOtherMin.x, vOtherMax.y, vOtherMax.z) - _float3(vOtherMin.x, vOtherMax.y, vOtherMin.z);
			D3DXVec3Cross(&m_vPushDir, &vFirst, &vSecond);
		}
		else if (vMyMax.x >= vOtherMin.x) // 오른쪽 충돌
		{
			_float3 vFirst = _float3(vOtherMax.x, vOtherMin.y, vOtherMin.z) - _float3(vOtherMax.x, vOtherMin.y, vOtherMax.z);
			_float3 vSecond = _float3(vOtherMax.x, vOtherMax.y, vOtherMax.z) - _float3(vOtherMax.x, vOtherMin.y, vOtherMax.z);
			D3DXVec3Cross(&m_vPushDir, &vFirst, &vSecond);
		}

		if (vMyMin.y <= vOtherMax.y) // 아랫면 충돌
		{
			_float3 vFirst = _float3(vOtherMin.x, vOtherMin.y, vOtherMax.z) - _float3(vOtherMin.x, vOtherMin.y, vOtherMin.z);
			_float3 vSecond = _float3(vOtherMax.x, vOtherMin.y, vOtherMax.z) - _float3(vOtherMin.x, vOtherMin.y, vOtherMax.z);
			D3DXVec3Cross(&m_vPushDir, &vFirst, &vSecond);
		}
		else if (vMyMax.y >= vOtherMin.y) // 윗면 충돌
		{
			_float3 vFirst = _float3(vOtherMax.x, vOtherMax.y, vOtherMax.z) - _float3(vOtherMax.x, vOtherMax.y, vOtherMin.z);
			_float3 vSecond = _float3(vOtherMax.x, vOtherMax.y, vOtherMax.z) - _float3(vOtherMin.x, vOtherMax.y, vOtherMax.z);
			D3DXVec3Cross(&m_vPushDir, &vFirst, &vSecond);
		}

		if (vMyMin.z <= vOtherMax.z) // 뒷면 충돌
		{
			_float3 vFirst = _float3(vOtherMin.x, vOtherMin.y, vOtherMax.z) - _float3(vOtherMax.x, vOtherMin.y, vOtherMax.z);
			_float3 vSecond = _float3(vOtherMin.x, vOtherMax.y, vOtherMax.z) - _float3(vOtherMin.x, vOtherMin.y, vOtherMax.z);
			D3DXVec3Cross(&m_vPushDir, &vFirst, &vSecond);
		}
		else if (vMyMax.z >= vOtherMin.z) // 앞면 충돌
		{
			_float3 vFirst = _float3(vOtherMax.x, vOtherMin.y, vOtherMax.z) - _float3(vOtherMax.x, vOtherMin.y, vOtherMin.z);
			_float3 vSecond = _float3(vOtherMin.x, vOtherMax.y, vOtherMin.z) - _float3(vOtherMax.x, vOtherMax.y, vOtherMin.z);
			D3DXVec3Cross(&m_vPushDir, &vFirst, &vSecond);
		}
		
		m_bAABB = true;
		return m_bAABB;
	}
	else
	{
		MSGBOX("Collider!!!!");
		m_bAABB = false;
		return m_bAABB;
	}
	

}

_float3 CCollision_Manager::Reflect_Direction()
{
	if (m_bAABB == true)
	{
		//m_vPushDir;
		D3DXVec3Normalize(&m_vPushDir, &m_vPushDir);

		_float radius = CBoxCollider::COLLIDERINFO::COLL_SIZE * 0.5;
		m_vPushDir *= radius;
		return m_vPushDir;
	}
	else
		return _float3{ 0.f, 0.f, 0.f };
}


void CCollision_Manager::Free()
{
	
	for (auto& pBoxCollider : m_CollList)
		Safe_Release(pBoxCollider);

	m_CollList.clear();

}


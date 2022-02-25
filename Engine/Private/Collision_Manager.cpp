#include "Collision_Manager.h"
#include "BoxCollider.h"
#include "Collider.h"

IMPLEMENT_SINGLETON(CCollision_Manager)

CCollision_Manager::CCollision_Manager()
{
}

list<CGameObject*>& CCollision_Manager::Get_Collision_List(CBoxCollider* target)
{
	list<CGameObject*> list;

	for (auto& collList : m_CollList)
	{
		for(auto& coll : collList)
		{
			if (coll == target)
				continue;

			if (AABB(target, coll, true))
			{
				list.push_back(coll->Get_Parent());
			}
		}
	}

	return list; 
}

list<CGameObject*>& CCollision_Manager::Get_Ray_Collision_List(_float3 dir, _float3 pos, _float& dis)
{
	list<CGameObject*> list;

	for (auto& collList : m_CollList)
	{
		for (auto& coll : collList)
		{
			if (RayCollision(dir, pos, coll, dis))
			{
				list.push_back(coll->Get_Parent());
			}
		}
	}

	return list;
}

HRESULT CCollision_Manager::Collision(CCollider::COLLOBJTYPE _first, CCollider::COLLOBJTYPE _second)
{
	for (auto& pCollider : m_CollList[_first])
	{
		for (auto& pCollider2 : m_CollList[_second])
		{
			if (pCollider2 == pCollider)
				continue;

			AABB(pCollider, pCollider2, false);
		}
	}
	return S_OK;
}

HRESULT CCollision_Manager::Add_Collider(CCollider::COLLOBJTYPE _type,  CBoxCollider* collider)
{
	auto iter = find_if(m_CollList[_type].begin(), m_CollList[_type].end(), [collider](CBoxCollider* a) {return a == collider; });

	if (iter != m_CollList[_type].end())
		return E_FAIL;

	Safe_AddRef(collider);
	m_CollList[_type].push_back(collider);
	return S_OK;
}

HRESULT CCollision_Manager::Release_Collider(CCollider::COLLOBJTYPE _type, CBoxCollider* collider)
{
	auto iter = find_if(m_CollList[_type].begin(), m_CollList[_type].end(), [collider](CBoxCollider* a) {return a == collider; });

	if (iter != m_CollList[_type].end())
		return E_FAIL;

	Safe_Release(collider);
	m_CollList[_type].erase(iter);
	return S_OK;

}

HRESULT CCollision_Manager::Release_ColliderList()
{
	for (auto& collList : m_CollList)
	{
		for (auto& pBoxCollider : collList)
			Safe_Release(pBoxCollider);
		collList.clear();
	}
	return S_OK; 
}

_bool CCollision_Manager::AABB(CBoxCollider* _MyCollider, CBoxCollider* _OtherCollider, _bool justReturn)
{
	_float3 vMyMax = (_MyCollider)->Get_State(CBoxCollider::COLLIDERINFO::COLL_MAX);
	_float3 vMyMin = (_MyCollider)->Get_State(CBoxCollider::COLLIDERINFO::COLL_MIN);

	_float3 vOtherMax = (_OtherCollider)->Get_State(CBoxCollider::COLLIDERINFO::COLL_MAX);
	_float3 vOtherMin = (_OtherCollider)->Get_State(CBoxCollider::COLLIDERINFO::COLL_MIN);
	//_float3 vOtherSize = (_OtherCollider)->Get_State(CBoxCollider::COLLIDERINFO::COLL_SIZE);

	if ((vMyMin.x <= vOtherMax.x && vMyMax.x >= vOtherMin.x) &&
		(vMyMin.y <= vOtherMax.y && vMyMax.y >= vOtherMin.y) &&
		(vMyMin.z <= vOtherMax.z && vMyMax.z >= vOtherMin.z))
	{
		if (justReturn)
			return true; 

		_float3 diff = vMyMax - vOtherMin;

		_float3 vMyCenter = (_MyCollider)->Get_State(CBoxCollider::COLLIDERINFO::COLL_CENTER);
		_float3 vOtherCenter = (_OtherCollider)->Get_State(CBoxCollider::COLLIDERINFO::COLL_CENTER);

		_float3 vMyCenterDir =  vOtherCenter - vMyCenter;
		_float3 vOtherCenterDir = vMyCenter - vOtherCenter;

		D3DXVec3Normalize(&vMyCenterDir,&vMyCenterDir);
		D3DXVec3Normalize(&vOtherCenterDir, &vOtherCenterDir);

		_float3 vMyPoint[8], vOtherPoint[8];

		int index[12][3] = { {0,1,2}, {0,2,3},{0,1,5},{0,4,5},{1,3,6},{1,5,6},{2,3,7},{2,6,7},{3,0,4},{3,7,4},{4,5,6},{4,7,6} };

		vMyPoint[0] = vMyMin;
		vMyPoint[1] = _float3(vMyMax.x, vMyMin.y, vMyMin.z);
		vMyPoint[2] = _float3(vMyMax.x, vMyMin.y, vMyMax.z);
		vMyPoint[3] = _float3(vMyMin.x, vMyMin.y, vMyMax.z);
		vMyPoint[4] = _float3(vMyMin.x, vMyMax.y, vMyMin.z);
		vMyPoint[5] = _float3(vMyMax.x, vMyMax.y, vMyMin.z);
		vMyPoint[6] = vMyMax;
		vMyPoint[7] = _float3(vMyMin.x, vMyMax.y, vMyMax.z);

		vOtherPoint[0] = vOtherMin;
		vOtherPoint[1] = _float3(vOtherMax.x, vOtherMin.y, vOtherMin.z);
		vOtherPoint[2] = _float3(vOtherMax.x, vOtherMin.y, vOtherMax.z);
		vOtherPoint[3] = _float3(vOtherMin.x, vOtherMin.y, vOtherMax.z);
		vOtherPoint[4] = _float3(vOtherMin.x, vOtherMax.y, vOtherMin.z);
		vOtherPoint[5] = _float3(vOtherMax.x, vOtherMax.y, vOtherMin.z);
		vOtherPoint[6] = vOtherMax;
		vOtherPoint[7] = _float3(vOtherMin.x, vOtherMax.y, vOtherMax.z);

		_float u, v, myDis, otherDis;

		for(auto& i : index)
		{
			D3DXIntersectTri(&vMyPoint[i[0]], &vMyPoint[i[1]], &vMyPoint[i[2]], &vMyCenter, &vMyCenterDir, &u, &v, &myDis);
			D3DXIntersectTri(&vOtherPoint[i[0]], &vOtherPoint[i[1]], &vOtherPoint[i[2]], &vOtherCenter, &vOtherCenterDir, &u, &v, &otherDis);
		}

		_float3 myCollPos = vMyCenterDir * myDis;
		_float3 otherCollPos = vOtherCenterDir * otherDis;

		_float3 myCollPoint = myCollPos + vMyCenter;
		_float3 otherCollPoint = otherCollPos + vOtherCenter;

		_float3 resultDir= otherCollPoint - myCollPoint;


		if (diff.x < diff.y && diff.z < diff.y)
			_MyCollider->Reflect_Direction(_float3(0.f, resultDir.y, 0.f));

		else if (diff.y < diff.x && diff.z < diff.x)
			_MyCollider->Reflect_Direction(_float3(resultDir.x, 0.f, 0.f));

		else if (diff.x < diff.z && diff.y < diff.z)
			_MyCollider->Reflect_Direction(_float3(0.f, 0.f, resultDir.z ));


		return true;
	}

	return false;

	

}

_bool CCollision_Manager::RayCollision(_float3 dir, _float3 pos, CBoxCollider* _OtherCollider, _float& dis)
{
	_float3 vOtherPoint[8];

	_float3 vOtherMax = (_OtherCollider)->Get_State(CBoxCollider::COLLIDERINFO::COLL_MAX);
	_float3 vOtherMin = (_OtherCollider)->Get_State(CBoxCollider::COLLIDERINFO::COLL_MIN);

	int index[12][3] = { {0,1,2}, {0,2,3},{0,1,5},{0,4,5},{1,3,6},{1,5,6},{2,3,7},{2,6,7},{3,0,4},{3,7,4},{4,5,6},{4,7,6} };

	vOtherPoint[0] = vOtherMin;
	vOtherPoint[1] = _float3(vOtherMax.x, vOtherMin.y, vOtherMin.z);
	vOtherPoint[2] = _float3(vOtherMax.x, vOtherMin.y, vOtherMax.z);
	vOtherPoint[3] = _float3(vOtherMin.x, vOtherMin.y, vOtherMax.z);
	vOtherPoint[4] = _float3(vOtherMin.x, vOtherMax.y, vOtherMin.z);
	vOtherPoint[5] = _float3(vOtherMax.x, vOtherMax.y, vOtherMin.z);
	vOtherPoint[6] = vOtherMax;
	vOtherPoint[7] = _float3(vOtherMin.x, vOtherMax.y, vOtherMax.z);

	dis = 0.f;
	_float u, v, otherDis;

	for (auto& i : index)
	{
		if (D3DXIntersectTri(&vOtherPoint[i[0]], &vOtherPoint[i[1]], &vOtherPoint[i[2]], &pos, &dir, &u, &v, &otherDis))
		{
			if(dis == 0.f || dis > otherDis)
				dis = otherDis;
		}
	}

	if (dis > 0.f)
		return true;
	return false; 
}

void CCollision_Manager::Free()
{

	for(auto& collList : m_CollList)
	{
		for (auto& pBoxCollider : collList)
			Safe_Release(pBoxCollider);
		collList.clear();
	}

}


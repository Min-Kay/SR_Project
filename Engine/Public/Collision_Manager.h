#pragma once
#include "Base.h"

BEGIN(Engine)
class CCollider;
class CBoxCollider;
class CSphereCollider;
class CGameObject;

class CCollision_Manager :
    public CBase
{
	DECLARE_SINGLETON(CCollision_Manager)

protected:
	explicit CCollision_Manager();
	virtual ~CCollision_Manager() = default;

public:
	typedef enum tagCollider
	{
		COLL_BOX,COLL_END
	}COLLIDER;


public:
	list<CGameObject*> Collision_All();
	list<CGameObject*> Collision_Box();

public:
	HRESULT Add_Collider(COLLIDER _type, CCollider* collider);
	
private:
	list<CCollider*>* m_CollList = nullptr;

public:
	virtual void Free() override;
};
END


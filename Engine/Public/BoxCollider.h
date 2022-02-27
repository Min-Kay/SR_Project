#pragma once
#include "Collider.h"
#include "BoxCollider.h"
#include "VIBuffer_Cube.h"

BEGIN(Engine)
class CTransform;
class ENGINE_DLL CBoxCollider :
    public CCollider
{
protected:
	explicit  CBoxCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit  CBoxCollider(const CBoxCollider& rhs);

	virtual ~CBoxCollider() = default;

public:
	typedef enum taginfo {
		COLL_CENTER,COLL_SIZE,COLL_MIN,COLL_MAX,COLL_END
	}COLLIDERINFO;

	//typedef struct tagDesc {
	//	_float3 CollInfo[4];
	//	COLLSTYLE Type;
	//	CGameObject* Parent;
	//}COLLDESC;


public:
	HRESULT Update() override;

public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;


	_float3 Get_State(COLLIDERINFO info);
	HRESULT Set_State(COLLIDERINFO info , _float3 Input);
	HRESULT Set_ParentInfo(CGameObject* obj);

public:
	void Set_AdditionalPos(_float3 pos);
	const _float3& Get_AdditionalPos() const;
	HRESULT Set_Coilider();
	void Draw_Box();
	
	void Reflect_Direction(_float3& _vDir);

private:
	CTransform* m_ParentPos = nullptr;
	_float3 m_CollInfo[4];
	_float3 m_AdditionaPos = _float3(0.f,0.f,0.f);

public:
	static CBoxCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CComponent* Clone(void* pArg) override;
	void Free() override;

};

END
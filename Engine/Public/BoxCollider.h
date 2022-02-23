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
		COLL_CENTER,COLL_SIZE,COLL_MIN,COLL_MAX
	}COLLIDERINFO;

	//typedef struct tagDesc {
	//	_float3 CollInfo[4];
	//	COLLTYPE Type;
	//	CGameObject* Parent;
	//}COLLDESC;


public:
	HRESULT Update() override;

public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;


	_float3 Get_State(COLLIDERINFO info);
	HRESULT Set_State(COLLIDERINFO info , _float3 Input);
	HRESULT Get_Parentcom();
	HRESULT Set_Coilider();
	
	//_float3 m_VerticesIndex[];
	//�θ���ġ �ް� �� ���� ��ġ �ٲٰ�
	//state �������°� ����
	//����ü�� �̸� �������ְ� v
	//����ü�� ���� , size ,min , max v
	//
private:
	CTransform* m_ParentPos = nullptr;
	_float3 m_CollInfo[4];
	
public:
	static CBoxCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CComponent* Clone(void* pArg) override;
	void Free() override;

};

END
#pragma once

/* 객체의 월드공간에서의 상태(Right, Up, Look, Position) 를 표현한다. */
/* 보관  + 변환 */
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:
	typedef struct tagTransformDesc
	{
		_float		fSpeedPerSec;
		_float		fRotationPerSec;
	}TRANSFORMDESC;
public:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	_float3 Get_State(STATE eState) {
		return *(_float3*)&m_WorldMatrix.m[eState][0];
	}

	_float3 Get_Scale();

	_float4x4 Get_WorldMatrixInverse();
	

	void Set_State(STATE eState, const _float3& vState) {
		memcpy(&m_WorldMatrix.m[eState][0], &vState, sizeof(_float3));
	}

	void Set_TransformDesc(const TRANSFORMDESC& TransformDesc);

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	void Go_Straight(_float fTimeDelta);
	void Go_BackWard(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);
	void LookAt(_float3 vTargetPos);
	void Turn(_float3 vAxis, _float fTimeDelta);
	void Rotation(_float3 vAxis, _float fRadian);
	void Scaled(_float3 vScale);
	

public:
	HRESULT Bind_OnGraphicDevice();

private:
	_float4x4			m_WorldMatrix;
	TRANSFORMDESC		m_TransformDesc;

public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
#pragma once

/* 객체의 월드공간에서의 상태(Right, Up, Look, Position) 를 표현한다. */
/* 보관  + 변환 */
#include "Component.h"

BEGIN(Engine)
class CShader;
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

	_float4x4 Get_WorldMatrix() const { return m_WorldMatrix; }

	_float4x4 Get_WorldMatrixInverse();
	
	_float	Get_Gravity() const { return m_fGravity; }

	_float	Get_Force() const { return m_fForce; }

	_float	Get_CollideFormalForce() const { return m_FormalForce; }

	void Set_WorldMatrix(_float4x4 _wm) { m_WorldMatrix = _wm; }

	void Set_State(STATE eState, const _float3& vState) {
		memcpy(&m_WorldMatrix.m[eState][0], &vState, sizeof(_float3));
	}

	void Set_TransformDesc(const TRANSFORMDESC& TransformDesc);
	const TRANSFORMDESC& Get_TransformDesc() const;

	HRESULT Bind_OnShader(CShader* _shader);

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	void Go_Straight(_float fTimeDelta);
	void Go_BackWard(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);
	void Go_Up(_float fTimeDelta);
	void Go_Down(_float fTimeDelta);

	void LookAt(_float3 vTargetPos);
	void Turn(_float3 vAxis, _float fTimeDelta);
	void Rotation(_float3 vAxis, _float fRadian);
	void Scaled(_float3 vScale);

public:
	void Gravity(_float weight , _float fTimeDelta);
	void Add_Velocity(_float _add);
	void Set_Velocity(_float _vel);
	const _float& Get_Velocity() const;

	void Add_Force(_float fTimeDelta);
	void Set_Force(_float3 _axis);

	void Set_OnCollide(_bool _bool);
	const _bool& Get_OnCollide() const;

public:
	HRESULT Bind_OnGraphicDevice();

private:
	_float4x4			m_WorldMatrix;
	TRANSFORMDESC		m_TransformDesc;

	_bool				m_bOnCollide = false;
	_bool				m_Forcing = false; 
private:
	_float				m_fGravity = 9.8f;
	_float				m_fVelocity = 0.f;
	_float				m_Press = 0.f;
	_float				m_FormalForce = 0.f;
	_float3				m_vForceAxis = _float3(0.f,0.f,0.f);
	_float				m_fForce = 0.f;

public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
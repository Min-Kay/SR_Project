#pragma once
#include "Enemy.h"
BEGIN(Engine)
class CTransform;
class CBoxCollider;
class CRenderer;
class CTexture;
class CShader;
class CVIBuffer_Cube;
END

BEGIN(Client)
class CSunflower :
    public CEnemy
{
protected:
	explicit CSunflower(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	explicit CSunflower(const CSunflower& rhs);
	virtual ~CSunflower() = default;

public:
	typedef enum tagSunflower {SF_CHARGING, SF_FIRING}SUNFLOWER;

public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;

public:
	HRESULT SetUp_Component() override;
	void Add_HP(_int _add) override;

public:
	void Set_Valid(_bool _bool);
	const _bool& Get_Valid() const;

	void Fire();
	void Set_Parent(CGameObject* _parent);

	const _bool& Get_Charging() const { return m_Charging; }
	const _bool& Get_Firing() const { return m_Firing; }

	const SUNFLOWER& Get_State() const { return m_State; }
private:
	void Firing(_float fTimeDelta);
	void Synchronize_Parent(_float fTimeDelta);
private:
	_float m_fTimer = 0.f;

	_bool m_Valid = true;

	_bool m_Charging = false;
	_float m_ChargingTime = 1.f;

	_bool m_Firing = false;
	_float m_FiringTime = 3.f;


	_bool m_Ending = true;
	_float m_EndingTime = 1.f;

private:
	CTransform* m_pTransform = nullptr;
	CTransform* m_pOnlyRotation = nullptr;

	CRenderer* m_pRenderer = nullptr;
	CBoxCollider* m_pCollider = nullptr;
	CShader* m_pShader = nullptr;
	CTexture* m_pTexture = nullptr;
	CVIBuffer_Cube* m_pBuffer = nullptr;


private:
	SUNFLOWER m_State = SF_CHARGING;
	_float4 m_Color = _float4(0.f,0.f,0.f,0.f);
	_float4 m_NormalColor = _float4(1.f, 1.f, 1.f, 0.5f);
	_float4 m_ChargingColor = _float4(1.f,0.f,0.f,0.5f);
	_float4 m_FiringColor = _float4(1.f, 0.5f, 0.f, 0.5f);


	_float3 m_NormalSize = _float3(0.2f, 0.2f, 100.f);
	_float3 m_FiringSize = _float3(7.f, 7.f, 100.f);

private:
	CGameObject* m_Parent = nullptr;
	CTransform* m_ParentTransform = nullptr;

public:
	static CSunflower* Create(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};
END

#pragma once
#include "Client_Defines.h"
#include "Effect.h"

BEGIN(Engine)
class CBoxCollider;
END 

BEGIN(Client)
class CBall :
    public CEffect
{
protected:
	explicit CBall(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	explicit CBall(const CBall& rhs);
	virtual ~CBall() = default;

public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;

public:
	void Set_Init(_float3 _pos, _float3 _dir);

private:
	_bool Check_Collide();

	void Move(_float fTimeDelta);

private:
	CBoxCollider* m_pBoxCollider = nullptr;


	_int m_Damage = 5.f;
	_float m_Speed = 2.f;

	_float3 m_Dir;

	_float m_Timer = 0.f;

public:
	static CBall* Create(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;

protected:
	HRESULT SetUp_Components() override;
	HRESULT SetUp_RenderState() override;
	HRESULT Release_RenderState() override;
};
END

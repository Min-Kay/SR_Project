#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
BEGIN(Engine)
class CRenderer;
class CBoxCollider;
class CTexture;
class CTransform;
class CVIBuffer_Cube;
class CShader;
END

BEGIN(Client) 
class CCubeBullet :
    public CGameObject
{
protected:
	explicit CCubeBullet(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	explicit CCubeBullet(const CCubeBullet& rhs);
	virtual ~CCubeBullet() = default;
	
public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;

public:
	void CCubeBullet::Set_Init(_float3 _pos, _float3 _dir);
private:
	void CCubeBullet::Move(_float fTimeDelta);
	HRESULT Synchronize_Transform(_float fTimeDelata);
	_bool Check_Collide();
private:
	HRESULT SetUp_Component();

private:
	CVIBuffer_Cube* m_pVIBufferCom = nullptr;

	CRenderer* m_pRendererCom = nullptr;

	CTexture* m_pTextureCom = nullptr;

	CBoxCollider* m_pBoxCollider = nullptr;

	CTransform* m_pTransformCom = nullptr;

	CTransform* m_pOnlyRotation = nullptr;

	CShader* m_pShader = nullptr; 

private:
	_int m_Damage = 5;
	_float m_Speed = 1.f;
	_float m_Timer = 0.f;

public:
	static CCubeBullet* Create(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};
END

#pragma once
#include "Client_Defines.h"
#include "Enemy.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CBoxCollider;
class CTexture;
class CShader;
class CVIBuffer_Cube;
END

BEGIN(Client)
class CPlayer;
class CLaser :
    public CEnemy
{
protected:
	explicit  CLaser(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	explicit CLaser(const CLaser& rhs);
	virtual ~CLaser() = default;
public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;

private:
	void Move(_float fTimeDelta);
	void Check_Player_Hit(_float fTimeDelta);
public:
	HRESULT SetUp_Component() override;
	void Add_HP(_int _add) override;

	void Set_Player(CPlayer* _player);
	void Set_Direction(_uint _dir);
private:
	_float m_fTimer = 0.f;
	_float m_DeathTimer = 2.f;
	_float m_Speed = 5.f;
	_bool m_hit = false;
	_uint m_Dir = 0;
private:
	CTransform* m_pTransform = nullptr;
	CRenderer* m_pRenderer = nullptr;
	CBoxCollider* m_pCollider = nullptr;
	CTexture* m_pTexture = nullptr;
	CShader* m_pShader = nullptr;
	CVIBuffer_Cube* m_pBuffer = nullptr;

private:
	CPlayer* m_pPlayer = nullptr;
	CBoxCollider* m_pPlayerCollider = nullptr;

public:
	static CLaser* Create(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};
END
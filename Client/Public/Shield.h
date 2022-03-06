#pragma once
#include "Client_Defines.h"
#include "Enemy.h"


BEGIN(Engine)
class CTransform;
class CRenderer;
class CTexture;
class CVIBuffer_Cube;
class CBoxCollider;
END

BEGIN(Client)
class CShield :
    public CEnemy
{
protected:
	explicit CShield(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	explicit CShield(const CShield& rhs);
	virtual ~CShield() = default;

public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;
	HRESULT SetUp_Component() override;

public:
	void Set_ParentTransform(CTransform* _tr);
	virtual void Add_HP(_int _add) override;

	void Spawn_Shield();
	void Break_Shield();

	void Set_Valid(_bool _bool);
	const _bool Get_Valid() const ;
private:
	void Synchronize_Transform();
	void Sizing_Particles();

private:
	CTransform* m_pTransform = nullptr;
	CRenderer* m_pRenderer = nullptr;
	CTexture* m_pTexture = nullptr;
	CVIBuffer_Cube* m_pBuffer = nullptr;
	CBoxCollider* m_pCollider = nullptr;

private:
	CTransform* m_ParentTr = nullptr;

private:
	_bool m_Valid = false;

public:
	static CShield* Create(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};
END

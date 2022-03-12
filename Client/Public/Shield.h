#pragma once
#include "Client_Defines.h"
#include "Enemy.h"


BEGIN(Engine)
class CTransform;
class CRenderer;
class CTexture;
class CVIBuffer_Cube;
class CBoxCollider;
class CShader;
END

BEGIN(Client)
class CBoss;
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

	void Spawn_Shield(_float fTimeDelta);
	void Break_Shield(_float fTimeDelta);

	void Add_ShieldHp(_int _add);

	void Set_Valid(_bool _bool);
	const _bool Get_Valid() const ;


	void Set_Parent(CBoss* _boss);
	const _int& Get_InitHp() const { return m_InitHp; }
private:
	void Synchronize_Transform();
	void Sizing_Particles();

private:
	CTransform* m_pTransform = nullptr;
	CRenderer* m_pRenderer = nullptr;
	CTexture* m_pTexture = nullptr;
	CVIBuffer_Cube* m_pBuffer = nullptr;
	CBoxCollider* m_pCollider = nullptr;
	CShader* m_pShader = nullptr;
private:
	CBoss* m_Parent = nullptr;
	CTransform* m_ParentTr = nullptr;

private:
	_float4 m_Color = _float4(0.f,0.f,0.f,0.3f); 
	_float m_Timer = 0.f;
	_bool m_Valid = false;
	_bool m_On = false;
	_bool m_Complete = false;

	_int m_InitHp = 100;
public:
	static CShield* Create(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};
END

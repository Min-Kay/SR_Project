#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CVIBuffer_Cube;
class CRenderer;
class CBoxCollider;
END


BEGIN(Client)
class CEnemy :
    public CGameObject
{
protected:
	explicit CEnemy(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	explicit CEnemy(const CEnemy& rhs);
	virtual ~CEnemy() = default;

public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;

public:
	virtual HRESULT SetUp_Component()PURE;

	void Set_HP(_int _hp);
	void Add_HP(_int _add);
	const _int& Get_HP() const;

	const _int& Get_Damage() const;

protected:
	_bool Check_HP(); 

protected:
	_int m_Hp;
	_int m_Damage;


public:
	CGameObject* Clone(void* pArg) = 0;
	void Free() override;
};
END

#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CVIBuffer_Cube;
class CTexture;
class CShader;
END

BEGIN(Client)
class CShield_Effect :
    public CGameObject
{
protected:
	explicit CShield_Effect(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	explicit CShield_Effect(const CShield_Effect& rhs);
	virtual ~CShield_Effect() = default;

public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;

public:
	void Set_Parent(CTransform* _tr);
	void Set_SpreadRange(_float _range);
	void Set_Split(_bool _bool);
	void Set_TurnType(_bool _xz , _bool _y = false);
	void Set_Valid(_bool _bool);
	void Set_CoolTime(_float _cool);

private:
	void Turning(_float fTimeDelta);
	void Split(_float fTimeDelta);

private:
	CTransform* m_pParent = nullptr;
	_float m_SpreadRange = 0.f; 
	_float m_Timer = 0.f;
	_float m_CoolTime = 0.f;

	_bool m_CoolDown = false;
	_bool m_Split = false;
	_bool m_TurnType = false;
	_bool m_YType = false;


	_bool m_Valid = false;
private:
	CTransform* m_pTransform = nullptr;
	CRenderer* m_pRenderer = nullptr;
	CVIBuffer_Cube* m_pBuffer = nullptr;
	CTexture* m_pTexture = nullptr;
	CShader* m_pShader = nullptr;


public:
	static CShield_Effect* Create(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};


END

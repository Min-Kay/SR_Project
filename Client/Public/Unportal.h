#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CVIBuffer_Cube;
class CTexture;
class CBoxCollider;
class CShader; 
END

BEGIN(Client)
class CPlayer;

class CUnportal :
    public CGameObject
{
protected:
	explicit CUnportal(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	explicit CUnportal(const CUnportal& rhs);
	virtual ~CUnportal() = default;

public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;


private:
	HRESULT SetUp_Component();

private:
	CTransform* m_pTransform = nullptr;
	CBoxCollider* m_pCollider = nullptr;
	CRenderer* m_pRenderer = nullptr;
	CTexture* m_pTexture = nullptr;
	CVIBuffer_Cube* m_pBuffer = nullptr;
	CShader* m_pShader = nullptr;
private:
	CPlayer* m_pPlayer = nullptr;

public:
	static CUnportal* Create(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};
END 

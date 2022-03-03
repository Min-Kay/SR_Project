#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CBoxCollider;
class CVIBuffer_Cube;
class CTexture;
class CRenderer;
END

BEGIN(Client)
class CBazierBullet :
    public CGameObject
{
protected:
	explicit CBazierBullet(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	explicit CBazierBullet(const CBazierBullet& rhs);
	virtual ~CBazierBullet() = default;

public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;


private:
	HRESULT SetUp_Component();

public:
	void Set_Pos(_float3 _start,_float3 _mid, _float3 _end);


private:
	void Move_By_Bazier(_float fTimeDelta);

private:
	_float3 m_StartPos;
	_float3 m_MidPos;
	_float3 m_EndPos;
	_float m_fTimer = 0.f;

private:
	CVIBuffer_Cube* m_pVIBufferCom = nullptr;

	CRenderer* m_pRendererCom = nullptr;

	CTexture* m_pTextureCom = nullptr;

	CBoxCollider* m_pBoxCollider = nullptr;

	CTransform* m_pTransformCom = nullptr;

public:
	static CBazierBullet* Create(LPDIRECT3DDEVICE9 m_pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};
END

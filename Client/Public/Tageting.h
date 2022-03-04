#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CTexture;
class CVIBuffer_Rect;
class CBoxCollider;
END

BEGIN(Client)
class CPlayer;
class CTageting :
	public CGameObject
{
protected:
	explicit CTageting(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTageting(const CTageting& rhs);
	virtual ~CTageting() = default;

public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
	_int Tick(_float fTimeDelta) override;
	_int LateTick(_float fTimeDelta) override;
	HRESULT Render() override;

	HRESULT	Set_RanderState();
	HRESULT	Release_RanderState();
	HRESULT SetUp_Component();
	HRESULT FaceOn_Camera();
private:

	CTexture*			m_pTextureCom = nullptr;

	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

	CTransform*			m_pTransformCom = nullptr;

	CRenderer*			m_pRendererCom = nullptr;

	CBoxCollider*		m_pBoxColliderCom = nullptr;


private:
	_float3 m_tagetPos;
	CPlayer* m_pPlayer =nullptr;;
	_float	timer =0.0f;
	CTransform*		m_pTarget = nullptr;
public:
	static CTageting* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};
END


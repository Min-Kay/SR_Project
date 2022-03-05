#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Cube;
class CBoxCollider;
END

BEGIN(Client)
class CPlayer;
class CWater final : public CGameObject
{
private:
	explicit CWater(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CWater(const CWater& rhs);
	virtual ~CWater() = default;

public:
	
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Player(CPlayer* _player) {
		m_Player = _player;
	}

private:
	/* �ؽ��� */
	CTexture*				m_pTextureCom = nullptr;

	/* �� */
	CVIBuffer_Cube*		m_pVIBufferCom = nullptr;

	/* ���� (��ġ, ũ��, ȸ��) */
	CTransform*			m_pTransformCom = nullptr;

	/* �׷�����. */
	CRenderer*			m_pRendererCom = nullptr;

	/* �浹ü */
	CBoxCollider*		 m_pBoxColliderCom = nullptr;

	_float				m_fFrame = 0.f;
	_float				m_iCount = 0.f;
private:
	HRESULT SetUp_Components();


	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

	CPlayer*		m_Player = nullptr;
public:
	static CWater* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
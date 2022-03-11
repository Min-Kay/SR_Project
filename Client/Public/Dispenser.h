#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Cube;
class CBoxCollider;
class CShader;
END

BEGIN(Client)
class CPlayer;
class CDispenser final : public CGameObject
{
private:
	explicit CDispenser(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CDispenser(const CDispenser& rhs);
	virtual ~CDispenser() = default;

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
	CShader*				 m_pShader = nullptr;

	_float				m_fFrame = 0.f;
	_float				m_iCount = 0.f;
private:
	HRESULT SetUp_Components();
	CPlayer*		m_Player = nullptr;
public:
	static CDispenser* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
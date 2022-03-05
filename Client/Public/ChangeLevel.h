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
class CChangeLevel final : public CGameObject
{
private:
	explicit CChangeLevel(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CChangeLevel(const CChangeLevel& rhs);
	virtual ~CChangeLevel() = default;

public:
	_bool	Get_LevelOut() { return m_bLevelOut; }

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;



private:
	/* �ؽ��� */
	CTexture*			m_pTextureCom = nullptr;

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

	CPlayer*			m_Player = nullptr;
private:
	HRESULT SetUp_Components();


	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

	_bool		m_bLevelOut = false;
public:
	static CChangeLevel* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
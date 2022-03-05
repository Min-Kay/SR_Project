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
	/* 텍스쳐 */
	CTexture*			m_pTextureCom = nullptr;

	/* 모델 */
	CVIBuffer_Cube*		m_pVIBufferCom = nullptr;

	/* 상태 (위치, 크기, 회전) */
	CTransform*			m_pTransformCom = nullptr;

	/* 그려진다. */
	CRenderer*			m_pRendererCom = nullptr;

	/* 충돌체 */
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
#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CBoxCollider;

END

BEGIN(Client)

class CTile final : public CGameObject
{
private:
	explicit CTile(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTile(const CTile& rhs);
	virtual ~CTile() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	/* 텍스쳐 */
	CTexture*				m_pTextureCom = nullptr;

	/* 모델 */
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

	/* 상태 (위치, 크기, 회전) */
	CTransform*			m_pTransformCom = nullptr;

	/* 그려진다. */
	CRenderer*			m_pRendererCom = nullptr;

private:
	_uint				m_iTextureIndex = 2;

private:
	HRESULT SetUp_Components();

public:
	void	Set_TextureIndex(_uint _iTextureIndex)
	{
		m_iTextureIndex = _iTextureIndex;
	}

public:
	static CTile* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
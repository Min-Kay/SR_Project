#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI final : public CGameObject
{
private:
	explicit CUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI(const CUI& rhs);
	virtual ~CUI() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_float4x4			m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

private:
	/* �ؽ��� */
	CTexture*			m_pTextureCom = nullptr;

	/* �� */
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

	/* ���� (��ġ, ũ��, ȸ��) */
	CTransform*			m_pTransformCom = nullptr;

	/* �׷�����. */
	CRenderer*			m_pRendererCom = nullptr;

private:
	HRESULT SetUp_Components();

public:
	static CUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
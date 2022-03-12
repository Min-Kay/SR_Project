#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CBoxCollider;
class CShader;

END

BEGIN(Client)

class CTileCollider final : public CGameObject
{
private:
	explicit CTileCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTileCollider(const CTileCollider& rhs);
	virtual ~CTileCollider() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Color(_float4 _color) { m_Color = _color; }

private:
	/* �ؽ��� */
	CTexture*				m_pTextureCom = nullptr;

	/* �� */
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

	/* ���� (��ġ, ũ��, ȸ��) */
	CTransform*			m_pTransformCom = nullptr;

	/* �׷�����. */
	CRenderer*			m_pRendererCom = nullptr;

	/* �浹ü */
	CBoxCollider*		m_pBoxColliderCom = nullptr;


	CShader*				m_pShader = nullptr;
private:
	_uint				m_iTextureIndex = 2;


	_float4				m_Color = _float4(0.f,0.f,0.f,0.f);
private:
	HRESULT SetUp_Components();

public:
	void Set_TextureIndex(_uint _iTextureIndex)
	{
		m_iTextureIndex = _iTextureIndex;
	}

public:
	static CTileCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
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

class CTile_Cube : public CGameObject
{
private:
	explicit CTile_Cube(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTile_Cube(const CTile_Cube& rhs);
	virtual ~CTile_Cube() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

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
	CBoxCollider*		m_pBoxColliderCom = nullptr;

private:
	_uint				m_iTextureIndex = 0;
	_bool				m_bOpen = true;
	_bool				m_bClose = false;
private:
	HRESULT SetUp_Components();
public:
	HRESULT	Open_Event(const _tchar* pLayerTag, const _tchar* pLayerTag2);
	void	Set_TextureIndex(_uint _iTextureIndex)
	{
		m_iTextureIndex = _iTextureIndex;
	}

public:
	static CTile_Cube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
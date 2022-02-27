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

class CDoor_right final : public CGameObject
{
private:
	explicit CDoor_right(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CDoor_right(const CDoor_right& rhs);
	virtual ~CDoor_right() = default;

public:
	
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
	
	_uint m_Count = 0;

private:
	HRESULT SetUp_Components();
public:

	HRESULT Open(_bool IsOpen, _float fTimeDelta);

public:
	static CDoor_right* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
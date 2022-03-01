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
	/* 텍스쳐 */
	CTexture*				m_pTextureCom = nullptr;

	/* 모델 */
	CVIBuffer_Cube*		m_pVIBufferCom = nullptr;

	/* 상태 (위치, 크기, 회전) */
	CTransform*			m_pTransformCom = nullptr;

	/* 그려진다. */
	CRenderer*			m_pRendererCom = nullptr;

	/* 충돌체 */
	CBoxCollider*		m_pBoxColliderCom = nullptr;
	
	_float m_Count = 0;
	const _float m_MaxCount = 1;
	//const _float m_MinCount = 0;
private:
	HRESULT SetUp_Components();
public:

	HRESULT Open(_bool& IsOpen, _float fTimeDelta);
	HRESULT Close(_bool& IsClose, _float fTimeDelta);

public:
	static CDoor_right* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
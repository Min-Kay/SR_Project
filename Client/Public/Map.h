#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
class CVIBuffer_Terrain;
END

BEGIN(Client)

class CMap final : public CGameObject
{
private:
	explicit CMap(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMap(const CMap& rhs);
	virtual ~CMap() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	/* 텍스쳐 */

	/* 모델 */
	CVIBuffer_Terrain* m_pVIBufferCom = nullptr;

	/* 상태 (위치, 크기, 회전) */
	CTransform* m_pTransformCom = nullptr;

	/* 그려진다. */
	CRenderer* m_pRendererCom = nullptr;

private:
	HRESULT SetUp_Components();



public:
	static CMap* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

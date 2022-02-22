#pragma once

#include "Tool_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Tool)

class CBackGround final : public CGameObject
{
private:
	explicit CBackGround(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBackGround(const CBackGround& rhs);
	virtual ~CBackGround() = default;
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
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

	/* ���� (��ġ, ũ��, ȸ��) */
	CTransform*			m_pTransformCom = nullptr;

	/* �׷�����. */
	CRenderer*			m_pRendererCom = nullptr;

private:
	HRESULT SetUp_Components();


	_float3 m_Rect;
	_int m_TextNum = 0;
private:
	POINT m_MousePos;
	POINT m_HoldMousePos;

public:
	static CBackGround* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Cube;
class CBoxCollider;
class CShader;
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

public:
	void Set_Color(_float4 _color) { m_Color = _color; }
	void Set_Vaild(_bool _bool) { m_Vaild = _bool; }

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

	CShader*			m_pShader = nullptr;

private:
	_uint				m_iTextureIndex = 0;

	CBoxCollider*		m_PlayerCollider = nullptr;


private:
	_bool				m_Vaild = true;
	_float4				m_Color = _float4(0.f,0.f,0.f,0.f);
private:
	HRESULT SetUp_Components();
public:
	_bool	Open_Event(_uint iLevelIndex, const _tchar* pLeftDoorLayerTag, const _tchar* pRightDoorLayerTag);
	_bool Open_Block_Event(_uint iLevelIndex, const _tchar* pLeftDoorLayerTag, const _tchar* pRightDoorLayerTag);
	_bool	Close_Event(_uint iLevelIndex, const _tchar* pLeftDoorLayerTag, const _tchar* pRightDoorLayerTag);
	_bool Save_Point();
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
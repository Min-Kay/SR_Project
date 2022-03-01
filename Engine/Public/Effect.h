#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;

class ENGINE_DLL CEffect :
    public CGameObject
{
protected:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	void Tick_Effect(_float fTimeDelta);

public:
	typedef enum tagEffectStyle { EFFECTSTYLE_FIX, EFFECTSTYLE_STRAIGHT,EFFECTSTYLE_REPEAT,EFFECTSTYLE_WAVE, EFFECTSTYLE_END} EFFECTSTYLE;
	typedef enum tagEffectAlpha {
		EFFECTALPHA_DEFAULT, EFFECTALPHA_TEST, EFFECTALPHA_BLEND, EFFECTALPHA_END
	}EFFECTALPHA;

public:
	typedef struct tagEffect
	{
		EFFECTSTYLE Style;
		_float AnimateSpeed;
		_uint FrameCount;
		EFFECTALPHA Alpha;
		D3DCMPFUNC Func;
		_uint	Ref;
		_bool Bilboard;
		_bool FixY;
		const _tchar* Texture;
	}EFFECTDESC;

protected:
	/* 텍스쳐 */
	CTexture* m_pTextureCom = nullptr;

	/* 모델 */
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;

	/* 상태 (위치, 크기, 회전) */
	CTransform* m_pTransformCom = nullptr;

	/* 그려진다. */
	CRenderer* m_pRendererCom = nullptr;

protected:
	_bool				m_Vaild = true; 
	EFFECTDESC			m_Desc;
	_uint				m_iCurrFrameIndex = 0;
	_float				m_fFrame = 0.f;

	_bool				m_Wave = false;

protected:
	virtual HRESULT SetUp_Components();
	virtual HRESULT SetUp_RenderState();
	virtual HRESULT Release_RenderState();

public:
	void Set_CurrentFrameIndex(_uint iIndex);
	void Set_Style(EFFECTSTYLE _style);

	const _uint& Get_CurrentFrameIndex() const;

public:
	const _bool& Get_Vaild() const;
	void Set_Vaild(_bool _bool);
	

protected:
	HRESULT FaceOn_Camera();

public:
	static CEffect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
   
};
END

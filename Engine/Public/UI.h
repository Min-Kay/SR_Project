#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;

class ENGINE_DLL CUI  : public CGameObject
{
protected:
	explicit CUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI(const CUI& rhs);
	virtual ~CUI() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	typedef enum tagStyle {
		STYLE_FIX, STYLE_STRAIGHT, STYLE_REPEAT, STYLE_WAVE, STYLE_END
	}STYLE;

	typedef enum tagAlpha {
		ALPHA_DEFAULT, ALPHA_TEST,ALPHA_BLEND, ALPHA_END
	}ALPHA;

	typedef struct taguidesc {
		_uint WinCX, WinCY;
		_uint Layer;
		_float PosX, PosY, SizeX, SizeY;
		_uint FrameCount;
		_float AnimateSpeed;
		STYLE	Style;
		ALPHA	Alpha;
		D3DCMPFUNC Func;
		_uint	Ref;
		const _tchar* Texture;
	}UIDESC;

protected:
	_float4x4			m_ProjMatrix;
	_float				m_WinCX, m_WinCY, m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float				m_fFrame = 0.f;
	_float				m_AnimSpd = 0.f;
	_uint				m_FrameCount = 0;
	_uint				m_CurrFrame = 0;
	STYLE				m_Style;
	ALPHA				m_Alpha;

protected:
	D3DCMPFUNC			m_func;
	_uint				m_AlphaRef = 0;

protected:
	_bool				m_wave = false;
	_bool				m_Vaild = true;

protected:
	/* �ؽ��� */
	CTexture*			m_pTextureCom = nullptr;

	/* �� */
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

	/* ���� (��ġ, ũ��, ȸ��) */
	CTransform*			m_pTransformCom = nullptr;

	/* �׷�����. */
	CRenderer*			m_pRendererCom = nullptr;

protected:
	virtual HRESULT SetUp_Components(const _tchar* _texture);

public:
	HRESULT Set_UI(_uint iWinCX, _uint iWinCY, _float x,_float y, _float sizeX, _float sizeY);
	HRESULT Set_CurrFrameIndex(_uint iIndex);
	const _uint& Get_CurrFrameIndex() const;

	HRESULT Set_Style(STYLE _style);
	void Set_AlphaTest(D3DCMPFUNC _func, _uint ref = 0);

public:
	HRESULT Set_Pos(_float fx, _float fy);
	HRESULT Set_Size(_float sizeX, _float sizeY);

public:
	virtual HRESULT Tick_UI(_float fTimeDelta);
	virtual HRESULT Set_RenderState();

public:
	void Set_Vaild(_bool _bool);
	const _bool Get_Vaild() const;

protected:
	HRESULT Bind_UI();

public:
	static CUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
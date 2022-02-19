#include "stdafx.h"
#include "UI.h"
#include "GameInstance.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CUI::CUI(const CUI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI::NativeConstruct(void * pArg)
{	
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	UIDESC desc = *static_cast<UIDESC*>(pArg);
	m_fSizeX = desc.SizeX;
	m_fSizeY = desc.SizeY;
	m_fX = desc.PosX;
	m_fY = desc.PosY;
	Set_Layer(desc.Layer);
	m_AnimSpd = desc.AnimateSpeed;
	m_Style = desc.Style;
	m_FrameCount = desc.FrameCount;

	m_Alpha = desc.Alpha;
	m_AlphaRef = desc.Ref;
	m_func = desc.Func;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components(desc.Texture)))
		return E_FAIL;	
	
	Set_UI(m_fX, m_fY, m_fSizeX, m_fSizeY);
	
	return S_OK;
}

_int CUI::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	Tick_UI(fTimeDelta);

	return _int();
}

_int CUI::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return _int();
}

HRESULT CUI::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	Bind_UI();

	Set_RenderState();

	return S_OK;
}

HRESULT CUI::SetUp_Components(const _tchar* _texture)
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RECT, COM_BUFFER, (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _texture,COM_TEXTURE, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI::Set_CurrFrameIndex(_uint iIndex)
{
	if (m_FrameCount <= iIndex)
		return E_FAIL;

	m_CurrFrame = iIndex;

	return S_OK;
}

HRESULT CUI::Set_UI(_float x, _float y, _float sizeX, _float sizeY)
{
	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinCX, g_iWinCY, 0.0f, 1.f);

	m_fSizeX = sizeX;
	m_fSizeY = sizeY;

	m_fX = x;
	m_fY = y;

	m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinCX * 0.5f, -m_fY + g_iWinCY * 0.5f, 0.f));

	return S_OK;
}

HRESULT CUI::Set_Style(STYLE _style)
{
	if (_style >= STYLE_END)
		return E_FAIL;

	m_Style = _style;

	return S_OK;
}

HRESULT CUI::Tick_UI(_float fTimeDelta)
{
	switch (m_Style)
	{
	case STYLE_FIX:
		break;
	case STYLE_STRAIGHT:
		if ((_uint)m_fFrame >= m_FrameCount - 1)
			break;

		m_CurrFrame = (_uint)m_fFrame;
		m_fFrame += m_AnimSpd * fTimeDelta;

		break;
	case STYLE_REPEAT:
		if ((_uint)m_fFrame >= m_FrameCount - 1)
			m_fFrame = 0.f;

		m_CurrFrame = (_uint)m_fFrame;
		m_fFrame += m_AnimSpd * fTimeDelta;

		break;
	case STYLE_WAVE:
		if ((_uint)m_fFrame >= m_FrameCount - 1)
			m_wave = true;
		else if ((_uint)m_fFrame <= 0)
			m_wave = false;

		m_CurrFrame = (_uint)m_fFrame;
		m_fFrame = m_wave ? m_fFrame - m_AnimSpd * fTimeDelta : m_fFrame + m_AnimSpd * fTimeDelta;

		break;
	}

	return S_OK;
}

HRESULT CUI::Set_RenderState()
{

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(m_CurrFrame)))
		return E_FAIL;

	switch (m_Alpha)
	{
	case ALPHA_DEFAULT:
		m_pVIBufferCom->Render();

		break;
	case ALPHA_BLEND:
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_pVIBufferCom->Render();

		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		break;
	case ALPHA_TEST:
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, m_AlphaRef);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, m_func);

		m_pVIBufferCom->Render();

		m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		break;
	}

	return S_OK;
}

void CUI::Set_AlphaTest(D3DCMPFUNC _func, _uint ref)
{
	m_func = _func;
	m_AlphaRef = ref;
}

HRESULT CUI::Bind_UI()	
{
	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	_float4x4		ViewMatrix;
	D3DXMatrixIdentity(&ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
}

CUI * CUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI*	pInstance = new CUI(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI::Clone(void* pArg )
{
	/* 새로운객체를 복제하여 생성한다. */
	CUI*	pInstance = new CUI(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom); 
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}

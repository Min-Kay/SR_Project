#include "UI.h"
#include "GameInstance.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CUI::CUI(const CUI& rhs)
	: CGameObject(rhs)
	, m_pShader(rhs.m_pShader)
	, m_pTransformCom(rhs.m_pTransformCom)
	, m_pRendererCom(rhs.m_pRendererCom)
	, m_pVIBufferCom(rhs.m_pVIBufferCom)
{
	Safe_AddRef(m_pShader);
	Safe_AddRef(m_pTransformCom);
	Safe_AddRef(m_pRendererCom);
	Safe_AddRef(m_pVIBufferCom);
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

	Set_Type(OBJ_UI);

	UIDESC desc = *static_cast<UIDESC*>(pArg);

	m_desc = desc;

	Set_Layer(m_desc.Layer);

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;	
	
	Set_UI(m_desc.WinCX, m_desc.WinCY, m_desc.PosX, m_desc.PosY, m_desc.SizeX, m_desc.SizeY);
	
	return S_OK;
}

_int CUI::Tick(_float fTimeDelta)
{
	if (!m_Vaild)
		return 0;

	if (0 > __super::Tick(fTimeDelta))
		return -1;

	Tick_UI(fTimeDelta);

	return _int();
}

_int CUI::LateTick(_float fTimeDelta)
{
	if (!m_Vaild)
		return 0;

	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return _int();
}

HRESULT CUI::Render()
{
	if (!m_Vaild)
		return 0;

	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	//Bind_UI();

	Set_RenderState();

	return S_OK;
}

HRESULT CUI::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(0, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(0, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(0, PROTO_RECT, COM_BUFFER, (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(0, PROTO_SHADER_RECT, COM_SHADER, (CComponent**)&m_pShader)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(0, m_desc.Texture,COM_TEXTURE, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI::Set_CurrFrameIndex(_uint iIndex)
{
	if (m_desc.FrameCount <= iIndex)
		return E_FAIL;

	m_CurrFrame = iIndex;
	m_fFrame = (_float)m_CurrFrame;

	return S_OK;
}

const _uint& CUI::Get_CurrFrameIndex() const
{
	return m_CurrFrame;
}

HRESULT CUI::Set_UI(_uint iWinCX, _uint iWinCY, _float x, _float y, _float sizeX, _float sizeY)
{
	D3DXMatrixOrthoLH(&m_ProjMatrix, (_float)iWinCX, (_float)iWinCY, 0.0f, 1.f);

	m_desc.SizeX = sizeX;
	m_desc.SizeY = sizeY;

	m_desc.PosX = x;
	m_desc.PosY = y;

	m_pTransformCom->Scaled(_float3(m_desc.SizeX, m_desc.SizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_desc.PosX - iWinCX * 0.5f, -m_desc.PosY + iWinCY * 0.5f, 0.f));

	return S_OK;
}

HRESULT CUI::Set_Style(STYLE _style)
{
	if (_style >= STYLE_END)
		return E_FAIL;

	m_desc.Style = _style;

	return S_OK;
}

HRESULT CUI::Tick_UI(_float fTimeDelta)
{
	switch (m_desc.Style)
	{
	case STYLE_FIX:
		break;
	case STYLE_STRAIGHT:
		if ((_uint)m_fFrame >= m_desc.FrameCount)
			break;

		m_CurrFrame = (_uint)m_fFrame;
		m_fFrame += m_desc.AnimateSpeed * fTimeDelta;

		break;
	case STYLE_REPEAT:
		if ((_uint)m_fFrame >= m_desc.FrameCount)
			m_fFrame = 0.f;

		m_CurrFrame = (_uint)m_fFrame;
		m_fFrame += m_desc.AnimateSpeed * fTimeDelta;

		break;
	case STYLE_WAVE:
		if ((_uint)m_fFrame >= m_desc.FrameCount)
			m_wave = true;
		else if ((_uint)m_fFrame <= 0)
			m_wave = false;

		m_CurrFrame = (_uint)m_fFrame;
		m_fFrame = m_wave ? m_fFrame - m_desc.AnimateSpeed * fTimeDelta : m_fFrame + m_desc.AnimateSpeed * fTimeDelta;

		break;
	}

	return S_OK;
}

HRESULT CUI::Set_RenderState()
{
	_float4x4		WorldMatrix, ViewMatrix, ProjMatrix;

	WorldMatrix = m_pTransformCom->Get_WorldMatrix();
	D3DXMatrixIdentity(&ViewMatrix);
	
	m_pShader->SetUp_ValueOnShader("g_WorldMatrix", D3DXMatrixTranspose(&WorldMatrix, &WorldMatrix), sizeof(_float4x4));
	m_pShader->SetUp_ValueOnShader("g_ViewMatrix", D3DXMatrixTranspose(&ViewMatrix, &ViewMatrix), sizeof(_float4x4));
	m_pShader->SetUp_ValueOnShader("g_ProjMatrix", D3DXMatrixTranspose(&ProjMatrix, &m_ProjMatrix), sizeof(_float4x4));

	m_pShader->SetUp_ValueOnShader("g_ColorStack", m_desc.Shader_Control, sizeof(_float));
	m_pTextureCom->Bind_OnShader(m_pShader, "g_Texture",m_CurrFrame);
	m_pShader->Begin_Shader(m_desc.Shader_Style);
	m_pVIBufferCom->Render();
	m_pShader->End_Shader();

	/*switch (m_desc.Alpha)
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
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, m_desc.Ref);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, m_desc.Func);

		m_pVIBufferCom->Render();

		m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		break;
	}*/


	return S_OK;
}

void CUI::Set_Vaild(_bool _bool)
{
	m_Vaild = _bool;
}

const _bool CUI::Get_Vaild() const
{
	return m_Vaild;
}

void CUI::Set_AlphaTest(D3DCMPFUNC _func, _uint ref)
{
	m_desc.Func = _func;
	m_desc.Ref = ref;
}

HRESULT CUI::Set_Pos(_float fx, _float fy)
{
	m_desc.PosX = fx;
	m_desc.PosY = fy;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_desc.PosX - m_desc.SizeX * 0.5f, -m_desc.PosY + m_desc.SizeY * 0.5f, 0.f));

	return S_OK;
}

HRESULT CUI::Set_Size(_float sizeX, _float sizeY)
{
	m_desc.SizeX = sizeX;
	m_desc.SizeY = sizeY;

	m_pTransformCom->Scaled(_float3(m_desc.SizeX, m_desc.SizeY, 1.f));
	return S_OK;
}


HRESULT CUI::Bind_UI()	
{
	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	_float4x4		ViewMatrix;
	D3DXMatrixIdentity(&ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	return S_OK;
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
	Safe_Release(m_pShader);

}

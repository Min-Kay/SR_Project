#include "stdafx.h"
#include "..\Public\Impact.h"
#include "GameInstance.h"
#include "Shader.h"

CImpact::CImpact(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CImpact::CImpact(const CImpact & rhs)
	: CGameObject(rhs)
	,m_pTransformCom(rhs.m_pTransformCom)
	,m_pShader(rhs.m_pShader)
	,m_pVIBufferCom(rhs.m_pVIBufferCom)
	,m_pRendererCom(rhs.m_pRendererCom)
	,m_pTexture(rhs.m_pTexture)
{
	Safe_AddRef(m_pShader);
	Safe_AddRef(m_pTransformCom);
	Safe_AddRef(m_pVIBufferCom);
	Safe_AddRef(m_pRendererCom);
	Safe_AddRef(m_pTexture);

}

HRESULT CImpact::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CImpact::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_Impact = *static_cast<IMPACT*>(pArg);
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_Impact.Position);
	m_pTransformCom->Scaled(m_Impact.Size);

	mt19937 ranX(rd());
	mt19937 ranY(rd());
	mt19937 ranZ(rd());

	uniform_real_distribution <_double> spread(-m_Impact.RandomDirection, m_Impact.RandomDirection);

	_float randomPos[3];

	randomPos[0] = (_float)spread(ranX);
	randomPos[1] = (_float)spread(ranY);
	randomPos[2] = (_float)spread(ranZ);

	m_vDir = _float3((m_Impact.Position.x + randomPos[0]), (m_Impact.Position.y + randomPos[1]), (m_Impact.Position.z + (_float)spread(ranZ))) - m_Impact.Position;
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	m_CurrColor = m_Impact.Color;

	return S_OK;
}

_int CImpact::Tick(_float fTimeDelta)
{
	if (fTimeDelta <= 0.f)
		return 0;

	m_fTimer += fTimeDelta;

	if (m_Impact.DeleteTime <= m_fTimer)
		Set_Dead(true);

	if (Get_Dead())
		return 0;

	if (0 > __super::Tick(fTimeDelta))
		return -1;

	if(m_Impact.Change)
	{
		m_CurrColor.x = m_CurrColor.x < m_Impact.EndColor.x ? m_CurrColor.x + fTimeDelta : m_CurrColor.x - fTimeDelta;
		m_CurrColor.y = m_CurrColor.y < m_Impact.EndColor.y ? m_CurrColor.y + fTimeDelta : m_CurrColor.y - fTimeDelta;
		m_CurrColor.z = m_CurrColor.z < m_Impact.EndColor.z ? m_CurrColor.z + fTimeDelta : m_CurrColor.z - fTimeDelta;
		m_CurrColor.w = m_CurrColor.w < m_Impact.EndColor.w ? m_CurrColor.w + fTimeDelta : m_CurrColor.w - fTimeDelta;
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_vDir * m_Impact.SpreadSpeed * fTimeDelta);

	return _int();
}

_int CImpact::LateTick(_float fTimeDelta)
{
	if (Get_Dead())
		return 0;

	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CImpact::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	FaceOn_Camera();

	m_pTransformCom->Bind_OnShader(m_pShader);

	m_pShader->SetUp_ValueOnShader("g_ColorStack", &g_ControlShader, sizeof(_float));
	m_pShader->SetUp_ValueOnShader("g_Color", m_CurrColor, sizeof(_float4));
	m_pTexture->Bind_OnShader(m_pShader, "g_Texture", 0);
	m_pShader->Begin_Shader(SHADER_SETCOLOR);
	m_pVIBufferCom->Render();
	m_pShader->End_Shader();
	m_pShader->SetUp_ValueOnShader("g_Color", _float4(0.f,0.f,0.f,0.f), sizeof(_float4));

	return S_OK;
}



HRESULT CImpact::SetUp_Components()
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

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_SHADER_RECT, COM_SHADER, (CComponent**)&m_pShader)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Impact"), COM_TEXTURE, (CComponent**)&m_pTexture)))
		return E_FAIL;
	return S_OK;
}

HRESULT CImpact::FaceOn_Camera()
{
	CGameInstance* p_instance = GET_INSTANCE(CGameInstance);

	_float4x4		ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	_float3		vCamPosition = *(_float3*)&ViewMatrix.m[3][0];
	_float3		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3		vDir = vPosition - vCamPosition;
	_float		m_fCamDistance = D3DXVec3Length(&vDir);

	if (!m_pTarget)
	{
		m_pTarget = p_instance->Find_Camera_Object(MAIN_CAM)->Get_CameraTransform();
	}

	if (m_pTarget)
	{
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, m_pTarget->Get_State(CTransform::STATE_RIGHT) * m_pTransformCom->Get_Scale().x);
		m_pTransformCom->Set_State(CTransform::STATE_UP, m_pTarget->Get_State(CTransform::STATE_UP) * m_pTransformCom->Get_Scale().y);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_pTarget->Get_State(CTransform::STATE_LOOK) * m_pTransformCom->Get_Scale().z);
	}


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}


HRESULT CImpact::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;


	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CImpact::Release_RenderState()
{
	
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	return S_OK;
}


CImpact * CImpact::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CImpact*	pInstance = new CImpact(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CImpact");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CImpact::Clone(void* pArg)
{
	/* 새로운객체를 복제하여 생성한다. */
	CImpact*	pInstance = new CImpact(*this);


	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CImpact");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CImpact::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);

}

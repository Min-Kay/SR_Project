#include "stdafx.h"
#include "Shield_Effect.h"

#include "Transform.h"
#include "Renderer.h"
#include "VIBuffer_Cube.h"
#include "Texture.h"
#include "Shader.h"

CShield_Effect::CShield_Effect(LPDIRECT3DDEVICE9 m_pGraphic_Device)
	:CGameObject(m_pGraphic_Device)
{
}

CShield_Effect::CShield_Effect(const CShield_Effect& rhs)
	:CGameObject(rhs)
	,m_pTransform(rhs.m_pTransform)
	,m_pTexture(rhs.m_pTexture)
	,m_pRenderer(rhs.m_pRenderer)
	,m_pBuffer(rhs.m_pBuffer)
	,m_pShader(rhs.m_pShader)

{
	Safe_AddRef(m_pTransform);
	Safe_AddRef(m_pTexture);
	Safe_AddRef(m_pRenderer);
	Safe_AddRef(m_pBuffer);
	Safe_AddRef(m_pShader);
}

HRESULT CShield_Effect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	return S_OK; 
}

HRESULT CShield_Effect::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;


	CTransform::TRANSFORMDESC desc;
	desc.fRotationPerSec = 90.f;
	desc.fSpeedPerSec = 5.f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_TRANSFORM, COM_TRANSFORM, (CComponent**)&m_pTransform, &desc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Shield_Effect"), COM_TEXTURE, (CComponent**)&m_pTexture)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_CUBE, COM_BUFFER, (CComponent**)&m_pBuffer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_RENDERER, COM_RENDERER, (CComponent**)&m_pRenderer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTO_SHADER_CUBE, COM_SHADER, (CComponent**)&m_pShader)))
		return E_FAIL;


	Set_Type(OBJ_STATIC);

	return S_OK;
}

_int CShield_Effect::Tick(_float fTimeDelta)
{
	if (Get_Dead() || fTimeDelta <= 0.f)
		return 0;

	if(m_CoolTime <= 0.f)
		return 0;

	m_Timer += fTimeDelta;

	if (!m_CoolDown && m_CoolTime <= m_Timer)
	{
		m_CoolDown = true;
		m_Valid = true;
		m_Timer = 0.f;
	}

	if (!m_CoolDown)
		return 0;

	if (!m_Valid)
		return 0;

	if (0 > __super::Tick(fTimeDelta))
		return -1;

	if(!m_Split)
		Turning(fTimeDelta);
	else
		Split(fTimeDelta);

	return 0;
}

_int CShield_Effect::LateTick(_float fTimeDelta)
{
	if (!m_Valid)
		return 0;

	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return 0;
}

HRESULT CShield_Effect::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pTransform->Bind_OnShader(m_pShader);

	m_pShader->SetUp_ValueOnShader("g_ColorStack", &g_ControlShader, sizeof(_float));
	m_pShader->SetUp_ValueOnShader("g_Color", _float4(sinf(D3DXToRadian(m_Timer)), sinf(D3DXToRadian(m_Timer)), sinf(D3DXToRadian(m_Timer)), 1.f), sizeof(_float4));
	m_pTexture->Bind_OnShader(m_pShader, "g_Texture", 0);

	m_pShader->Begin_Shader(SHADER_SETCOLOR_CUBE);
	m_pBuffer->Render();
	m_pShader->End_Shader();
	m_pShader->SetUp_ValueOnShader("g_Color", _float4(0.f,0.f,0.f,0.f), sizeof(_float4));

	return S_OK; 
}

void CShield_Effect::Set_SpreadRange(_float _range)
{
	m_SpreadRange = _range;
}

void CShield_Effect::Set_Split(_bool _bool)
{
	m_Split = _bool;
	if (m_Split)
		m_Timer = 0.f;
}

void CShield_Effect::Set_TurnType(_bool _xz, _bool _y)
{
	m_TurnType = _xz;
	m_YType = _y;
}

void CShield_Effect::Set_Valid(_bool _bool)
{
	m_Valid = _bool;

}

void CShield_Effect::Set_CoolTime(_float _cool)
{
	m_CoolTime = _cool;
}

void CShield_Effect::Set_Parent(CTransform* _tr)
{
	m_pParent = _tr;
}

void CShield_Effect::Turning(_float fTimeDelta)
{
	m_Timer += fTimeDelta * 150.f;
	if (!m_pParent)
		return;

	_float3 vParentPos , vTurningPos;
	vParentPos = m_pParent->Get_State(CTransform::STATE_POSITION);

	_float radian = D3DXToRadian(m_Timer);

	vTurningPos.x = m_TurnType ? cosf(radian) * m_SpreadRange + vParentPos.x : sinf(radian) * m_SpreadRange + vParentPos.x;
	vTurningPos.y = m_YType? vParentPos.y : sinf(radian) * m_SpreadRange + vParentPos.y;
	vTurningPos.z = m_TurnType ? sinf(radian) * m_SpreadRange + vParentPos.z : cosf(radian) * m_SpreadRange + vParentPos.z;

	_float3 vRight, vUp, vLook, vScale;

	_float degree = m_Timer - 90.f;
	degree = D3DXToRadian(degree);

	vScale = m_pTransform->Get_Scale();
	vLook = m_pTransform->Get_State(CTransform::STATE_POSITION) - vParentPos;
	if(m_TurnType)
	{
		vRight.x = cosf(degree) * m_SpreadRange + vParentPos.x;
		vRight.y = m_YType ? vParentPos.y: sinf(degree) * m_SpreadRange + vParentPos.y;
		vRight.z = sinf(degree) * m_SpreadRange + vParentPos.z;
	}
	else
	{
		vRight.x = sinf(degree) * m_SpreadRange + vParentPos.x;
		vRight.y = m_YType ? vParentPos.y : sinf(degree) * m_SpreadRange + vParentPos.y;
		vRight.z = cosf(degree) * m_SpreadRange + vParentPos.z;
	}

	vRight = vRight - vParentPos;

	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);

	D3DXVec3Normalize(&vUp, &vUp);

	m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
	m_pTransform->Set_State(CTransform::STATE_UP, vUp * vScale.y);
	m_pTransform->Set_State(CTransform::STATE_LOOK, vLook * vScale.z);
	m_pTransform->Set_State(CTransform::STATE_POSITION, vTurningPos);

}

void CShield_Effect::Split(_float fTimeDelta)
{
	m_Timer += fTimeDelta;
	if (m_Timer >= 3.f)
		Set_Dead(true);

	m_pTransform->Go_Straight(fTimeDelta * 4.f);

}

CShield_Effect* CShield_Effect::Create(LPDIRECT3DDEVICE9 m_pGraphic_Device)
{
	CShield_Effect* p_effect = new CShield_Effect(m_pGraphic_Device);
	if(FAILED(p_effect->NativeConstruct_Prototype()))
	{
		Safe_Release(p_effect);
		return nullptr;
	}
	return p_effect;
}

CGameObject* CShield_Effect::Clone(void* pArg)
{
	CShield_Effect* p_effect = new CShield_Effect(*this);
	if (FAILED(p_effect->NativeConstruct(pArg)))
	{
		Safe_Release(p_effect);
		return nullptr;
	}
	return p_effect;
}

void CShield_Effect::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pShader);

}

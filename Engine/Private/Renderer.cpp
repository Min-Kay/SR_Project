#include "..\Public\Renderer.h"

#include "GameObject.h"
#include "Transform.h"

CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

HRESULT CRenderer::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CRenderer::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject * pRenderObject)
{
	if (eRenderGroup >= RENDER_END || 
		nullptr == pRenderObject)
		return E_FAIL;

	m_RenderObjects[eRenderGroup].push_back(pRenderObject);

	Safe_AddRef(pRenderObject);

	return S_OK;
}

HRESULT CRenderer::Render(vector<_uint> exceptions)
{
	for (_uint i = 0; i < RENDER_END; ++i)
	{
		auto ignore = find_if(exceptions.begin(), exceptions.end(), [i](_uint j) {return i == j; });

		if (ignore != exceptions.end())
			continue;

		switch (i)
		{
		case RENDER_ALPHA:
			Alpha_Sorting();
			break;
		case RENDER_UI:
			UI_Sorting();
			break;
		default:
			Render_GroupIndex((RENDERGROUP)i);
			break;
		}
	}
	return S_OK;
}

HRESULT CRenderer::Clear_RenderObjects()
{
	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for (auto& pRenderObject : m_RenderObjects[i])
		{
			Safe_Release(pRenderObject);
		}
		m_RenderObjects[i].clear();
	}

	return S_OK;
}

HRESULT CRenderer::Render_GroupIndex(RENDERGROUP eRenderGroup)
{
	for (auto& pRenderObject : m_RenderObjects[eRenderGroup])
	{
		if (nullptr != pRenderObject)
		{
			if (FAILED(pRenderObject->Render()))
				return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT CRenderer::Alpha_Sorting()
{
	_float4x4 ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	for (auto& pRenderObjects : m_RenderObjects[RENDER_ALPHA])
	{
		_float3 objPos = ((CTransform*)(pRenderObjects->Get_Component(TEXT("Com_Transform"))))->Get_State(CTransform::STATE_POSITION);
		pRenderObjects->Compute_Distance(objPos, *(_float3*)&ViewMatrix.m[3][0]);
	}

	m_RenderObjects[RENDER_ALPHA].sort([](CGameObject* a, CGameObject* b) -> _bool { return a->Get_Distance() > b->Get_Distance(); });

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	Render_GroupIndex(RENDER_ALPHA);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


	return S_OK;
}

HRESULT CRenderer::UI_Sorting()
{
	m_RenderObjects[RENDER_UI].sort([](CGameObject* a, CGameObject* b) -> _bool { return a->Get_Layer() > b->Get_Layer(); });

	Render_GroupIndex(RENDER_UI);

	return S_OK;
}

CRenderer * CRenderer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRenderer*	pInstance = new CRenderer(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRenderer::Clone(void * pArg)
{
	AddRef();
	return this;	
}

void CRenderer::Free()
{
	__super::Free();

	for (auto& RenderObjects : m_RenderObjects)
	{
		for (auto& pGameObject : RenderObjects)
			Safe_Release(pGameObject);
		RenderObjects.clear();
	}
}

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
	//for (_uint i = 0; i < RENDER_END; ++i)
	//{
	//	for (auto& pRenderObject : m_RenderObjects[i])
	//	{
	//		if (nullptr != pRenderObject)
	//		{
	//			if (FAILED(pRenderObject->Render()))
	//				return E_FAIL;
	//		}
	//		Safe_Release(pRenderObject);
	//	}
	//	m_RenderObjects[i].clear();
	//}

	for (_uint i = 0; i < RENDER_END; ++i)
	{
		auto ignore = find_if(exceptions.begin(), exceptions.end(), [i](_uint j) {return i == j; });

		if (ignore != exceptions.end())
			continue;

		if (i == RENDER_ALPHA)
			Alpha_Sorting();
	
		for (auto& pRenderObject : m_RenderObjects[i])
		{
			if (nullptr != pRenderObject)
			{
				if (FAILED(pRenderObject->Render()))
					return E_FAIL;
			}
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

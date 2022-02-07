#include "..\Public\Renderer.h"

#include "GameObject.h"


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

HRESULT CRenderer::Render()
{
	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for (auto& pRenderObject : m_RenderObjects[i])
		{
			if (nullptr != pRenderObject)
			{
				if (FAILED(pRenderObject->Render()))
					return E_FAIL;
			}
			Safe_Release(pRenderObject);
		}
		m_RenderObjects[i].clear();
	}

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

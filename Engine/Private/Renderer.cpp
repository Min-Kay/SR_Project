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
	//Make_Frustum();
	//Frustum_Culling();

	for (_uint i = 0; i < RENDER_END; ++i)
	{
		auto ignore = find_if(exceptions.begin(), exceptions.end(), [i](_uint j) {return i == j; });

		if (ignore != exceptions.end())
			continue;

		switch (i)
		{
		//case RENDER_ALPHA:
			//Alpha_Sorting();
			//break;
		case RENDER_UI:
			UI_Sorting();
			break;
		default:
			Render_GroupIndex((RENDERGROUP)i);
			break;
		}
	}

	//Clear_CullingRenderObjects();

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

HRESULT CRenderer::Clear_CullingRenderObjects()
{
	for(auto& RenderList : m_CullingRenderObjects)
	{
		RenderList.clear();
	}

	return S_OK;
}

void CRenderer::Initialize_Vertex()
{
	m_vtx[0].x = -1.0f;	m_vtx[0].y = -1.0f;	m_vtx[0].z = 0.0f;
	m_vtx[1].x = 1.0f;	m_vtx[1].y = -1.0f;	m_vtx[1].z = 0.0f;
	m_vtx[2].x = 1.0f;	m_vtx[2].y = -1.0f;	m_vtx[2].z = 1.0f;
	m_vtx[3].x = -1.0f;	m_vtx[3].y = -1.0f;	m_vtx[3].z = 1.0f;
	m_vtx[4].x = -1.0f;	m_vtx[4].y = 1.0f;	m_vtx[4].z = 0.0f;
	m_vtx[5].x = 1.0f;	m_vtx[5].y = 1.0f;	m_vtx[5].z = 0.0f;
	m_vtx[6].x = 1.0f;	m_vtx[6].y = 1.0f;	m_vtx[6].z = 1.0f;
	m_vtx[7].x = -1.0f;	m_vtx[7].y = 1.0f;	m_vtx[7].z = 1.0f;
}

HRESULT CRenderer::Make_Frustum()
{
	_float4x4		ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	Initialize_Vertex();

	for (auto i : m_vtx)
		D3DXVec3TransformCoord(&i, &i, &ViewMatrix);

	m_CurrCamPos = (m_vtx[0] + m_vtx[5]) / 2.0f;

	//D3DXPlaneFromPoints(&m_Plane[0], m_vtx+4, m_vtx+7, m_vtx+6);	// 상 평면(top)
	//D3DXPlaneFromPoints(&m_Plane[1], m_vtx , m_vtx+1, m_vtx+2);	// 하 평면(bottom)
	//D3DXPlaneFromPoints(&m_Plane[2], m_vtx , m_vtx+4, m_vtx+5);	// 근 평면(near)
	D3DXPlaneFromPoints(&m_Plane[3], m_vtx + 2, m_vtx + 6, m_vtx + 7);	// 원 평면(far)
	D3DXPlaneFromPoints(&m_Plane[4], m_vtx, m_vtx + 3, m_vtx + 7);	// 좌 평면(left)
	D3DXPlaneFromPoints(&m_Plane[5], m_vtx + 1, m_vtx + 5, m_vtx + 6);	// 우 평면(right)

	return S_OK;
}

HRESULT CRenderer::Frustum_Culling()
{
	CGameObject::OBJCULLING info;
	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for(auto& iter : m_RenderObjects[i])
		{
			info = iter->Get_CullingInfo();
			if (In_Frustum(info.Position,info.Radius))
				m_CullingRenderObjects[i].push_back(iter);
		}
	}

	return S_OK;
}

_bool CRenderer::In_Frustum(_float3 _pos , _float _radius)
{
	float		fDist;

	fDist = D3DXPlaneDotCoord(&m_Plane[3], &_pos);
	if (fDist > (_radius + PLANE_EPSILON)) return FALSE;	// 평면과 중심점의 거리가 반지름보다 크면 프러스텀에 없음
	fDist = D3DXPlaneDotCoord(&m_Plane[4], &_pos);
	if (fDist > (_radius + PLANE_EPSILON)) return FALSE;	// 평면과 중심점의 거리가 반지름보다 크면 프러스텀에 없음
	fDist = D3DXPlaneDotCoord(&m_Plane[5], &_pos);
	if (fDist > (_radius + PLANE_EPSILON)) return FALSE;	// 평면과 중심점의 거리가 반지름보다 크면 프러스텀에 없음

	/*for(auto plane : m_Plane)
	{
		fDist = D3DXPlaneDotCoord(&plane, &_pos);
		if (fDist > _radius + PLANE_EPSILON) 
			return FALSE;
	}*/

	return true;
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
		_float3 objPos = ((CTransform*)(pRenderObjects->Get_Component(COM_TRANSFORM)))->Get_State(CTransform::STATE_POSITION);
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

	for (auto& RenderObjects : m_CullingRenderObjects)
	{
		RenderObjects.clear();
	}

	for (auto& RenderObjects : m_RenderObjects)
	{
		for (auto& pGameObject : RenderObjects)
			Safe_Release(pGameObject);
		RenderObjects.clear();
	}

}

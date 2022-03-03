#pragma once

#include "Component.h"

/* �ΰ��ӳ��� �׷������ϴ� ��ü���� ��Ƽ� ����(�׷����� �������.) �Ѵ�. */
/* ��ü���� �׸��� �Լ�(Render) �� ȣ�����ֳ�. */

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{	
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_SKYBOX, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };
public:
	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CRenderer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	HRESULT Render(vector<_uint> exceptions);

	HRESULT Clear_RenderObjects();
	HRESULT Clear_CullingRenderObjects();



private:
	void Initialize_Vertex();
	HRESULT Make_Frustum();
	HRESULT Frustum_Culling();
	_bool In_Frustum(_float3 _pos, _float _radius);

private:
	HRESULT Render_GroupIndex(RENDERGROUP eRenderGroup);
	HRESULT Alpha_Sorting();
	HRESULT UI_Sorting();

private:
	list<class CGameObject*>				m_RenderObjects[RENDER_END];
	list<class CGameObject*>				m_CullingRenderObjects[RENDER_END];

	typedef	list <class CGameObject* >		RENDEROBJECTS;

private:
	_float3	m_vtx[8];
	_float3 m_CurrCamPos;
	D3DXPLANE m_Plane[6];

	_float PLANE_EPSILON = 5.0f;

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free();
};

END
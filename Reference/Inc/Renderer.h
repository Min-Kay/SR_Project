#pragma once

#include "Component.h"

/* 인게임내에 그려져야하는 객체들을 모아서 보관(그려지는 순서대로.) 한다. */
/* 객체들의 그리는 함수(Render) 를 호출해주낟. */

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{	
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };
public:
	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CRenderer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	HRESULT Render();

private:
	list<class CGameObject*>				m_RenderObjects[RENDER_END];
	typedef	list <class CGameObject* >		RENDEROBJECTS;

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free();
};

END
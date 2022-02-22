
// ToolView.h : CToolView 클래스의 인터페이스
//

#pragma once

#include "Tool_Define.h"
#include "GameInstance.h"
#include "Renderer.h"
#include "Terrain.h"


class CToolDoc;
class CToolView : public CView

{
protected: // serialization에서만 만들어집니다.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

	// 특성입니다.
public:
	CToolDoc* GetDocument() const;

	// 작업입니다.
public:

	// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	_int Tick(_float fTimeDelta); /* 갱신한다. */
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);


	// 구현입니다.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

public:
	LPDIRECT3DDEVICE9	m_device = nullptr;
	CRenderer*			m_pRenderer = nullptr;
	CGameInstance*		m_pGameInstance = nullptr;

public:

	CTerrain*			Get_Terrain(void) { return m_pTerrain; }

	CTerrain*			m_pTerrain = nullptr;

private:
	_bool	m_bReady;
public:
	//POINT m_MousePos_ToolView;
	HRESULT DefaultSetting();
	HRESULT Ready_Prototype_GameObject();
	HRESULT Ready_Prototype_Component();

public:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Terrain(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Cube(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);

	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
{
	return reinterpret_cast<CToolDoc*>(m_pDocument);
}
#endif


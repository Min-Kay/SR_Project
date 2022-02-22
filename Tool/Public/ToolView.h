
// ToolView.h : CToolView Ŭ������ �������̽�
//

#pragma once

#include "Tool_Define.h"
#include "GameInstance.h"
#include "Renderer.h"
#include "Terrain.h"


class CToolDoc;
class CToolView : public CView

{
protected: // serialization������ ��������ϴ�.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

	// Ư���Դϴ�.
public:
	CToolDoc* GetDocument() const;

	// �۾��Դϴ�.
public:

	// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	_int Tick(_float fTimeDelta); /* �����Ѵ�. */
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);


	// �����Դϴ�.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// ������ �޽��� �� �Լ�
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

#ifndef _DEBUG  // ToolView.cpp�� ����� ����
inline CToolDoc* CToolView::GetDocument() const
{
	return reinterpret_cast<CToolDoc*>(m_pDocument);
}
#endif


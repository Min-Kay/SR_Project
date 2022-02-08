#pragma once

#include "Tool_Include.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
END

class CToolView;
class CTerrain : public CGameObject
{
protected:
	CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_float fTimeDelta);
	virtual _int LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT MiniRender();

public:
	vector<TILE*>&		Get_Tile(void) { return m_vecTile; }
	void	TileChange(const D3DXVECTOR3& vPos, const int& iTileIdx);
	void	SetMainView(CToolView* pMainView) { m_pMainView = pMainView;  }

private:
	int		GetTileIndex(const D3DXVECTOR3& vPos);
	// bool	Picking(const D3DXVECTOR3& vPos, const int& iIndex); // 직선의 방정식
	bool	Picking(const D3DXVECTOR3& vPos, const int& iIndex);

	void	SetRatio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);
public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;

private:
	vector<TILE*>		m_vecTile;
	CToolView*			m_pMainView = nullptr;
	CRenderer*			m_pRendererCom = nullptr;


private:
	// CGameObject을(를) 통해 상속됨
	virtual CGameObject* Clone(void* pArg) override;

};


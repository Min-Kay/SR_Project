#pragma once

#include "Tool_Include.h"
#include "Base.h"

class CToolView;
class CTerrain : public CBase
{
protected:
	CTerrain();
	virtual ~CTerrain() = default;

public:
	HRESULT		Initialize(void);
	void		Update(void);
	void		MiniRender(void);
	void		Render(void);

	vector<TILE*>&		Get_Tile(void) { return m_vecTile; }

	void	TileChange(const D3DXVECTOR3& vPos, const int& iTileIdx);

	void	SetMainView(CToolView* pMainView) { m_pMainView = pMainView;  }

public:
	static CTerrain* Create();
	virtual void Free() override;

 private:
	int		GetTileIndex(const D3DXVECTOR3& vPos);
	// bool	Picking(const D3DXVECTOR3& vPos, const int& iIndex); // 직선의 방정식
	bool	Picking(const D3DXVECTOR3& vPos, const int& iIndex);

	void	SetRatio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);

private:
	vector<TILE*>		m_vecTile;
	CToolView*			m_pMainView = nullptr;

};


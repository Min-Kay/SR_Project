#pragma once

namespace Tool
{
	
	typedef struct tagTile
	{
		D3DXVECTOR3	vPos;
		D3DXVECTOR3 vSize;

		BYTE		byOption;
		BYTE		byTileIdx;

		// ���� Ÿ���� �ε���
		int			iIndex;
		// �θ� Ÿ���� �ε���
		int			iParentIndex;

	}TILE;

	typedef struct tagUnitData
	{
#ifndef _AFX
		wstring			strName;
#else
		CString		strName;
#endif

		int			iAttack;
		int			iDef;
		BYTE		byJobIndex;
		BYTE		byItem;

	}UNITDATA;

}

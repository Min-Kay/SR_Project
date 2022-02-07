#pragma once

namespace Tool
{
	
	typedef struct tagTile
	{
		D3DXVECTOR3	vPos;
		D3DXVECTOR3 vSize;

		BYTE		byOption;
		BYTE		byTileIdx;

		// 현재 타일의 인덱스
		int			iIndex;
		// 부모 타일의 인덱스
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

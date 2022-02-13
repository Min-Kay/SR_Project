#pragma once

namespace Engine
{
	/*
	D3DDECLUSAGE_POSITION = 0,
	D3DDECLUSAGE_BLENDWEIGHT,   // 1
	D3DDECLUSAGE_BLENDINDICES,  // 2
	D3DDECLUSAGE_NORMAL,        // 3
	D3DDECLUSAGE_PSIZE,         // 4
	D3DDECLUSAGE_TEXCOORD,      // 5
	D3DDECLUSAGE_TANGENT,       // 6
	D3DDECLUSAGE_BINORMAL,      // 7
	D3DDECLUSAGE_TESSFACTOR,    // 8
	D3DDECLUSAGE_POSITIONT,     // 9
	D3DDECLUSAGE_COLOR,         // 10
	D3DDECLUSAGE_FOG,           // 11
	D3DDECLUSAGE_DEPTH,         // 12
	D3DDECLUSAGE_SAMPLE,        // 13
	*/
	typedef struct tagVertex_Texture
	{
		D3DXVECTOR3			vPosition;
		D3DXVECTOR2			vTexUV;
	}VTXTEX;

	typedef struct tagLineIndices32
	{
		unsigned long	_0, _1;
	}LINEINDICES32;


	typedef struct tagLineIndices16
	{
		unsigned short	_0, _1;
	}LINEINDICES16;

	typedef struct tagFaceIndices32
	{
		unsigned long	_0, _1, _2;
	}FACEINDICES32;


	typedef struct tagFaceIndices16
	{
		unsigned short	_0, _1, _2;
	}FACEINDICES16;


	typedef struct tagInfo
	{
		D3DXVECTOR3	vPos;
		D3DXVECTOR3	vDir;
		D3DXVECTOR3 vLook;
		D3DXMATRIX	matWorld;

	}INFO;

	typedef struct tagTexture
	{
		// �̹��� �� ���� �����ϴ� com��ü
		LPDIRECT3DTEXTURE9		pTexture;

		// �̹��� ������ �����ϱ� ���� ����ü
		D3DXIMAGE_INFO			tImgInfo;

	}TEXINFO;

	typedef struct tagTexturePath
	{
		wstring			wstrObjKey = L"";
		wstring			wstrStateKey = L"";
		wstring			wstrPath = L"";
		int				iCount = 0;

	}IMGPATH;

	typedef struct tagFrame
	{
		tagFrame() {}
		tagFrame(float _fFrame, float _fMax)
			: fFrame(_fFrame), fMax(_fMax)
		{}

		float   fFrame;	// ���� ��µǴ� �̹��� �ε���
		float	fMax;	// �ִ� �̹��� ���
	}FRAME;
}

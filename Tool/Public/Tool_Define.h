#pragma once

static const unsigned int g_iWinCX = 1280;
static const unsigned int g_iWinCY = 720;

#define		TILECX	130
#define		TILECY	68

#define		TILEX	20
#define		TILEY	30

#define		RUBY		0x01		// -> 0001
#define		DIAMOND		0X02		// -> 0010
#define		SAPPHIRE	0x04		// -> 0100

#define		MIN_STR		64
#define		MAX_STR		256

extern HINSTANCE	g_hInst;
extern HWND g_hWnd;

namespace Tool {}
using namespace Tool; 


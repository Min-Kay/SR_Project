#pragma once

static const unsigned int g_iWinCX = 1280;
static const unsigned int g_iWinCY = 720;

enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_BOSS, LEVEL_END };

extern HINSTANCE	g_hInst;
extern HWND		g_hWnd;

namespace Client {}

using namespace Client;


#pragma once

static const unsigned int g_iWinCX = 1280;
static const unsigned int g_iWinCY = 720;

enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_BOSS, LEVEL_END };

extern HINSTANCE	g_hInst;
extern HWND		g_hWnd;


#define CAM_STATIC TEXT("Prototype_GameObject_Camera_Static")
#define CAM_DYNAMIC TEXT("Prototype_GameObject_Camera_Dynamic")
#define CAM_PORTAL TEXT("Prototype_GameObject_Camera_Portal")


#define PROTO_UI TEXT("Prototype_GameObject_UI")


namespace Client {}

using namespace Client;


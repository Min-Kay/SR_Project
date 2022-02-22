#pragma once

static const unsigned int g_iWinCX = 1280;
static const unsigned int g_iWinCY = 720;

enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_BOSS, LEVEL_END };

extern HINSTANCE	g_hInst;
extern HWND		g_hWnd;


#define MAIN_CAM TEXT("Main_Camera")

#define CAM_STATIC TEXT("Prototype_GameObject_Camera_Static")
#define CAM_DYNAMIC TEXT("Prototype_GameObject_Camera_Dynamic")
#define CAM_PORTAL TEXT("Prototype_GameObject_Camera_Portal")
#define CAM_PLAYER TEXT("Prototype_GameObject_Camera_Player")


#define PROTO_UI TEXT("Prototype_GameObject_UI")
#define PROTO_BUTTON TEXT("Prototype_GameObject_Button")

#define	GRAVITY float -9.8f;

namespace Client {}

using namespace Client;


#pragma once

static const unsigned int g_iWinCX = 1280;
static const unsigned int g_iWinCY = 720;

enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_BOSS, LEVEL_END };

extern HINSTANCE	g_hInst;
extern HWND		g_hWnd;

#define COM_TEXTURE TEXT("Com_Texture")
#define COM_TRANSFORM TEXT("Com_Transform")
#define COM_RENDERER TEXT("Com_Renderer")
#define COM_BUFFER TEXT("Com_VIBuffer")


#define PROTO_TRANSFORM TEXT("Prototype_Component_Transform")
#define PROTO_RENDERER TEXT("Prototype_Component_Renderer")
#define PROTO_RECT TEXT("Prototype_Component_VIBuffer_Rect")
#define PROTO_PORTAL TEXT("Prototype_Component_VIBuffer_Portal")
#define PROTO_CUBE TEXT("Prototype_Component_VIBuffer_Cube")


#define CAM_STATIC TEXT("Prototype_GameObject_Camera_Static")
#define CAM_DYNAMIC TEXT("Prototype_GameObject_Camera_Dynamic")
#define CAM_PORTAL TEXT("Prototype_GameObject_Camera_Portal")


#define PROTO_UI TEXT("Prototype_GameObject_UI")


namespace Client {}

using namespace Client;


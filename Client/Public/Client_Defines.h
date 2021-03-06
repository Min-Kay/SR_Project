#pragma once

static const unsigned int g_iWinCX = 1280;
static const unsigned int g_iWinCY = 720;

enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_STAGEONE,LEVEL_STAGETWO, LEVEL_ENDING, LEVEL_END };
enum SHADER_RECT { SHADER_NONE, SHADER_SETCOLOR, SHADER_GROWCOLOR, SHADER_GRAYCOLOR , SHADER_NONE_BLEND, SHADER_SETCOLOR_BLEND, SHADER_GROWCOLOR_BLEND, SHADER_GRAYCOLOR_BLEND};
enum SHADER_CUBE
{
	SHADER_NONE_CUBE, SHADER_SETCOLOR_CUBE, SHADER_GROWCOLOR_CUBE, SHADER_GRAYCOLOR_CUBE, SHADER_NONE_BLEND_CUBE, SHADER_SETCOLOR_BLEND_CUBE, SHADER_GROWCOLOR_BLEND_CUBE, SHADER_GRAYCOLOR_BLEND_CUBE, SHADER_SKYBOX
};
extern HINSTANCE	g_hInst;
extern HWND		g_hWnd;
extern LEVEL	g_CurrLevel;
extern float     g_ControlTime;
extern float	g_ControlShader;


#define MAIN_CAM TEXT("Main_Camera")

#define CAM_STATIC TEXT("Prototype_GameObject_Camera_Static")
#define CAM_DYNAMIC TEXT("Prototype_GameObject_Camera_Dynamic")
#define CAM_PORTAL TEXT("Prototype_GameObject_Camera_Portal")
#define CAM_PLAYER TEXT("Prototype_GameObject_Camera_Player")

#define	GRAVITY float -9.8f;

namespace Client {}

using namespace Client;


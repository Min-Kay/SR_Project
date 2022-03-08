#pragma once

#pragma warning (disable : 4251)

#include <d3d9.h>
#include <d3dx9.h>
#include <typeinfo>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <process.h>

#include <vector>
#include <list>
#include <map>
#include <string>
#include <algorithm>
using namespace std;

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG

#include "Engine_Macro.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "Engine_Struct.h"


#include <io.h>
#include "../../fmod/64/inc/fmod.h"
#pragma comment(lib, "../../fmod/64/fmod_vc.lib")


#define COM_TEXTURE TEXT("Com_Texture")
#define COM_TRANSFORM TEXT("Com_Transform")
#define COM_RENDERER TEXT("Com_Renderer")
#define COM_BUFFER TEXT("Com_VIBuffer")
#define COM_COLLIDER TEXT("Com_Collider")
#define COM_SHADER TEXT("Com_Shader")


#define PROTO_TRANSFORM TEXT("Prototype_Component_Transform")
#define PROTO_RENDERER TEXT("Prototype_Component_Renderer")

#define PROTO_RECT TEXT("Prototype_Component_VIBuffer_Rect")
#define PROTO_COLOR TEXT("Prototype_Component_VIBuffer_Color")

#define PROTO_PORTAL TEXT("Prototype_Component_VIBuffer_Portal")
#define PROTO_CUBE TEXT("Prototype_Component_VIBuffer_Cube")
#define PROTO_TERRAIN TEXT("Prototype_Component_VIBuffr_Terrain")

#define PROTO_COLLIDER TEXT("Prototype_Component_Collider")
#define PROTO_SHADER_RECT TEXT("Prototype_Component_Shader_Rect")
#define PROTO_SHADER_CUBE TEXT("Prototype_Component_Shader_Cube")

#define PROTO_UI TEXT("Prototype_GameObject_UI")
#define PROTO_BUTTON TEXT("Prototype_GameObject_Button")
#define PROTO_EFFECT TEXT("Prototype_GameObject_Effect")

namespace Engine {}
using namespace Engine;

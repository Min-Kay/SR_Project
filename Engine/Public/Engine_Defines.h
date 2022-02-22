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


#define COM_TEXTURE TEXT("Com_Texture")
#define COM_TRANSFORM TEXT("Com_Transform")
#define COM_RENDERER TEXT("Com_Renderer")
#define COM_BUFFER TEXT("Com_VIBuffer")
#define COM_COLLIDER TEXT("Com_Collider")

#define PROTO_TRANSFORM TEXT("Prototype_Component_Transform")
#define PROTO_RENDERER TEXT("Prototype_Component_Renderer")

#define PROTO_RECT TEXT("Prototype_Component_VIBuffer_Rect")
#define PROTO_PORTAL TEXT("Prototype_Component_VIBuffer_Portal")
#define PROTO_CUBE TEXT("Prototype_Component_VIBuffer_Cube")
#define PROTO_TERRAIN TEXT("Prototype_Component_VIBuffr_Terrain")

#define PROTO_COLLIDER TEXT("Prototype_Component_Collider")



using namespace Engine;

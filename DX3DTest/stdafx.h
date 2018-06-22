// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here
#include <vector>
#include <map>
#include <set>
#include <list>
#include <string>
#include <assert.h>
#include <fstream>
#include <atlstr.h>
#include <unordered_map>
#include <unordered_set>

using namespace std;
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


#include "./system/DX.h"
#include "./system/Debug.h"
#include "./system/Mouse.h"
#include "./system/Keyboard.h"
#include "./system/Time.h"
#include "./system/Camera.h"

#include "GlobalDefinition.h"
#include "./object/interface/IObject.h"
#include "./object/interface/IDisplayObject.h"
#include "./object/interface/IUnitObject.h"
#include "./object/interface/IScene.h"
#include "./object/interface/IMap.h"
#include "./object/action/IAction.h"
#include "./object/action/IActionObject.h"

#include "ParticleSystem.h"
#include "./manager/ObjectManager.h"
#include "./manager/MapManager.h"
#include "./manager/SceneManager.h"
#include "./manager/TextureManager.h"
#include "./manager/FontManager.h"
#include "./manager/ShaderManager.h"
#include "ImGuiManager.h"
#include "ResourceManager.h"


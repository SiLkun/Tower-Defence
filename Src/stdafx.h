#pragma once

////////////////////////////////////////////////////////////////////////////////
// Filename: stdafx.h
////////////////////////////////////////////////////////////////////////////////
#ifndef STDAFX_H_
#define STDAFX_H_

//////////////
// INCLUDES //
//////////////

#include <dxgi.h>
#include <d3d11.h>
#include <d3d10.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include <vector>
#include <list>
#include <windows.h>
#include <dinput.h>
#include <pdh.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <process.h>

using namespace std;

/////////////
// LINKING //
/////////////
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "pdh.lib")
#pragma comment(lib, "winmm.lib")





namespace TD
{
	using namespace TD;

	#include "Texture.h"
	#include "Font.h"
	#include "Fontshader.h"
	#include "Gui.h"
	#include "Cpu.h"
	#include "Fps.h"
	#include "Timer.h"
	#include "Sound.h"
	#include "Lightshader.h"
	#include "Light.h"
	#include "Terrainshader.h"
	#include "Terrain.h"
	#include "Camera.h"
	#include "Input.h"
	#include "Mesh.h"
	#include "Model.h"
	#include "Projectile.h"
	#include "Creeper.h"
	#include "Tower.h"
	#include "Game.h"
	#include "D3D.h"
	#include "Engine.h"
	#include "System.h"
	
	

	

}

#endif



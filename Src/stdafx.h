#pragma once

////////////////////////////////////////////////////////////////////////////////
// Filename: stdafx.h
////////////////////////////////////////////////////////////////////////////////
#ifndef STDAFX_H_
#define STDAFX_H_


//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include <vector>

using namespace std;
/////////////
// LINKING //
/////////////
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")



namespace TD
{
	using namespace TD;

	#include "Tower.h"
	#include "Creeper.h"
	#include "Game.h"
	#include "Application.h"
	#include "System.h"
}

#endif



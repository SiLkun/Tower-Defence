////////////////////////////////////////////////////////////////////////////////
// Filename: application.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _APPLICATION_H_
#define _APPLICATION_H_


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "stdafx.h"

using namespace TD;

////////////////////////////////////////////////////////////////////////////////
// Class name: Engine
////////////////////////////////////////////////////////////////////////////////
class Engine
{
public:
	Engine();
	Engine(const Engine&);
	~Engine();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();


private:
	bool HandleInput(float);
	bool RenderGraphics();

private:
	Game* pGame;
	D3D* pDirect3D;
	Input* pInput;
	Camera* pCamera;
	Timer* pTimer;
	Fps* pFps;
	Cpu* pCpu;
	FontShader* pFontShader;
	Text* pText;
	int screenWidth, screenHeight;
};

#endif
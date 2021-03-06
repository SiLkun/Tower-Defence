////////////////////////////////////////////////////////////////////////////////
// Filename: Application.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{

	Engine::Engine()
	{
		pGui = 0;
		pGame = 0;
		pDirect3D = 0;
		pInput = 0;
		pCamera = 0;
		pTimer = 0;
		pFps = 0;
		pCpu = 0;
		pFontShader = 0;
	}

	Engine::Engine(const Engine&)
	{
	}

	Engine::~Engine()
	{
	}

	bool Engine::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
	{
		bool result;
		D3DXMATRIX baseViewMatrix;

		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		// Create the input object.  The input object will be used to handle reading the pKeyboard and pMouse input from the user.
		pInput = new Input;
		if(!pInput)
		{
			return false;
		}
		
		// Initialize the input object.
		result = pInput->Initialize(hinstance, hwnd, screenWidth, screenHeight);
		if(!result)
		{
			MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
			return false;
		}
		
		// Create the Direct3D object.
		pDirect3D = new D3D;
		if(!pDirect3D)
		{
			return false;
		}

		// Initialize the Direct3D object.
		result = pDirect3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
		if(!result)
		{
			MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
			return false;
		}
		
		// Create the camera object.
		pCamera = new Camera;
		if(!pCamera)
		{
			return false;
		}

		// Initialize a base view matrix with the camera for 2D user interface rendering.
		pCamera->SetPosition( D3DXVECTOR3(0.0f, 0.0f, -1.0f));
		pCamera->Render();
		pCamera->GetViewMatrix(baseViewMatrix);

		pCamera->SetPosition(D3DXVECTOR3(30.0f, 15.0f, -30.0f));
		pCamera->SetRotation(D3DXVECTOR3((float)D3DX_PI /8.0f , (float)-D3DX_PI /4.0f , 0.0f));

	
		// Create the timer object.
		pTimer = new Timer;
		if(!pTimer)
		{
			return false;
		}

		// Initialize the timer object.
		result = pTimer->Initialize();
		if(!result)
		{
			MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
			return false;
		}

		// Create the fps object.
		pFps = new Fps;
		if(!pFps)
		{
			return false;
		}

		// Initialize the fps object.
		pFps->Initialize();

		// Create the cpu object.
		pCpu = new Cpu;
		if(!pCpu)
		{
			return false;
		}

		// Initialize the cpu object.
		pCpu->Initialize();

		// Create the font shader object.
		pFontShader = new FontShader;
		if(!pFontShader)
		{
			return false;
		}

		// Initialize the font shader object.
		result = pFontShader->Initialize(pDirect3D->GetDevice(), hwnd);
		if(!result)
		{
			MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
			return false;
		}

		// Create the text object.
		pGui = new Gui;
		if(!pGui)
		{
			return false;
		}

		// Initialize the text object.
		result = pGui->Initialize(pDirect3D->GetDevice(), pDirect3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
		if(!result)
		{
			MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
			return false;
		}

		this->hwnd = hwnd;

		unsigned ret;
		_beginthreadex(0,0,::TD::Engine::ThreadLoadProject,this, 0,&ret);

		return true;
	}

	unsigned int __stdcall Engine::ThreadLoadProject(void *pvParam)
	{
		bool result;
		Engine * pEngine = (Engine * )pvParam;

		pEngine->pGame = new Game;
		if(!pEngine->pGame)
		{
			return false;
		}


		// Initialize the game object.
		result = pEngine->pGame->Initialize(pEngine->pDirect3D->GetDevice(),pEngine->hwnd);
		if(!result)
		{
			return 1;
		}
		return 0;
	}

	void Engine::Release()
	{

		// Release the text object.
		if(pGui)
		{
			pGui->Release();
			delete pGui;
			pGui = 0;
		}

		// Release the font shader object.
		if(pFontShader)
		{
			pFontShader->Release();
			delete pFontShader;
			pFontShader = 0;
		}

		// Release the cpu object.
		if(pCpu)
		{
			pCpu->Release();
			delete pCpu;
			pCpu = 0;
		}

		// Release the fps object.
		if(pFps)
		{
			delete pFps;
			pFps = 0;
		}

		// Release the timer object.
		if(pTimer)
		{
			delete pTimer;
			pTimer = 0;
		}
		
		// Release the terrain object.
		if(pGame)
		{
			pGame->Release();
			delete pGame;
			pGame = 0;
		}

		// Release the camera object.
		if(pCamera)
		{
			delete pCamera;
			pCamera = 0;
		}

		// Release the Direct3D object.
		if(pDirect3D)
		{
			pDirect3D->Release();
			delete pDirect3D;
			pDirect3D = 0;
		}

		// Release the input object.
		if(pInput)
		{
			pInput->Release();
			delete pInput;
			pInput = 0;
		}

		return;
	}


	bool Engine::Frame()
	{
		bool result;


		// Read the user input.
		result = pInput->Frame();
		if(!result)
		{
			return false;
		}
	
		// Check if the user pressed escape and wants to exit the application.
		if(pInput->IsKeyPressed(DIK_ESCAPE) == true)
		{
			return false;
		}

		// Update the system stats.
		pTimer->Frame();
		pFps->Frame();
		pCpu->Frame();

		// Update the FPS value in the text object.
		result = pGui->SetFps(pFps->GetFps(), pDirect3D->GetDeviceContext());
		if(!result)
		{
			return false;
		}
	
		// Update the CPU usage value in the text object.
		result = pGui->SetCpu(pCpu->GetCpuPercentage(), pDirect3D->GetDeviceContext());
		if(!result)
		{
			return false;
		}
		
		if(pGame)
		{
			// Update the CPU usage value in the text object.
			result = pGui->SetTime((int)pGame->GetTime(), pDirect3D->GetDeviceContext());
			if(!result)
			{
				return false;
			}

			// Update the CPU usage value in the text object.
			result = pGui->SetLevel((int)pGame->GetLevel() - 1, pDirect3D->GetDeviceContext());
			if(!result)
			{
				return false;
			}

			// Update the CPU usage value in the text object.
			result = pGui->SetGold((int)pGame->GetGold(), pDirect3D->GetDeviceContext());
			if(!result)
			{
				return false;
			}

			result = pGui->SetLivesLeft((int)pGame->GetLivesLeft(), pDirect3D->GetDeviceContext());
			if(!result)
			{
				return false;
			}
		


			int time = pGame->GetWaveDelay() - (pGame->GetTime() - pGame->GetPreviousWaveTime());
			result = pGui->SetNextWaveTime(time, pDirect3D->GetDeviceContext());
			if(!result)
			{
				return false;
			}
		}

		// Do the frame input processing.
		result = HandleInput(pTimer->GetTime());
		if(!result)
		{
			return false;
		}

		// Render the graphics.
		result = RenderGraphics();
		if(!result)
		{
			return false;
		}

		return result;
	}

	
	bool Engine::HandleInput(float frameTime)
	{
		// Set the frame time for calculating the updated position.
		pCamera->SetFrameTime(frameTime);

		D3DXVECTOR3 move(0,0,0);
		D3DXVECTOR3 rotate(0,0,0);

		// Handle the input.
		if(pInput->IsKeyPressed(DIK_LEFT) || pInput->IsKeyPressed(DIK_A))
			move.x = -1.0f;

		if(pInput->IsKeyPressed(DIK_RIGHT)|| pInput->IsKeyPressed(DIK_D))
			move.x = 1.0f;

		if(pInput->IsKeyPressed(DIK_UP)|| pInput->IsKeyPressed(DIK_W))
			move.z = 1.0f;

		if(pInput->IsKeyPressed(DIK_DOWN)|| pInput->IsKeyPressed(DIK_S))
			move.z = -1.0f;
	

	
		POINT point;
		GetCursorPos(&point);
		
		// Move the mouse cursor coordinates into the -1 to +1 range.
		float pointX = ((2.0f * (float)point.x) / (float)screenWidth) - 1.0f;
		float pointY = (((2.0f * (float)point.y) / (float)screenHeight) - 1.0f) * -1.0f;
		
		D3DXMATRIX viewMatrix,projectionMatrix;
		pCamera->GetViewMatrix(viewMatrix);
		pDirect3D->GetProjectionMatrix(projectionMatrix);

		// Adjust the points using the projection matrix to account for the aspect ratio of the viewport.
		pointX = pointX / projectionMatrix._11;
		pointY = pointY / projectionMatrix._22;

		D3DXVECTOR3 p;
		pCamera->GetPosition(p);

		if(pGame)
		{
			pGame->MouseLeftMove(pInput->IsMouseLeftPressed(),pointX,pointY,p,viewMatrix);
		}
	



		if(pInput->IsMouseRightPressed())
		{
			int pMouseRightDownX, pMouseRightDownY,pMouseX, pMouseY;
			pInput->GetMouseRightDownLocation(pMouseRightDownX, pMouseRightDownY);
			pInput->GetMouseLocation(pMouseX, pMouseY);

			int mouseMovementX = pMouseX - pMouseRightDownX;
			int mouseMovementY = pMouseY - pMouseRightDownY;

			rotate.x = (float)mouseMovementY;
			rotate.y = (float)mouseMovementX;
		}
		pCamera->Move(move);
		pCamera->Rotate(rotate);
		return true;
	}


	bool Engine::RenderGraphics()
	{
		D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
		bool result;


		// Clear the scene.
		pDirect3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

		// Generate the view matrix based on the camera's position.
		pCamera->Render();

		// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
		pDirect3D->GetWorldMatrix(worldMatrix);
		pCamera->GetViewMatrix(viewMatrix);
		pDirect3D->GetProjectionMatrix(projectionMatrix);
		pDirect3D->GetOrthoMatrix(orthoMatrix);

		if(pGame)
		{
			pGame->Update(pDirect3D->GetDevice(),pCamera,pTimer->GetTime());
			pGame->Render(pDirect3D->GetDeviceContext(), viewMatrix, projectionMatrix);
		}
		// Turn off the Z buffer to begin all 2D rendering.
		pDirect3D->TurnZBufferOff();
		
		// Turn on the alpha blending before rendering the text.
		pDirect3D->TurnOnAlphaBlending();

		if(pGame && pGame->IsLoaded())
		{
			if(pGame->IsLost())
			{
				result = pGui->RenderGameOver(pDirect3D->GetDeviceContext(), worldMatrix, orthoMatrix);
				if(!result)
				{
					return false;
				}
			}
			else
			{
				// Render the text user interface elements.
				result = pGui->Render(pDirect3D->GetDeviceContext(), worldMatrix, orthoMatrix);
				if(!result)
				{
					return false;
				}
			}
		}
		else
		{
			result = pGui->RenderLoadScreen(pDirect3D->GetDeviceContext(), worldMatrix, orthoMatrix);
			if(!result)
			{
				return false;
			}
		}

		// Turn off alpha blending after rendering the text.
		pDirect3D->TurnOffAlphaBlending();

		// Turn the Z buffer back on now that all 2D rendering has completed.
		pDirect3D->TurnZBufferOn();

		// Present the rendered scene to the screen.
		pDirect3D->EndScene();

		return true;
	}



}
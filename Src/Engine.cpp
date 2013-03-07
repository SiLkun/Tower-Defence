////////////////////////////////////////////////////////////////////////////////
// Filename: Application.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{

	Engine::Engine()
	{
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
		float cameraX, cameraY, cameraZ;
		D3DXMATRIX baseViewMatrix;
		char videoCard[128];
		int videoMemory;

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
		pCamera->SetPosition(0.0f, 0.0f, -1.0f);
		pCamera->Render();
		pCamera->GetViewMatrix(baseViewMatrix);

		// Set the initial position of the camera.
		cameraX = 50.0f;
		cameraY = 2.0f;
		cameraZ = -7.0f;

		pCamera->SetPosition(cameraX, cameraY, cameraZ);
		
		// Create the terrain object.
		pTerrain = new Terrain;
		if(!pTerrain)
		{
			return false;
		}

		// Initialize the terrain object.
		result = pTerrain->Initialize(pDirect3D->GetDevice());
		if(!result)
		{
			MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
			return false;
		}
		
		// Create the color shader object.
		pColorShader = new ColorShader;
		if(!pColorShader)
		{
			return false;
		}

		// Initialize the color shader object.
		result = pColorShader->Initialize(pDirect3D->GetDevice(), hwnd);
		if(!result)
		{
			MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
			return false;
		}

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

		// Create the position object.
		pPosition = new Position;
		if(!pPosition)
		{
			return false;
		}

		// Set the initial position of the viewer to the same as the initial camera position.
		pPosition->SetPosition(cameraX, cameraY, cameraZ);

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
		pText = new Text;
		if(!pText)
		{
			return false;
		}

		// Initialize the text object.
		result = pText->Initialize(pDirect3D->GetDevice(), pDirect3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
		if(!result)
		{
			MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
			return false;
		}

		// Retrieve the video card information.
		pDirect3D->GetVideoCardInfo(videoCard, videoMemory);

		// Set the video card information in the text object.
		result = pText->SetVideoCardInfo(videoCard, videoMemory, pDirect3D->GetDeviceContext());
		if(!result)
		{
			MessageBox(hwnd, L"Could not set video card info in the text object.", L"Error", MB_OK);
			return false;
		}
	
		return true;
	}


	void Engine::Shutdown()
	{
		// Release the text object.
		if(pText)
		{
			pText->Shutdown();
			delete pText;
			pText = 0;
		}

		// Release the font shader object.
		if(pFontShader)
		{
			pFontShader->Shutdown();
			delete pFontShader;
			pFontShader = 0;
		}

		// Release the cpu object.
		if(pCpu)
		{
			pCpu->Shutdown();
			delete pCpu;
			pCpu = 0;
		}

		// Release the fps object.
		if(pFps)
		{
			delete pFps;
			pFps = 0;
		}

		// Release the position object.
		if(pPosition)
		{
			delete pPosition;
			pPosition = 0;
		}

		// Release the timer object.
		if(pTimer)
		{
			delete pTimer;
			pTimer = 0;
		}

		// Release the color shader object.
		if(pColorShader)
		{
			pColorShader->Shutdown();
			delete pColorShader;
			pColorShader = 0;
		}

		// Release the terrain object.
		if(pTerrain)
		{
			pTerrain->Shutdown();
			delete pTerrain;
			pTerrain = 0;
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
			pDirect3D->Shutdown();
			delete pDirect3D;
			pDirect3D = 0;
		}

		// Release the input object.
		if(pInput)
		{
			pInput->Shutdown();
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
		if(pInput->IsEscapePressed() == true)
		{
			return false;
		}

		// Update the system stats.
		pTimer->Frame();
		pFps->Frame();
		pCpu->Frame();

		// Update the FPS value in the text object.
		result = pText->SetFps(pFps->GetFps(), pDirect3D->GetDeviceContext());
		if(!result)
		{
			return false;
		}
	
		// Update the CPU usage value in the text object.
		result = pText->SetCpu(pCpu->GetCpuPercentage(), pDirect3D->GetDeviceContext());
		if(!result)
		{
			return false;
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
		bool keyDown, result;
		float posX, posY, posZ, rotX, rotY, rotZ;


		// Set the frame time for calculating the updated position.
		pPosition->SetFrameTime(frameTime);

		// Handle the input.
		keyDown = pInput->IsLeftPressed();
		pPosition->TurnLeft(keyDown);

		keyDown = pInput->IsRightPressed();
		pPosition->TurnRight(keyDown);

		keyDown = pInput->IsUpPressed();
		pPosition->MoveForward(keyDown);

		keyDown = pInput->IsDownPressed();
		pPosition->MoveBackward(keyDown);

		keyDown = pInput->IsAPressed();
		pPosition->MoveUpward(keyDown);

		keyDown = pInput->IsZPressed();
		pPosition->MoveDownward(keyDown);

		keyDown = pInput->IsPgUpPressed();
		pPosition->LookUpward(keyDown);

		keyDown = pInput->IsPgDownPressed();
		pPosition->LookDownward(keyDown);
	
		// Get the view point position/rotation.
		pPosition->GetPosition(posX, posY, posZ);
		pPosition->GetRotation(rotX, rotY, rotZ);

		// Set the position of the camera.
		pCamera->SetPosition(posX, posY, posZ);
		pCamera->SetRotation(rotX, rotY, rotZ);

		// Update the position values in the text object.
		result = pText->SetCameraPosition(posX, posY, posZ, pDirect3D->GetDeviceContext());
		if(!result)
		{
			return false;
		}

		// Update the rotation values in the text object.
		result = pText->SetCameraRotation(rotX, rotY, rotZ, pDirect3D->GetDeviceContext());
		if(!result)
		{
			return false;
		}

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

		// Render the terrain buffers.
		pTerrain->Render(pDirect3D->GetDeviceContext());

		// Render the model using the color shader.
		result = pColorShader->Render(pDirect3D->GetDeviceContext(), pTerrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
		if(!result)
		{
			return false;
		}

		// Turn off the Z buffer to begin all 2D rendering.
		pDirect3D->TurnZBufferOff();
		
		// Turn on the alpha blending before rendering the text.
		pDirect3D->TurnOnAlphaBlending();

		// Render the text user interface elements.
		result = pText->Render(pDirect3D->GetDeviceContext(), pFontShader, worldMatrix, orthoMatrix);
		if(!result)
		{
			return false;
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
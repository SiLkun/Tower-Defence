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
		pCamera->SetPosition( D3DXVECTOR3(0.0f, 0.0f, -1.0f));
		pCamera->Render();
		pCamera->GetViewMatrix(baseViewMatrix);

		pCamera->SetPosition(D3DXVECTOR3(50.0f, 2.0f, 7.0f));
		
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
		if(pInput->IsKeyPressed(DIK_ESCAPE) == true)
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
		bool result;

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
	
		if(pInput->IsMouseRightPressed())
		{
			int pMousePreviousX, pMousePreviousY,pMouseX, pMouseY;
			pInput->GetMousePreviousLocation(pMousePreviousX, pMousePreviousY);
			pInput->GetMouseLocation(pMouseX, pMouseY);

			int mouseMovementX = pMouseX - pMousePreviousX;
			int mouseMovementY = pMouseY - pMousePreviousY;

			rotate.x = mouseMovementY;
			rotate.y = mouseMovementX;
		}
		pCamera->Move(move);
		pCamera->Rotate(rotate);


		D3DXVECTOR3 position(0,0,0);
		D3DXVECTOR3 rotation(0,0,0);

		// Set the position of the camera.
		pCamera->GetPosition(position);
		pCamera->GetRotation(rotation);

		// Update the position values in the text object.
		result = pText->SetCameraPosition(position.x, position.y, position.z, pDirect3D->GetDeviceContext());
		if(!result)
		{
			return false;
		}

		// Update the rotation values in the text object.
		result = pText->SetCameraRotation(rotation.x, rotation.y, rotation.z, pDirect3D->GetDeviceContext());
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
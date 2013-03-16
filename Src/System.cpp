////////////////////////////////////////////////////////////////////////////////
// Filename: system.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{

	System::System()
	{
		pEngine = 0;
	}


	System::System(const System& other)
	{
	}


	System::~System()
	{
	}


	bool System::Initialize()
	{
		int screenWidth, screenHeight;
		bool result;


		// Initialize the width and height of the screen to zero before sending the variables into the function.
		screenWidth = 0;
		screenHeight = 0;

		// Initialize the windows api.
		InitializeWindows(screenWidth, screenHeight);

		// Create the application wrapper object.
		pEngine = new Engine;
		if(!pEngine)
		{
			return false;
		}

		// Initialize the application wrapper object.
		result = pEngine->Initialize(hinstance, hwnd, screenWidth, screenHeight);
		if(!result)
		{
			return false;
		}

		return true;
	}


	void TD::System::Release()
	{
		// Release the application wrapper object.
		if(pEngine)
		{
			pEngine->Release();
			delete pEngine;
			pEngine = 0;
		}

		// Release the window.
		ReleaseWindows();
	
		return;
	}


	void TD::System::Run()
	{
		MSG msg;
		bool done, result;


		// Initialize the message structure.
		ZeroMemory(&msg, sizeof(MSG));
	
		// Loop until there is a quit message from the window or the user.
		done = false;
		while(!done)
		{
			// Handle the windows messages.
			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// If windows signals to end the application then exit out.
			if(msg.message == WM_QUIT)
			{
				done = true;
			}
			else
			{
				// Otherwise do the frame processing.
				result = Frame();
				if(!result)
				{
					done = true;
				}
			}

		}

		return;
	}


	bool TD::System::Frame()
	{
		bool result;

		// Do the frame processing for the application object.
		result = pEngine->Frame();
		if(!result)
		{
			return false;
		}


		return true;
	}


	LRESULT CALLBACK TD::System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}


	void TD::System::InitializeWindows(int& screenWidth, int& screenHeight)
	{
		WNDCLASSEX wc;
		DEVMODE dmScreenSettings;
		int posX, posY;


		// Get an external pointer to this object.	
		ApplicationHandle = this;

		// Get the instance of this application.
		hinstance = GetModuleHandle(NULL);

		// Give the application a name.
		applicationName = L"Engine";

		// Setup the windows class with default settings.
		wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc   = WndProc;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = 0;
		wc.hInstance     = hinstance;
		wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
		wc.hIconSm       = wc.hIcon;
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName  = NULL;
		wc.lpszClassName = applicationName;
		wc.cbSize        = sizeof(WNDCLASSEX);
	
		// Register the window class.
		RegisterClassEx(&wc);

		// Determine the resolution of the clients desktop screen.
		screenWidth  = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
		if(FULL_SCREEN)
		{
			// If full screen set the screen to maximum size of the users desktop and 32bit.
			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
			dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
			dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
			dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
			dmScreenSettings.dmBitsPerPel = 32;			
			dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			// Change the display settings to full screen.
			ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

			// Set the position of the window to the top left corner.
			posX = posY = 0;
		}
		else
		{
			// If windowed then set it to 800x600 resolution.
			screenWidth  = screenWidth;
			screenHeight = screenHeight;

			// Place the window in the middle of the screen.
			posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
			posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
		}

		// Create the window with the screen settings and get the handle to it.
		hwnd = CreateWindowEx(NULL, applicationName, applicationName, 
								WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
								posX, posY, screenWidth, screenHeight, NULL, NULL, hinstance, NULL);

		// Bring the window up on the screen and set it as main focus.
		ShowWindow(hwnd, SW_SHOW);
		SetForegroundWindow(hwnd);
		SetFocus(hwnd);

		// Hide the mouse cursor.
		ShowCursor(true);

		return;
	}


	void TD::System::ReleaseWindows()
	{
		// Show the pMouse cursor.
		ShowCursor(true);

		// Fix the display settings if leaving full screen mode.
		if(FULL_SCREEN)
		{
			ChangeDisplaySettings(NULL, 0);
		}

		// Remove the window.
		DestroyWindow(hwnd);
		hwnd = NULL;

		// Remove the application instance.
		UnregisterClass(applicationName, hinstance);
		hinstance = NULL;

		// Release the pointer to this class.
		ApplicationHandle = NULL;

		return;
	}


	LRESULT CALLBACK TD::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
	{
		switch(umessage)
		{
			// Check if the window is being destroyed.
			case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}

			// Check if the window is being closed.
			case WM_CLOSE:
			{
				PostQuitMessage(0);		
				return 0;
			}

			// All other messages pass to the message handler in the system class.
			default:
			{
				return TD::ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
			}
		}
	}
}
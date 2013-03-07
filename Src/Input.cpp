////////////////////////////////////////////////////////////////////////////////
// Filename: Input.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{

	Input::Input()
	{
		pDirectInput = 0;
		pKeyboard = 0;
		pMouse = 0;
	}


	Input::Input(const Input& other)
	{
	}


	Input::~Input()
	{
	}


	bool Input::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
	{
		HRESULT result;


		// Store the screen size which will be used for positioning the pMouse cursor.
		screenWidth = screenWidth;
		screenHeight = screenHeight;

		// Initialize the location of the pMouse on the screen.
		pMouseX = 0;
		pMouseY = 0;

		// Initialize the main direct input interface.
		result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDirectInput, NULL);
		if(FAILED(result))
		{
			return false;
		}

		// Initialize the direct input interface for the pKeyboard.
		result = pDirectInput->CreateDevice(GUID_SysKeyboard, &pKeyboard, NULL);
		if(FAILED(result))
		{
			return false;
		}

		// Set the data format.  In this case since it is a pKeyboard we can use the predefined data format.
		result = pKeyboard->SetDataFormat(&c_dfDIKeyboard);
		if(FAILED(result))
		{
			return false;
		}

		// Set the cooperative level of the pKeyboard to not share with other programs.
		result = pKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		if(FAILED(result))
		{
			return false;
		}

		// Now acquire the pKeyboard.
		result = pKeyboard->Acquire();
		if(FAILED(result))
		{
			return false;
		}

		// Initialize the direct input interface for the pMouse.
		result = pDirectInput->CreateDevice(GUID_SysMouse, &pMouse, NULL);
		if(FAILED(result))
		{
			return false;
		}

		// Set the data format for the pMouse using the pre-defined pMouse data format.
		result = pMouse->SetDataFormat(&c_dfDIMouse);
		if(FAILED(result))
		{
			return false;
		}

		// Set the cooperative level of the pMouse to share with other programs.
		result = pMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if(FAILED(result))
		{
			return false;
		}

		// Acquire the pMouse.
		result = pMouse->Acquire();
		if(FAILED(result))
		{
			return false;
		}

		return true;
	}


	void Input::Shutdown()
	{
		// Release the pMouse.
		if(pMouse)
		{
			pMouse->Unacquire();
			pMouse->Release();
			pMouse = 0;
		}

		// Release the pKeyboard.
		if(pKeyboard)
		{
			pKeyboard->Unacquire();
			pKeyboard->Release();
			pKeyboard = 0;
		}

		// Release the main interface to direct input.
		if(pDirectInput)
		{
			pDirectInput->Release();
			pDirectInput = 0;
		}

		return;
	}


	bool Input::Frame()
	{
		bool result;


		// Read the current state of the pKeyboard.
		result = ReadKeyboard();
		if(!result)
		{
			return false;
		}

		// Read the current state of the pMouse.
		result = ReadMouse();
		if(!result)
		{
			return false;
		}

		// Process the changes in the pMouse and pKeyboard.
		ProcessInput();

		return true;
	}


	bool Input::ReadKeyboard()
	{
		HRESULT result;


		// Read the pKeyboard pDevice.
		result = pKeyboard->GetDeviceState(sizeof(pKeyboardState), (LPVOID)&pKeyboardState);
		if(FAILED(result))
		{
			// If the pKeyboard lost focus or was not acquired then try to get control back.
			if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			{
				pKeyboard->Acquire();
			}
			else
			{
				return false;
			}
		}
		
		return true;
	}


	bool Input::ReadMouse()
	{
		HRESULT result;


		// Read the pMouse pDevice.
		result = pMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&pMouseState);
		if(FAILED(result))
		{
			// If the pMouse lost focus or was not acquired then try to get control back.
			if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			{
				pMouse->Acquire();
			}
			else
			{
				return false;
			}
		}

		return true;
	}


	void Input::ProcessInput()
	{
		// Update the location of the pMouse cursor based on the change of the pMouse location during the frame.
		pMouseX += pMouseState.lX;
		pMouseY += pMouseState.lY;

		// Ensure the pMouse location doesn't exceed the screen width or height.
		if(pMouseX < 0)  { pMouseX = 0; }
		if(pMouseY < 0)  { pMouseY = 0; }
	
		if(pMouseX > screenWidth)  { pMouseX = screenWidth; }
		if(pMouseY > screenHeight) { pMouseY = screenHeight; }
	
		return;
	}


	void Input::GetMouseLocation(int& pMouseX, int& pMouseY)
	{
		pMouseX = pMouseX;
		pMouseY = pMouseY;
		return;
	}


	bool Input::IsEscapePressed()
	{
		// Do a bitwise and on the pKeyboard state to check if the escape key is currently being pressed.
		if(pKeyboardState[DIK_ESCAPE] & 0x80)
		{
			return true;
		}

		return false;
	}


	bool Input::IsLeftPressed()
	{
		// Do a bitwise and on the pKeyboard state to check if the key is currently being pressed.
		if(pKeyboardState[DIK_LEFT] & 0x80)
		{
			return true;
		}

		return false;
	}


	bool Input::IsRightPressed()
	{
		// Do a bitwise and on the pKeyboard state to check if the key is currently being pressed.
		if(pKeyboardState[DIK_RIGHT] & 0x80)
		{
			return true;
		}

		return false;
	}


	bool Input::IsUpPressed()
	{
		// Do a bitwise and on the pKeyboard state to check if the key is currently being pressed.
		if(pKeyboardState[DIK_UP] & 0x80)
		{
			return true;
		}

		return false;
	}


	bool Input::IsDownPressed()
	{
		// Do a bitwise and on the pKeyboard state to check if the key is currently being pressed.
		if(pKeyboardState[DIK_DOWN] & 0x80)
		{
			return true;
		}

		return false;
	}


	bool Input::IsAPressed()
	{
		// Do a bitwise and on the pKeyboard state to check if the key is currently being pressed.
		if(pKeyboardState[DIK_A] & 0x80)
		{
			return true;
		}

		return false;
	}


	bool Input::IsZPressed()
	{
		// Do a bitwise and on the pKeyboard state to check if the key is currently being pressed.
		if(pKeyboardState[DIK_Z] & 0x80)
		{
			return true;
		}

		return false;
	}


	bool Input::IsPgUpPressed()
	{
		// Do a bitwise and on the pKeyboard state to check if the key is currently being pressed.
		if(pKeyboardState[DIK_PGUP] & 0x80)
		{
			return true;
		}

		return false;
	}


	bool Input::IsPgDownPressed()
	{
		// Do a bitwise and on the pKeyboard state to check if the key is currently being pressed.
		if(pKeyboardState[DIK_PGDN] & 0x80)
		{
			return true;
		}

		return false;
	}
}
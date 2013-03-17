////////////////////////////////////////////////////////////////////////////////
// Filename: Input.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{

	Input::Input()
	{
		mouseLeftDown = false;
		pDirectInput = 0;
		pKeyboard = 0;
		pMouse = 0;

		mouseX = 0;
		mouseY = 0;
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
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;

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
		result = pKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if(FAILED(result))
		{
			return false;
		}

		// Now acquire the pKeyboard.
		result = pKeyboard->Acquire();

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
		if(SUCCEEDED(result))
		{
			pMouse->Acquire();

		}



		return true;
	}


	void Input::Release()
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

		return true;
	}


	bool Input::ReadKeyboard()
	{
		HRESULT result;


		// Read the pKeyboard pDevice.
		result = pKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
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
		result = pMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
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
				// Update the location of the pMouse cursor based on the change of the pMouse location during the frame.
		mouseX += mouseState.lX;
		mouseY += mouseState.lY;

		// Ensure the mouse location doesn't exceed the screen width or height.
		if(mouseX < 0)  
		{ 
			mouseX = 0; 
		}
		
		if(mouseY < 0)  
		{ 
			mouseY = 0; 
		}
	
		if(mouseX > screenWidth)  
		{ 
			mouseX = screenWidth; 
		}

		if(mouseY > screenHeight) 
		{ 
			mouseY = screenHeight; 
		}
	
		if(mouseLeftDown)
		{
			if(!IsMouseLeftPressed())
			{
				mouseLeftDown = false;
			}
		}
		else
		{
			if(IsMouseLeftPressed())
			{
				mouseLeftDown = true;
				mouseLeftDownX = mouseX;
				mouseLeftDownY = mouseY;

			}
		}

		if(IsMouseRightPressed())
		{
			if(!mouseRightDown)
			{
				mouseRightDown = true;
				mouseRightDownX = mouseX;
				mouseRightDownY = mouseY;
			}
		}
		else if(!IsMouseRightPressed())
		{
			mouseRightDown = false;
		}
		return true;
	}

	bool Input::IsMouseLeftPressed()
	{
		if(mouseState.rgbButtons[0] & 0x80)
		{
			return true;
		}
		return false;
	}

	bool Input::IsMouseRightPressed()
	{
		if(mouseState.rgbButtons[1] & 0x80)
		{
			return true;
		}
		return false;
	}

	void Input::GetMouseLeftDownLocation(int& mouseX, int& mouseY)
	{
		mouseX = this->mouseLeftDownX;
		mouseY = this->mouseLeftDownY;
		return;
	}

	void Input::GetMouseRightDownLocation(int& mouseX, int& mouseY)
	{
		mouseX = this->mouseRightDownX;
		mouseY = this->mouseRightDownY;
		return;
	}
	void Input::GetMouseLocation(int& mouseX, int& mouseY)
	{
		mouseX = this->mouseX;
		mouseY = this->mouseY;
		return;
	}


	bool Input::IsKeyPressed(unsigned char key)
	{
		// Do a bitwise and on the pKeyboard state to check if the escape key is currently being pressed.
		if(keyboardState[key] & 0x80)
		{
			return true;
		}

		return false;
	}
}
////////////////////////////////////////////////////////////////////////////////
// Filename: Input.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUT_H_
#define _INPUT_H_

#include "stdafx.h"


///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define DIRECTINPUT_VERSION 0x0800


////////////////////////////////////////////////////////////////////////////////
// Class name: Input
////////////////////////////////////////////////////////////////////////////////
class Input
{
public:
	Input();
	Input(const Input&);
	~Input();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsMouseLeftPressed();
	bool IsMouseRightPressed();
	void GetMouseLeftDownLocation(int&, int&);
	void GetMouseRightDownLocation(int&, int&);
	void GetMouseLocation(int&, int&);

	bool IsKeyPressed(unsigned char);


private:
	bool ReadKeyboard();
	bool ReadMouse();
private:
	IDirectInput8* pDirectInput;
	IDirectInputDevice8* pKeyboard;
	IDirectInputDevice8* pMouse;

	unsigned char keyboardState[256];
	DIMOUSESTATE mouseState;

	int screenWidth, screenHeight;

	bool mouseLeftDown,mouseRightDown;
	int mouseX, mouseY;
	int mouseLeftDownX, mouseLeftDownY;
	int mouseRightDownX, mouseRightDownY;
};

#endif
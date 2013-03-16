////////////////////////////////////////////////////////////////////////////////
// Filename: Winmain.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{

#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
	_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(422112);
#endif

	TD::System* System;
	bool result;
	
	
	// Create the system object.
	System = new TD::System;
	if(!System)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = System->Initialize();
	if(result)
	{
		System->Run();
	}

	// Release and release the system object.
	System->Release();
	delete System;
	System = 0;

#if defined(DEBUG) | defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}
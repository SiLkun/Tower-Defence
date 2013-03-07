///////////////////////////////////////////////////////////////////////////////
// Filename: timerclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{


	Timer::Timer()
	{
	}


	Timer::Timer(const Timer& other)
	{
	}


	Timer::~Timer()
	{
	}


	bool Timer::Initialize()
	{
		// Check to see if this system supports high performance timers.
		QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
		if(frequency == 0)
		{
			return false;
		}

		// Find out how many times the frequency counter ticks every millisecond.
		ticksPerMs = (float)(frequency / 1000);

		QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

		return true;
	}


	void Timer::Frame()
	{
		INT64 currentTime;
		float timeDifference;


		// Query the current time.
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

		// Calculate the difference in time since the last time we queried for the current time.
		timeDifference = (float)(currentTime - startTime);

		// Calculate the frame time by the time difference over the timer speed resolution.
		frameTime = timeDifference / ticksPerMs;

		// Restart the timer.
		startTime = currentTime;

		return;
	}


	float Timer::GetTime()
	{
		return frameTime;
	}
}
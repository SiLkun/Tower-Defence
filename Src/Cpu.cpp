///////////////////////////////////////////////////////////////////////////////
// Filename: cpuclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{
	Cpu::Cpu()
	{
	}


	Cpu::Cpu(const Cpu& other)
	{
	}


	Cpu::~Cpu()
	{
	}


	void Cpu::Initialize()
	{
		PDH_STATUS status;


		// Initialize the flag indicating whether this object can read the system cpu usage or not.
		canReadCpu = true;

		// Create a query object to poll cpu usage.
		status = PdhOpenQuery(NULL, 0, &queryHandle);
		if(status != ERROR_SUCCESS)
		{
			canReadCpu = false;
		}

		// Set query object to poll all cpus in the system.
		status = PdhAddCounter(queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &counterHandle);
		if(status != ERROR_SUCCESS)
		{
			canReadCpu = false;
		}

		// Initialize the start time and cpu usage.
		lastSampleTime = GetTickCount(); 
		cpuUsage = 0;

		return;
	}


	void Cpu::Shutdown()
	{
		if(canReadCpu)
		{
			PdhCloseQuery(queryHandle);
		}

		return;
	}


	void Cpu::Frame()
	{
		PDH_FMT_COUNTERVALUE value; 

		if(canReadCpu)
		{
			// If it has been 1 second then update the current cpu usage and reset the 1 second timer again.
			if((lastSampleTime + 1000) < GetTickCount())
			{
				lastSampleTime = GetTickCount(); 

				PdhCollectQueryData(queryHandle);
        
				PdhGetFormattedCounterValue(counterHandle, PDH_FMT_LONG, NULL, &value);

				cpuUsage = value.longValue;
			}
		}

		return;
	}


	int Cpu::GetCpuPercentage()
	{
		int usage;


		// If the class can read the cpu from the operating system then return the current usage.  If not then return zero.
		if(canReadCpu)
		{
			usage = (int)cpuUsage;
		}
		else
		{
			usage = 0;
		}

		return usage;
	}
}
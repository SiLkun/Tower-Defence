////////////////////////////////////////////////////////////////////////////////
// Filename: Tower.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{

	Tower::Tower()
	{
	}

	Tower::Tower(const Tower&)
	{
	}

	Tower::~Tower()
	{
	}

	void Tower::DetermineTarget()
	{
		
	}

	bool Tower::hasTarget()
	{
		if(pTarget)
			return true;

		return false;
	}

	void Tower::Attack()
	{
	}
}
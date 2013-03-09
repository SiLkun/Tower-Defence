////////////////////////////////////////////////////////////////////////////////
// Filename: Game.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{
	Creeper::Creeper()
	{
	}

	Creeper::Creeper(const Creeper&)
	{
	}

	Creeper::~Creeper()
	{
	}

	void Creeper::Update(float frameTime)
	{
		position.z -= (1 - (rand() % 2)) *frameTime * 0.001f;

	}

}
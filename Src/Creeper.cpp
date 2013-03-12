////////////////////////////////////////////////////////////////////////////////
// Filename: Game.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{
	Creeper::Creeper()
	{
		
		health = 1.0f;
	}

	Creeper::Creeper(const Creeper&)
	{
	}

	Creeper::~Creeper()
	{
	}



	
	void Creeper::Update(float frameTime)
	{
		position.z -=  0.01f * frameTime ;
		position.z +=  (rand() % 3) * 0.01f ;
		Model::Update();
	}

	void Creeper::Render(ID3D11DeviceContext* deviceContext)
	{
		if(onMap)
		{
			Model::Render(deviceContext);
		}

		return;
	}

	void Creeper::Hit(float damage)
	{
		health -= damage;
	}

	float Creeper::GetHealth()
	{
		return health;
	}

}
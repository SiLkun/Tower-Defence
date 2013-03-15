////////////////////////////////////////////////////////////////////////////////
// Filename: Game.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{
	Creeper::Creeper()
	{
		
		health = 0.5f;
		flying = false;
		fast = false;
		boss = false;
	}

	Creeper::Creeper(float healthModifier, bool flying, bool fast, bool boss)
	{
		this->health = healthModifier * 0.5f;
		this->flying = flying;
		this->fast = fast;
		this->boss = boss;
	}

	Creeper::Creeper(const Creeper&)
	{
	}

	Creeper::~Creeper()
	{
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

	bool Creeper::IsFlying(){
		return flying;
	}
	bool Creeper::IsFast(){
		return fast;
	}
	bool Creeper::IsBoss(){
		return boss;
	}
}
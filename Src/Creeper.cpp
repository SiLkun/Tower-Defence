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

	void Creeper::Update(float frameTime)
	{
		if(fast){
			position.z -=  0.05f * frameTime ;
			position.z +=  (rand() % 3) * 0.05f ;
		}
		else
		{
			position.z -=  0.01f * frameTime ;
			position.z +=  (rand() % 3) * 0.01f ;
		}
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

	float Creeper::GetFlying(){
		return flying;
	}
	float Creeper::GetFast(){
		return fast;
	}
	float Creeper::GetBoss(){
		return boss;
	}
}
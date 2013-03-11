////////////////////////////////////////////////////////////////////////////////
// Filename: Game.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{
	Creeper::Creeper()
	{
		onMap = false;
	}

	Creeper::Creeper(const Creeper&)
	{
	}

	Creeper::~Creeper()
	{
	}

	void Creeper::UpdateOnMap(float terrainWidth,float terrainHeight)
	{
		if(position.x > -terrainWidth/2 && position.x < terrainWidth/2 && position.z > -terrainHeight/2 && position.z < terrainHeight/2 )
		{
			onMap = true;
		}
		else
		{
			onMap = false;
		}

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
}
////////////////////////////////////////////////////////////////////////////////
// Filename: Game.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{
	 vector<Creeper::Config*> * Creeper::LoadCfg(char * filename)
	{
		ifstream fin;

		fin.open(filename);
		if(fin.fail())
		{
			return false;
		}

		char input[256];
		int numberoftowers = 0;
		vector<Config*> * configs = new vector<Config*>();
		
		Config * config = NULL;

		while(!fin.eof())
		{
			fin >> input;
			if(strcmp(input,"Creeper") == 0)
			{
				config = new Config();
				fin >> config->name;
				configs->push_back(config);
			}
			else if(strcmp(input,"mesh") == 0)
			{
				fin >> config->mesh;
			}
			else if(strcmp(input,"flying") == 0)
			{
				fin >> config->flying;
			}
			else if(strcmp(input,"speed") == 0)
			{
				fin >> config->speed;
			}
			else if(strcmp(input,"health") == 0)
			{
				fin >> config->health;
			}
			else if(strcmp(input,"boss") == 0)
			{
				fin >> config->boss;
			}
			else if(strcmp(input,"gold") == 0)
			{
				fin >> config->gold;
			}
		}

		return configs;
	}

	Creeper::Creeper()
	{
		onMap = false;
	}

	Creeper::Creeper(const Creeper& other)  : Model(other)
	{
		config = other.config;
	}

	Creeper::~Creeper()
	{
	}

	void Creeper::Initialize(ID3D11Device * pDevice,vector<Mesh *> * pMeshes,vector<Texture *> * pTextures,  Config * config)
	{
		this->config = *config;

		Mesh * pMesh = new Mesh();
		pMesh->Initialize(pDevice, this->config.mesh,pTextures);
		pMeshes->push_back(pMesh);

		SetSpeed(config->speed);
		Model::Initialize(pMesh);
	}

	void Creeper::Update(Terrain * pTerrain,float frameTime)
	{
		D3DXVECTOR3 p = position + direction;
		if(pTerrain->GetOccupied(floorf(p.x),floorf(p.z)))
		{
			acceleration.x = 0.0f;
			acceleration.y = 0.0f;
			acceleration.z = 0.0f;
		}

		Model::Update(frameTime);
		position.y = pTerrain->GetHeight(floorf(position.x),floorf(position.z));
	}

	void Creeper::Render(ID3D11DeviceContext* deviceContext)
	{
		Model::Render(deviceContext);
		return;
	}

	void Creeper::Hit(int damage)
	{
		config.health -= damage;
	}

	int Creeper::GetHealth()
	{
		return config.health;
	}

	void Creeper::SetHealth(int health)
	{
		config.health = health;
	}
	
	float Creeper::GetSpeed()
	{
		return config.speed;
	}

	void Creeper::SetSpeed(float speed)
	{
		config.speed = speed;
	}

	bool Creeper::IsFlying()
	{
		return config.flying;
	}

	bool Creeper::IsBoss()
	{
		return config.boss;
	}
}
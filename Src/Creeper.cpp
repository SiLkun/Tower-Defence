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
		reachedEnding = false;
		onMap = false;
		destination.x = 0;
		destination.y = 0;
		destination.z = 0;
		fateOutTime = 10000;

	}

	Creeper::Creeper(const Creeper& other)  : Model(other)
	{
		config = other.config;
		reachedEnding = false;
		fateOutTime = 2000;

	}

	Creeper::~Creeper()
	{

	}

	void Creeper::Initialize(ID3D11Device * pDevice,vector<Mesh *> * pMeshes,vector<Texture *> * pTextures,  Config * config)
	{
		this->config = *config;


		Mesh * pMesh = NULL;
		
		pMesh = Mesh::GetMesh(pMeshes,this->config.mesh);

		if(!pMesh)
		{
			pMesh = new Mesh();
			pMesh->Initialize(pDevice, this->config.mesh,pTextures);
			pMeshes->push_back(pMesh);
		}

		SetSpeed(config->speed);
		Model::Initialize(pMesh);


	}

	void Creeper::Update(Terrain * pTerrain,float frameTime)
	{
		if(reachedEnding || config.health <= 0)
		{
			acceleration.x = 0;
			acceleration.z = 0;
			if(reachedEnding)
				acceleration.y  = 0.04f;
			else
				acceleration.y  = -0.04f;
			fateOutTime -= frameTime;
		}
		else
		{
			if((int)position.x == (int)destination.x && (int)position.z == (int)destination.z)
			{
				reachedEnding = true;
			
			}
			else if(path.size() > 0)
			{
				POINT next = (POINT)*path.begin();
				if(floor(position.x + 0.5f) == next.x && floor(position.z + 0.5f) == next.y)
				{
					path.pop_front();
				}
				else
				{
					POINT next = (POINT)*path.begin();
					if(path.size() > 0)
					{			
						direction.x = next.x - position.x;
						direction.z = next.y -  position.z;

						D3DXVec3Normalize(&direction,&direction);

						D3DXVECTOR3 a;
						a.x = direction.x * config.speed ;
						a.y = direction.y * config.speed ;
						a.z = direction.z * config.speed ;
						this->SetAcceleration(a);
					}
				}
			}
	
			position.y = pTerrain->GetHeight(floorf(position.x),floorf(position.z));
		}
		Model::Update(frameTime);

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

	void Creeper::SetDestination(D3DXVECTOR3 d)
	{
		this->destination = d;
	}

	bool Creeper::ReachedEnding()
	{
		return reachedEnding;
	}

	void Creeper::SetPath(list<POINT> newpath)
	{
		path = newpath;
	}

}
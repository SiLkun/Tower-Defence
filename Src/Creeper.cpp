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

	}

	Creeper::Creeper(const Creeper& other)  : Model(other)
	{
		config = other.config;
		reachedEnding = false;
	}

	Creeper::~Creeper()
	{
		for (list<PathNode*>::iterator i = path.begin(); i != path.end(); i++)
		{
			delete *i;
			*i = NULL;
		}
		path.clear();
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
		if((int)position.x != (int)destination.x || (int)position.z != (int)destination.z)
		{
			if(path.size() == 0)
			{
				PathNode * node = new PathNode();
				node->x = (int)position.x;
				node->z = (int)position.z;
				node->moveCost =  0;
				node->EstimateCost = abs(destination.x - position.x) + abs(destination.z - position.z); 
				path.push_front(node);
				FindPath (pTerrain,&path,position.x,position.z,destination.x,destination.z);
			}
		}
		else
		{
			reachedEnding = true;
		}

		if(path.size() > 0)
		{
			PathNode * next = (PathNode * )*path.begin();
			if((int)position.x == next->x && (int)position.z == next->z)
			{
				delete next;
				path.pop_front();

				if(path.size() > 0)
				{			
					next = (PathNode * )*path.begin();
					direction.x = next->x - position.x;
					direction.z = next->z -  position.z;

					D3DXVec3Normalize(&direction,&direction);

					D3DXVECTOR3 a;
					a.x = direction.x * config.speed ;
					a.y = direction.y * config.speed ;
					a.z = direction.z * config.speed ;
					this->SetAcceleration(a);
				}
			}
		}
		else
		{
			D3DXVECTOR3 a;
			a.x = 0;
			a.y = 0;
			a.z = 0;
			this->SetAcceleration(a);
		}


	



		D3DXVECTOR3 p = position + direction;
		if(pTerrain->GetOccupied(p.x,p.z))
		{
			D3DXVECTOR3 a;
			a.x = 0;
			a.y = 0;
			a.z = 0;
			this->SetAcceleration(a);

			for (list<PathNode*>::iterator i = path.begin(); i != path.end(); i++)
			{
				delete *i;
				*i = NULL;
			}
			path.clear();

			PathNode * node = new PathNode();
			node->x = (int)position.x;
			node->z = (int)position.z;
			node->moveCost =  0;
			node->EstimateCost = abs(destination.x - position.x) + abs(destination.z - position.z); 
			path.push_front(node);
			FindPath (pTerrain,&path,position.x,position.z,destination.x,destination.z);
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

	void Creeper::SetDestination(D3DXVECTOR3 d)
	{
		this->destination = d;
	}

	bool Creeper::ReachedEnding()
	{
		return reachedEnding;
	}



//-----------------------------------------------------------------------------
// Name: FindPath
// Desc: Finds a path using A*
//-----------------------------------------------------------------------------
	bool Creeper::FindPath (Terrain * pTerrain,list<PathNode *> * path,int posX,int posY,int destX,int destY)
	{
		bool result = false;

		if(posX == destX && posY == destY)
			return true;


		int directions[8][3] = {{0,1,10},{1,1,14},{-1,1,14},{1,0,10},{-1,0,10},{-1,-1,14},{1,-1,14},{0,-1,10}};
		
		vector<PathNode*> paths; 
		for (int i = 0; i < 9; i ++)
		{
			if(pTerrain->GetWalkable(posX + directions[i][0],posY + directions[i][1]) && !pTerrain->GetOccupied(posX + directions[i][0],posY + directions[i][1]))
			{
				PathNode * node = new PathNode();
				node->x = posX + directions[i][0];
				node->z = posY + directions[i][1];
				node->moveCost =  directions[i][2];
				node->EstimateCost = sqrt(pow(destX - node->x,2) + pow(destY - node->z,2));


				for (vector<PathNode*>::iterator i = paths.begin(); i != paths.end(); i++)
				{
					PathNode * insertNodeHere = (PathNode*)*i;
					if((node->moveCost + node->EstimateCost)  < (insertNodeHere->moveCost + insertNodeHere->EstimateCost))
					{
						paths.insert(i,node);
						node = NULL;
						break;
					}
				}
				if(node)
				{
					paths.push_back(node);
				}
			}
			else if(pTerrain->GetOccupied(posX + directions[i][0],posY + directions[i][1]))
			{
				int bla = 4;
			}
		}

		vector<PathNode*>::iterator i = paths.begin();
		
		for (; i != paths.end(); )
		{
			PathNode * node = (PathNode*)*i;

			path->push_back(node);	
		
			if(!result && FindPath(pTerrain,path,node->x,node->z,destX,destY))
			{
				result = true;
				i = paths.erase(i);
				break;
			}
			else
			{
				i = i;
			}

			delete node;
			path->pop_back();

			i = paths.erase(i);
		}

		for (; i != paths.end(); )
		{
			PathNode * node = (PathNode*)*i;
			delete node;
			i = paths.erase(i);
		}

		return result;
	}
}
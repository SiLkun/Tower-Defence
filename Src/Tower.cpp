////////////////////////////////////////////////////////////////////////////////
// Filename: Tower.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{

	Tower::Tower()
	{
		pTarget = 0;
	}

	Tower::Tower(const Tower& tower) : Model(tower)
	{
		config.reloadtime = tower.config.reloadtime;
		config.range = tower.config.range;
		config.projectileSpeed = tower.config.projectileSpeed;
		config.damage = tower.config.damage;
		config.costs = tower.config.costs;
		pTarget = 0;
	}

	Tower::~Tower()
	{
	}

	void Tower::Initialize(ID3D11Device * pDevice,vector<Mesh *> * pMeshes,vector<Texture *> * pTextures,  Config * config)
	{
		this->config = *config;

		Mesh * pMesh = new Mesh();
		pMesh->Initialize(pDevice, this->config.mesh,pTextures);
		pMeshes->push_back(pMesh);

		Model::Initialize(pMesh);
	}

	 vector<Tower::Config*> * Tower::LoadCfg(char * filename)
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
			if(strcmp(input,"Tower") == 0)
			{
				config = new Config();
				fin >> config->name;
				configs->push_back(config);
			}
			else if(strcmp(input,"mesh") == 0)
			{
				fin >> config->mesh;
			}
			else if(strcmp(input,"reloadtime") == 0)
			{
				fin >> config->reloadtime;
			}
			else if(strcmp(input,"range") == 0)
			{
				fin >> config->range;
			}
			else if(strcmp(input,"projectileSpeed") == 0)
			{
				fin >> config->projectileSpeed;
			}
			else if(strcmp(input,"damage") == 0)
			{
				fin >> config->damage;
			}
			else if(strcmp(input,"costs") == 0)
			{
				fin >> config->costs;
			}
		}

		return configs;
	}

	void Tower::Update(ID3D11Device * pDevice,float time,float frameTime, Terrain * pTerrain,vector<Creeper*>* pCreepers,vector<Projectile*>* pProjectile)
	{
		if(HasTarget() && (time - lastAtackTime) > config.reloadtime)
		{
			lastAtackTime = time;
			Attack(pDevice,pProjectile);
		}

		Model::Update(frameTime);
	}

	void Tower::DetermineTarget(vector<Creeper*>* pCreepers)
	{
		if(HasTarget())
		{
			float distance = powf( powf(position.x - pTarget->GetPosition().x,2.0f) + powf(position.z - pTarget->GetPosition().z,2.0f),0.5f);

			if(distance >= config.range || pTarget->GetHealth() <= 0 || pTarget->reachedEnding)
			{
				pTarget = 0;
			}
		}

		if(!HasTarget() && pCreepers)
		{
			for(UINT iCreeper = 0;iCreeper < pCreepers->size();iCreeper++)
			{
				Creeper* pCreeper = pCreepers->at(iCreeper);
				D3DXVECTOR3 p = pCreeper->GetPosition();
				float distance = powf( powf(position.x - p.x,2.0f) + powf(position.z - p.z,2.0f),0.5f);

				if(distance < config.range &&  pCreeper->GetHealth() > 0 && !pCreeper->reachedEnding)
				{
					pTarget = pCreeper;
					return;
				}

			}
		}
	}

	bool Tower::HasTarget()
	{
		if(pTarget)
			return true;

		return false;
	}

	void Tower::Attack(ID3D11Device * pDevice,vector<Projectile*>* pProjectiles)
	{
		D3DXVECTOR3 delta = pTarget->GetPosition() - position;
		D3DXVECTOR3 vr = pTarget->GetAcceleration();
		float muzzleV = config.projectileSpeed;

		float a = D3DXVec3Dot(&vr, &vr) - muzzleV * muzzleV;
		float b = 2.0f*D3DXVec3Dot(&vr, &delta);
		float c = D3DXVec3Dot(&delta, &delta);

		float det = (b*b) - (4.0f*a*c);

		float solution;
		// If the determinant is negative, then there is no solution
		if(det > 0.0f)
		{
			solution = 2.0f*c/(sqrtf(det) - b);
		} 
		else 
		{
			solution =  -1.0f;
		}


		if(solution > 0.0f)
		{
			Projectile * pProjectile = new Projectile();
			pProjectile->SetPosition(this->position);
			pProjectile->SetSpeed(config.projectileSpeed);
			pProjectile->SetDamage(config.damage);

			D3DXVECTOR3 aimPoint = pTarget->GetPosition() + (solution * vr);
			pProjectile->SetTarget(aimPoint);
			pProjectiles->push_back(pProjectile);
		}


		
	}
}
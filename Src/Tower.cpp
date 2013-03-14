////////////////////////////////////////////////////////////////////////////////
// Filename: Tower.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{

	Tower::Tower()
	{
		reloadTime = 0.5f;
		lastAtackTime = -1.0f;
		pTarget = 0;
		range = 20.0f;
	}

	Tower::Tower(const Tower&)
	{

	}

	Tower::~Tower()
	{
	}

	void Tower::Update(ID3D11Device * pDevice,float time,float frameTime, Terrain * pTerrain,vector<Creeper*>* pCreepers,vector<Projectile*>* pProjectile)
	{
		if(HasTarget() && (time - lastAtackTime) > reloadTime)
		{
			lastAtackTime = time;
			Attack(pDevice,pProjectile);
		}

		Model::Update();
	}

	void Tower::DetermineTarget(vector<Creeper*>* pCreepers)
	{
		if(HasTarget())
		{
			float distance = powf( powf(position.x - pTarget->GetPosition()->x,2.0f) + powf(position.z - pTarget->GetPosition()->z,2.0f),0.5f);

				if(distance >= range)
				{
					pTarget = 0;
				}
		}
		if(pCreepers)
		{
			for(UINT iCreeper = 0;iCreeper < pCreepers->size();iCreeper++)
			{
				Creeper* pCreeper = pCreepers->at(iCreeper);
				D3DXVECTOR3 * p = pCreeper->GetPosition();
				float distance = powf( powf(position.x - p->x,2.0f) + powf(position.z - p->z,2.0f),0.5f);

				if(distance < range)
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
		Projectile * pProjectile = new Projectile();
		pProjectile->SetPosition(this->position.x, this->position.y + 2.0f, this->position.z);
		pProjectile->SetTarget((*pTarget->GetPosition()));
		pProjectiles->push_back(pProjectile);
	}
}
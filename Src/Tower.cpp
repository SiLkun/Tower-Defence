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
		projectileSpeed = 1.0f;
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

		Model::Update(frameTime);
	}

	void Tower::DetermineTarget(vector<Creeper*>* pCreepers)
	{
		if(HasTarget())
		{
			float distance = powf( powf(position.x - pTarget->GetPosition().x,2.0f) + powf(position.z - pTarget->GetPosition().z,2.0f),0.5f);

				if(distance >= range)
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
		D3DXVECTOR3 delta = pTarget->GetPosition() - position;
		D3DXVECTOR3 vr = pTarget->GetAcceleration();
		float muzzleV = projectileSpeed;

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
			pProjectile->SetSpeed(1.0f);

			D3DXVECTOR3 aimPoint = pTarget->GetPosition() + (solution * vr);
			pProjectile->SetTarget(aimPoint);
			pProjectiles->push_back(pProjectile);
		}


		
	}
}
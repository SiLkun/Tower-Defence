////////////////////////////////////////////////////////////////////////////////
// Filename: Projectile.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{
	Projectile::Projectile()
	{
		scale.x = 0.1f;
		scale.y = 0.1f;
		scale.z = 0.1f;
		isHit = false;
		damage = 0.03f;
	}

	Projectile::Projectile(const Projectile&)
	{
	}

	Projectile::~Projectile()
	{
	}

	void Projectile::SetTarget(D3DXVECTOR3 targetPosition)
	{
		target = targetPosition;
	}

	bool Projectile::IsHit()
	{
		return isHit;
	}

	void Projectile::Update(float frameTime,vector<Creeper*>* pCreepers)
	{
		D3DXVECTOR3 distance;
		distance.x = position.x - target.x;
		distance.y = position.y - target.y;
		distance.z = position.z - target.z;

		if(pCreepers)
		{
			for(UINT iCreeper = 0;iCreeper < pCreepers->size();iCreeper++)
			{
				Creeper* pCreeper = pCreepers->at(iCreeper);
				D3DXVECTOR3 * p = pCreeper->GetPosition();
				float distance = powf( powf(position.x - p->x,2.0f) + powf(position.z - p->z,2.0f),0.5f);

				if(distance < 1.0f)
				{
					pCreeper->Hit(damage);
					isHit = true;
				}

			}
		}

		if(D3DXVec3Length(&distance) < 1.0f)
		{
			isHit = true;
		}
		else
		{

			D3DXVec3Normalize(&distance,&distance);

			position.x -= (frameTime * distance.x * 0.05f);
			position.y -= (frameTime * distance.y * 0.05f);
			position.z -= (frameTime * distance.z * 0.05f);
		}

		Model::Update();
	}
}
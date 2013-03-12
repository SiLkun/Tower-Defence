////////////////////////////////////////////////////////////////////////////////
// Filename: Projectile.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{
	Projectile::Projectile()
	{
		isHit = false;
		damage = 0.3f;
	}

	Projectile::Projectile(const Projectile&)
	{
	}

	Projectile::~Projectile()
	{
	}

	void Projectile::SetTarget(D3DXVECTOR3 targetPosition)
	{
		D3DXVec3Normalize(&direction,&(position - targetPosition));
	}

	bool Projectile::IsHit()
	{
		return isHit;
	}

	void Projectile::Update(float frameTime,vector<Creeper*>* pCreepers)
	{
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

		if(!isHit)
		{
			position.x -= (frameTime * direction.x * 0.05f);
			position.y -= (frameTime * direction.y * 0.05f);
			position.z -= (frameTime * direction.z * 0.05f);
		}

		Model::Update();
	}
}
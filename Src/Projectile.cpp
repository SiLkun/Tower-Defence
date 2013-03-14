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

	void Projectile::Initialize(Mesh * pMesh)
	{
		Model::Initialize(pMesh);

		pLaunchSound->Play(this->position);
	}

	void Projectile::SetTarget(D3DXVECTOR3 targetPosition)
	{
		D3DXVec3Normalize(&direction,&(position - targetPosition));
	}

	void Projectile::SetLaunchSound(Sound * pSound)
	{
		pLaunchSound = pSound;
	}

	void Projectile::SetMoveSound(Sound * pSound)
	{
		pMoveSound = pSound;

	}

	void Projectile::SetHitSound(Sound * pSound)
	{
		pHitSound = pSound;
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
					pHitSound->Play(this->position);
				}

			}
		}

		if(!isHit)
		{
			position.x -= (frameTime * direction.x * 0.03f);
			position.y -= (frameTime * direction.y * 0.03f);
			position.z -= (frameTime * direction.z * 0.03f);
		}

		Model::Update();
	}

	void Projectile::Render(ID3D11DeviceContext* deviceContext)
	{
		Model::Render(deviceContext);
	}
}
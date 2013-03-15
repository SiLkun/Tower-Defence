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
		acceleration = -direction * speed;
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
			for(vector<Creeper*>::iterator iCreeper = pCreepers->begin();iCreeper != pCreepers->end();iCreeper++)
			{
				Creeper* pCreeper = (Creeper*) *iCreeper;
				D3DXVECTOR3 p = pCreeper->GetPosition();
				float distance = powf( powf(position.x - p.x,2.0f) + powf(position.z - p.z,2.0f),0.5f);

				if(distance < 1.0f)
				{
					pCreeper->Hit(damage);
					isHit = true;
					pHitSound->Play(this->position);
				}

			}
		}

		if(isHit)
		{
			acceleration.x = 0;
			acceleration.y = 0;
			acceleration.z = 0;
		}

		Model::Update(frameTime);
	}

	void Projectile::Render(ID3D11DeviceContext* deviceContext)
	{
		Model::Render(deviceContext);
	}
}
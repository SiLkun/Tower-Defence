////////////////////////////////////////////////////////////////////////////////
// Filename: Projectile.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{
	Projectile::Projectile()
	{
		scale.x = 0.2;
		scale.y = 0.2;
		scale.z = 0.2;

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

	void Projectile::Update(float frameTime)
	{
		float dx = position.x - target.x;
		float dy = position.y - target.y;
		float dz = position.z - target.z;

		position.x -= (frameTime * dx * 0.001f);
		position.y -= (frameTime * dy * 0.001f);
		position.z -= (frameTime * dz * 0.001f);

		Model::Update();
	}
}
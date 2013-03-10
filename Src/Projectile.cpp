////////////////////////////////////////////////////////////////////////////////
// Filename: Projectile.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{
	Projectile::Projectile()
	{
	}

	Projectile::Projectile(const Projectile&)
	{
	}

	Projectile::~Projectile()
	{
	}

	void Projectile::Update(float frameTime)
	{
		float dx = position.x - target.x;
		float dy = position.y - target.y;
		float dz = position.z - target.z;

		position.x -= (frameTime * dx * 0.01f);
		position.y -= (frameTime * dy * 0.01f);
		position.z -= (frameTime * dz * 0.01f);
	}
}
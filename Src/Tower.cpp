////////////////////////////////////////////////////////////////////////////////
// Filename: Tower.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{

	Tower::Tower()
	{
		projectiles = new vector<Projectile*>();
		scale.y = 2.0f;
		reloadTime = 0.1f;
		lastAtackTime = -1.0f;
		pTarget = 0;
		range = 10.0f;
	}

	Tower::Tower(const Tower&)
	{

	}

	Tower::~Tower()
	{
	}

	void Tower::Update(ID3D11Device * pDevice,float time,float frameTime)
	{
		if(HasTarget() && (time - lastAtackTime) > reloadTime)
		{
			lastAtackTime = time;
			Attack(pDevice);
		}


		if(projectiles)
		{
			for(UINT iProjectile = 0;iProjectile < projectiles->size();iProjectile++)
			{
				Projectile * pProjectile = projectiles->at(iProjectile);
				pProjectile->Update(frameTime);
			}
		}
		Model::Update();
	}

	void Tower::Render(ID3D11DeviceContext* pDeviceContext, LightShader * pLightShader, D3DXMATRIX viewMatrix,D3DXMATRIX projectionMatrix, Light * pLight)
	{
		if(projectiles)
		{
			for(UINT iProjectile = 0;iProjectile < projectiles->size();iProjectile++)
			{
				Projectile * pProjectile = projectiles->at(iProjectile);
				pProjectile->Render(pDeviceContext);
				pLightShader->Render(pDeviceContext, pProjectile->GetIndexCount(), pProjectile->GetWorldMatrix(), viewMatrix, projectionMatrix,  pProjectile->GetTexture(),pLight->GetAmbientColor(), pLight->GetDiffuseColor(), pLight->GetDirection() );
			}
		}
		Model::Render(pDeviceContext);

		return;
	}

	void Tower::DetermineTarget(vector<Creeper*>* pCreepers)
	{
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

	void Tower::Attack(ID3D11Device * pDevice)
	{
		Projectile * pProjectile = new Projectile();
		pProjectile->Initialize(pDevice, "data/cube.txt", L"data/seafloor.dds");
		pProjectile->SetPosition(this->position.x, this->position.y, this->position.z);
		pProjectile->SetTarget(*pTarget->GetPosition());
		projectiles->push_back(pProjectile);
	}

	vector<Projectile*> Tower::GetProjectiles()
	{
		return *projectiles;
	}
}
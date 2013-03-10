////////////////////////////////////////////////////////////////////////////////
// Filename: Tower.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{

	Tower::Tower()
	{
		projectiles = new vector<Projectile*>();
	}

	Tower::Tower(const Tower&)
	{
	}

	Tower::~Tower()
	{
	}

	void Tower::Update(float frameTime)
	{
		if(projectiles)
		{
			for(UINT iProjectile = 0;iProjectile < projectiles->size();iProjectile++)
			{
				Projectile * pProjectile = projectiles->at(iProjectile);

				//pProjectile->Render(pDeviceContext);
				pProjectile->Update(frameTime);

				//D3DXMATRIX modelMatrix;
				//D3DXMatrixIdentity(&modelMatrix);
				//D3DXMatrixTranslation(&modelMatrix,pProjectile->GetPosition()->x,pProjectile->GetPosition()->y,pProjectile->GetPosition()->z);

				// Render the model using the light shader.
				//result = pLightShader->Render(pDeviceContext, pProjectile->GetIndexCount(), modelMatrix, viewMatrix, projectionMatrix, 
						   //pProjectile->GetTexture(), pLight->GetDirection(), pLight->GetDiffuseColor());
			}
		}
	}

	void Tower::DetermineTarget()
	{
		
	}

	bool Tower::HasTarget()
	{
		if(pTarget)
			return true;

		return false;
	}

	void Tower::Attack()
	{
		Projectile * pProjectile = new Projectile();
		//result = pProjectile->Initialize(pDevice, "data/cube.txt", L"data/seafloor.dds");
		pProjectile->SetPosition(this->position.x, this->position.y, this->position.z);

		projectiles->push_back(pProjectile);
	}

	vector<Projectile*> Tower::GetProjectiles()
	{
		return *projectiles;
	}
}
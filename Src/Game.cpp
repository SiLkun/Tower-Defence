////////////////////////////////////////////////////////////////////////////////
// Filename: Game.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{
	Game::Game()
	{
		pTowerPlacement= 0;
		pTerrainShader = 0;
		pTerrain = 0;
		pLight = 0;
		pLightShader = 0;

		time = 0.0f;
		previousWaveTime = -10.0f;
		waveDelay = 20.0f;
		int waveCount = 10;
		int level = 0;
		int gold = 0;
		lives = 0;
		loaded = false;
	}
	Game::Game(const Game&)
	{
	}
	Game::~Game()
	{

	}

	bool Game::Initialize(ID3D11Device * pDevice, HWND hwnd)
	{
		bool result;
		level = 0;
		gold = 100;
		lives = 100;

		config = LoadCfg("Data/Config/Game.cfg");

		// Create the terrain object.
		pTerrain = new Terrain;
		if(!pTerrain)
		{
			return false;
		}
	
		result = pTerrain->Initialize(pDevice, config.terrainConfig);
		if(!result)
		{
			MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
			return false;
		}
		// Create the color shader object.
		pTerrainShader = new TerrainShader;
		if(!pTerrainShader)
		{
			return false;
		}

		// Initialize the color shader object.
		result = pTerrainShader->Initialize(pDevice, hwnd);
		if(!result)
		{
			MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
			return false;
		}

		vector<Tower::Config*> * towerConfig = Tower::LoadCfg(config.towerConfig);
		for ( vector<Tower::Config*>::iterator i = towerConfig->begin(); i != towerConfig->end();)
		{
			Tower * pTower = new Tower();
			pTower->Initialize(pDevice,&meshes,&textures,(Tower::Config*)*i);
			towerList.push_back(pTower);
			delete *i;
			*i = 0;
			i = towerConfig->erase(i);
		}
		towerConfig->clear();
		delete towerConfig;
		towerConfig = 0;	

		vector<Creeper::Config*> * creeperConfig = Creeper::LoadCfg(config.creeperConfig);
		for ( vector<Creeper::Config*>::iterator i = creeperConfig->begin(); i != creeperConfig->end();)
		{
			Creeper * pCreeper = new Creeper();
			pCreeper->Initialize(pDevice,&meshes,&textures,(Creeper::Config*)*i);
			creeperList.push_back(pCreeper);
			delete *i;
			*i = 0;
			i = creeperConfig->erase(i);
		}
		creeperConfig->clear();
		delete creeperConfig;
		creeperConfig = 0;	

		
		Mesh * pProjectileMesh = new Mesh();
		pProjectileMesh->Initialize(pDevice, "Data/Model/Projectile.obj",&textures);
		meshes.push_back(pProjectileMesh);
		

					// Create the sound object.
		Sound * pSound = new Sound;
		if(!pSound)
		{
			return false;
		}
 
		// Initialize the sound object.
		result = pSound->Initialize("Data/Sound/rocket_fire.wav",hwnd);
		if(!result)
		{
			MessageBox(hwnd, L"Could not initialize Direct Sound.", L"Error", MB_OK);
			return false;
		}
 
		sounds.push_back(pSound);

				// Create the sound object.
		pSound = new Sound;
		if(!pSound)
		{
			return false;
		}
 
		// Initialize the sound object.
		result = pSound->Initialize("Data/Sound/rocket_cruise.wav",hwnd);
		if(!result)
		{
			MessageBox(hwnd, L"Could not initialize Direct Sound.", L"Error", MB_OK);
			return false;
		}
 
		sounds.push_back(pSound);
		
				// Create the sound object.
		pSound = new Sound;
		if(!pSound)
		{
			return false;
		}
 
		// Initialize the sound object.
		result = pSound->Initialize("Data/Sound/rocket_explode.wav",hwnd);
		if(!result)
		{
			MessageBox(hwnd, L"Could not initialize Direct Sound.", L"Error", MB_OK);
			return false;
		}
 
		sounds.push_back(pSound);


		// Create the light shader object.
		pLightShader = new LightShader;
		if(!pLightShader)
		{
			return false;
		}

		// Initialize the light shader object.
		result = pLightShader->Initialize(pDevice, hwnd);
		if(!result)
		{
			MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
			return false;
		}


		// Create the light object.
		pLight = new Light;
		if(!pLight)
		{
			return false;
		}

		// Initialize the light object.
		
		pLight->SetAmbientColor(0.3f, 0.3f, 0.3f, 1.0f);
		pLight->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
		pLight->SetDirection(0.0f, 0.0f, 1.0f);

		loaded = true;
		// Initialize the model object.
		return true;
	}

	void Game::Release()
	{
		// Release the terrain object.
		if(pTerrain)
		{
			pTerrain->Release();
			delete pTerrain;
			pTerrain = 0;
		}

		// Release the color shader object.
		if(pTerrainShader)
		{
			pTerrainShader->Release();
			delete pTerrainShader;
			pTerrainShader = 0;
		}


		// Release the terrain object.
		if(pLight)
		{
			delete pLight;
			pLight = 0;
		}
		
		if(pLightShader)
		{
			delete pLightShader;
			pLightShader = 0;
		}

		for(vector<Sound*>::iterator iSound = sounds.begin();iSound != sounds.end();iSound++)
		{
			Sound* pSound = (Sound*) *iSound;
			// Release the terrain object.
			if(pSound)
			{
				pSound->Release();
				delete pSound;
				pSound = 0;
			}
		}

		sounds.clear();



		for(UINT iCreeper = 0;iCreeper < creepersInGame.size();iCreeper++)
		{
			Creeper* pCreeper = creepersInGame.at(iCreeper);
			// Release the terrain object.
			if(pCreeper)
			{
				delete pCreeper;
				pCreeper = 0;
			}
		}

		creepersInGame.clear();


		for(UINT iCreeper = 0;iCreeper < creeperList.size();iCreeper++)
		{
			Creeper* pCreeper = creeperList.at(iCreeper);
			// Release the terrain object.
			if(pCreeper)
			{
				delete pCreeper;
				pCreeper = 0;
			}
		}

		creeperList.clear();


		for(UINT iTower = 0;iTower < towersInGame.size();iTower++)
		{
			Tower* pTower = towersInGame.at(iTower);
			if(pTower)
			{
				delete pTower;
				pTower = 0;
			}
		}

		towersInGame.clear();



		if(pTowerPlacement)
		{
			delete pTowerPlacement;
			pTowerPlacement = 0;
		}


		for(UINT iTower = 0;iTower < towerList.size();iTower++)
		{
			Tower* pTower = towerList.at(iTower);
			if(pTower)
			{
				delete pTower;
				pTower = 0;
			}
		}

		towerList.clear();



		for(UINT i = 0;i < projectilesInGame.size();i++)
		{
			Projectile* p = projectilesInGame.at(i);
			if(p)
			{
				delete p;
				p = 0;
			}
		}

		projectilesInGame.clear();



		for(UINT i = 0;i < projectileList.size();i++)
		{
			Projectile* p = projectileList.at(i);
			if(p)
			{
				delete p;
				p = 0;
			}
		}

		projectileList.clear();
		
		for(vector<Mesh*>::iterator iMesh = meshes.begin();iMesh != meshes.end();iMesh++)
		{
			Mesh * pMesh = (Mesh*)*iMesh;
			pMesh->Release();
			delete pMesh;
		}
		meshes.clear();


		for(UINT iTexture = 0;iTexture < textures.size();iTexture++)
		{
			Texture * pTexture = textures.at(iTexture);
			if(pTexture)
			{
				pTexture->Release();
				delete pTexture;
				pTexture = 0;
			}
		}

		textures.clear();
	}

	float Game::GetTime()
	{
		return time;
	}

	int Game::GetLevel()
	{
		return level;
	}

	int Game::GetGold()
	{
		return gold;
	}

	float Game::GetWaveDelay()
	{
		return waveDelay;
	}
	
	
	float Game::GetPreviousWaveTime()
	{
		return previousWaveTime;
	}
	

	int Game::GetLivesLeft()
	{
		return lives;
	}

	bool Game::IsLoaded()
	{
		return loaded;
	}

	bool Game::IsLost()
	{
		return lives <= 0;
	}

	Game::GameConfig Game::LoadCfg(char * filename)
	{
		GameConfig config;

		ifstream fin;

		fin.open(filename);
		if(fin.fail())
		{
			return config;
		}

		char input[256];
		while(!fin.eof())
		{
			fin >> input;
			if(strcmp(input,"towerConfig") == 0)
			{
				fin >> config.towerConfig;
			}
			else if(strcmp(input,"creeperConfig") == 0)
			{
				fin >> config.creeperConfig;
			}
			else if(strcmp(input,"terrainConfig") == 0)
			{
				fin >> config.terrainConfig;
			}
		}

		return config;
	}

	Terrain * Game::GetTerrain()
	{
		return pTerrain;
	}

	Sound * Game::GetSound(string fileName)
	{
		 for(vector<Sound*>::iterator iSound = sounds.begin();iSound != sounds.end();iSound++)
		 {
			 Sound * pSound = (Sound*)*iSound;
			 if(pSound && pSound->GetFilename().compare(fileName) == 0)
			 {
				 return pSound;
			 }
		 }
		 return NULL;
	}

	bool Game::Update(ID3D11Device * pDevice,Camera * pCamera,float frameTime)
	{
		bool result = false;
		if(!loaded)
		{
			return result;
		}
		time += frameTime / 1000;

		if(lives > 0)
		{
			if(time > previousWaveTime + waveDelay)
			{
				previousWaveTime = time;
				waveCount = 1;

				for (int i = 0; i < waveCount; i++)
				{
					int waveId = level % creeperList.size();

					// Create the model object.
					Creeper* pCreeper = new Creeper(*creeperList.at(waveId));
					if(!pCreeper)
					{
						return false;
					}

					D3DXVECTOR3 p(0,0,0);
					D3DXVECTOR3 d(0,0,-1.0f);
					D3DXVECTOR3 a(0,0,0);
					float scale = 1.0f;
				
					scale *=   ( 1.0f + ((rand() % 10)) * 0.01f);  //  max 10% difference

					a.x = 0 ; //d.x * pCreeper->GetSpeed() ;
					a.y = 0 ; //d.y * pCreeper->GetSpeed() ;
					a.z = 0 ; //d.z * pCreeper->GetSpeed() ;

					if(pCreeper->IsFlying()) 
					{
						p.y += 6.0f;
					}

					
					if(pCreeper->IsBoss())	
					{
						p.z = pTerrain->GetHeight()/2 - (i * 5.0f) -1.0f;		
						waveCount = 3;
					}
					else
					{
						p.z = pTerrain->GetHeight()/2 - (i * 2.0f)  -1.0f;
						waveCount = 10;
					}


					pCreeper->SetPosition(p);
					pCreeper->SetAcceleration(a);
					pCreeper->SetDestination(D3DXVECTOR3(0,0,-pTerrain->GetHeight()/2 -1.0f)  );
					pCreeper->SetSpeed(0.1f);
					pCreeper->SetScale(D3DXVECTOR3(scale,scale,scale));
				
					pCreeper->SetHealth(pCreeper->GetHealth() *  (1  + (    pow((level - waveId)/10,2)   )));
					pCreeper->SetPath(pTerrain->FindPath(p.x,p.z,0,-pTerrain->GetHeight()/2));
					creepersInGame.push_back(pCreeper);
				}
				level++;
			}
		}

		D3DXVECTOR3 p;
		pCamera->GetPosition(p);
		for(vector<Sound*>::iterator iSound = sounds.begin();iSound != sounds.end();iSound++)
		{
			Sound* pSound = (Sound*) *iSound;
			pSound->Update(p);
		}
		
		if(pLight)
		{
			result = pLight->Update(frameTime);
		}

	
		if(lives > 0)
		{
			for(vector<Creeper*>::iterator i = creepersInGame.begin(); i != creepersInGame.end();i++)
			{
				Creeper* pCreeper = (Creeper*)*i;
				pCreeper->Update(pTerrain, frameTime);
			
				pCreeper->UpdateOnMap((float)pTerrain->GetWidth(),(float)pTerrain->GetHeight());
			}



			for(vector<Tower*>::iterator i = towersInGame.begin();i != towersInGame.end();i++)
			{
				Tower* pTower = (Tower*)*i;
				pTower->DetermineTarget(&creepersInGame);
				pTower->Update(pDevice,time,frameTime,pTerrain,&creepersInGame,&projectilesInGame);
			}


			if(pTowerPlacement)
			{
				pTowerPlacement->Update(pDevice,time,frameTime,pTerrain,&creepersInGame,&projectilesInGame);
			}
		}

		for(vector<Projectile*>::iterator i = projectilesInGame.begin();i != projectilesInGame.end();)
		{
			Projectile * pProjectile = (Projectile*)*i;
			if(pProjectile->IsHit() || !pProjectile->IsOnMap())
			{
				delete pProjectile;
				pProjectile = 0;
				i = projectilesInGame.erase(i);
			}
			else
			{
				
				if(!pProjectile->GetMesh())
				{
					pProjectile->SetLaunchSound(GetSound("Data/Sound/rocket_fire.wav"));
					pProjectile->SetMoveSound(GetSound("Data/Sound/rocket_cruise.wav"));
					pProjectile->SetHitSound(GetSound("Data/Sound/rocket_explode.wav"));
					pProjectile->Initialize(Mesh::GetMesh(&meshes,"Data/Model/Projectile.obj"));

				}
				pProjectile->Update(frameTime,&creepersInGame);
				pProjectile->UpdateOnMap((float)pTerrain->GetWidth(),(float)pTerrain->GetHeight());

				i++;
			}

		}

		for(vector<Creeper*>::iterator i = creepersInGame.begin();i != creepersInGame.end();)
		{
			Creeper* pCreeper = (Creeper*) *i;
			if(pCreeper->GetHealth() <= 0 && pCreeper->fateOutTime <= 0.0f)
			{
				gold += pCreeper->config.gold;
				delete pCreeper;
				pCreeper = 0;
				i = creepersInGame.erase(i);
			}
			else if(pCreeper->ReachedEnding() && pCreeper->fateOutTime <= 0.0f)
			{
				delete pCreeper;
				pCreeper = 0;
				i = creepersInGame.erase(i);
				lives--;
			}
			else
			{
				i++;
			}
		}
		
		return result;
	}

	bool Game::Render(ID3D11DeviceContext* pDeviceContext,D3DXMATRIX& viewMatrix,D3DXMATRIX& projectionMatrix)
	{
		bool result = false;	
		
		if(!loaded)
		{
			return result;
		}

		// Render the terrain buffers using the terrain shader.
		result = pTerrain->Render(pDeviceContext, pTerrainShader, viewMatrix, projectionMatrix, pLight->GetAmbientColor(), 
					   pLight->GetDiffuseColor(), pLight->GetDirection());
		if(!result)
		{
			return false;
		}
	

		for(vector<Creeper*>::iterator i = creepersInGame.begin();i != creepersInGame.end();i++)
		{
			Creeper* pCreeper = (Creeper*)*i;
			pCreeper->Render(pDeviceContext);

			// Render the model using the light shader.
			pLightShader->Render(pDeviceContext, pCreeper->GetMesh()->GetIndexCount(), pCreeper->GetWorldMatrix(), viewMatrix, projectionMatrix, 
						pCreeper->GetMesh()->GetTexture(), pLight->GetAmbientColor(), pLight->GetDiffuseColor(), pLight->GetDirection() );
		}


		for(vector<Tower*>::iterator i = towersInGame.begin(); i != towersInGame.end();i++)
		{
			Tower* pTower = (Tower*)*i;

			pTower->Render(pDeviceContext);

			// Render the model using the light shader.
			pLightShader->Render(pDeviceContext, pTower->GetMesh()->GetIndexCount(), pTower->GetWorldMatrix(), viewMatrix, projectionMatrix, 
				pTower->GetMesh()->GetTexture(),pLight->GetAmbientColor(), pLight->GetDiffuseColor(),  pLight->GetDirection());
		}

		if(pTowerPlacement)
		{
			pTowerPlacement->Render(pDeviceContext);

			// Render the model using the light shader.
			pLightShader->Render(pDeviceContext, pTowerPlacement->GetMesh()->GetIndexCount(), pTowerPlacement->GetWorldMatrix(), viewMatrix, projectionMatrix, 
				pTowerPlacement->GetMesh()->GetTexture(),pLight->GetAmbientColor(), pLight->GetDiffuseColor(),  pLight->GetDirection());
		}


		for(vector<Projectile*>::iterator i = projectilesInGame.begin();i != projectilesInGame.end();i++)
		{
			Projectile * pProjectile = (Projectile*)*i;
			pProjectile->Render(pDeviceContext);
			pLightShader->Render(pDeviceContext, pProjectile->GetMesh()->GetIndexCount(), pProjectile->GetWorldMatrix(), viewMatrix, projectionMatrix,  pProjectile->GetMesh()->GetTexture(),pLight->GetAmbientColor(), pLight->GetDiffuseColor(), pLight->GetDirection() );
		}


		return result;
	}

	void Game::MouseLeftMove(bool leftDown,float mouseX,float mouseY, D3DXVECTOR3& cameraPosition,D3DXMATRIX& viewMatrix)
	{
		if(!loaded)
			return;
		if(leftDown)
		{
			D3DXVECTOR3 p = Intersection(mouseX, mouseY,cameraPosition,viewMatrix);

			p.x = floorf(p.x);
			p.z = floorf(p.z);

			p.y = pTerrain->GetHeight(p.x,p.z);

		

			if(pTowerPlacement == NULL)
			{
				pTowerPlacement = new Tower(*towerList.at(0));
				if(!pTowerPlacement)
				{
					return;
				}
			}
			pTowerPlacement->SetPosition(p);
			
		}
		else if(pTowerPlacement != NULL)
		{
			D3DXVECTOR3 p = pTowerPlacement->GetPosition();
			int x = floorf(p.x);
			int z = floorf(p.z);

			if(pTowerPlacement->IsOnMap() && gold >= pTowerPlacement->config.costs && pTerrain->GetBuildable(x,z) && !pTerrain->GetOccupied(x,z))
			{
				gold-= pTowerPlacement->config.costs;
				pTerrain->SetOccupied(pTowerPlacement->GetPosition().x,pTowerPlacement->GetPosition().z,true);
				towersInGame.push_back(pTowerPlacement);
				pTowerPlacement = NULL;
			}
			else
			{
				delete pTowerPlacement;
				pTowerPlacement = NULL;
			}
		}
	}

	D3DXVECTOR3 Game::Intersection(float pointX, float pointY, D3DXVECTOR3& cameraPosition,D3DXMATRIX& viewMatrix)
	{
		D3DXMATRIX inverseViewMatrix, worldMatrix, translateMatrix, inverseWorldMatrix;
		D3DXVECTOR3 direction, origin, rayOrigin, rayDirection;


		D3DXMatrixInverse(&inverseViewMatrix, NULL, &viewMatrix);

		// Calculate the direction of the picking ray in view space.
		direction.x = (pointX * inverseViewMatrix._11) + (pointY * inverseViewMatrix._21) + inverseViewMatrix._31;
		direction.y = (pointX * inverseViewMatrix._12) + (pointY * inverseViewMatrix._22) + inverseViewMatrix._32;
		direction.z = (pointX * inverseViewMatrix._13) + (pointY * inverseViewMatrix._23) + inverseViewMatrix._33;



		// Get the world matrix and translate to the location of the sphere.
		worldMatrix = pTerrain->GetWorldMatrix();

		// Now get the inverse of the translated world matrix.
		D3DXMatrixInverse(&inverseWorldMatrix, NULL, &worldMatrix);

		// Now transform the ray origin and the ray direction from view space to world space.
		D3DXVec3TransformCoord(&rayOrigin, &cameraPosition, &inverseWorldMatrix);
		D3DXVec3TransformNormal(&rayDirection, &direction, &inverseWorldMatrix);

		// Normalize the ray direction.
		D3DXVec3Normalize(&rayDirection, &rayDirection);


		D3DXVECTOR3 topLeft(-pTerrain->GetWidth()/2.0f,0,pTerrain->GetHeight()/2.0f);
		D3DXVECTOR3 topRight(pTerrain->GetWidth()/2.0f,0,pTerrain->GetHeight()/2.0f);
		D3DXVECTOR3 bottomLeft(-pTerrain->GetWidth()/2.0f,0,-pTerrain->GetHeight()/2.0f);
		D3DXVECTOR3 bottomRight(pTerrain->GetWidth()/2.0f,0,-pTerrain->GetHeight()/2.0f);


		D3DXVECTOR3 intersectionPoint;
		float u,v,dist;
		if(!D3DXIntersectTri(&topLeft, &topRight, &bottomRight,&rayOrigin,&rayDirection,&u,&v,&dist))
		{
			if(!D3DXIntersectTri(&topLeft, &bottomRight,&bottomLeft,&rayOrigin,&rayDirection,&u,&v,&dist))
			{

			}
		}

		intersectionPoint = rayOrigin + (rayDirection * dist);
		return intersectionPoint;
	}
}
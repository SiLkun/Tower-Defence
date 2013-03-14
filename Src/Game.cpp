////////////////////////////////////////////////////////////////////////////////
// Filename: Game.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{
	Game::Game()
	{
		creepers = new vector<Creeper*>();
		towers = new vector<Tower*>();
		textures = new vector<Texture*>();
		projectiles = new vector<Projectile*>();
		meshes = new vector<Mesh*>();
		sounds = new vector<Sound*>();

		pTerrainShader = 0;
		pTerrain = 0;
		pLight = 0;
		pLightShader = 0;

		time = 0.0f;
		previousWaveTime = -20.0f;
		waveDelay = 20.0f;
		int waveCount = 10;
		int level = 1;
		int gold = 0;
	}
	Game::Game(const Game&)
	{
	}
	Game::~Game()
	{
		delete creepers;
		delete towers;
	}

	float Game::GetTime()
	{
		return time;
	}

	float Game::GetLevel()
	{
		return level;
	}

	float Game::GetGold()
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
	
	bool Game::Initialize(ID3D11Device * pDevice, HWND hwnd)
	{
		bool result;
		level = 1;
		gold = 0;
		// Create the terrain object.
		pTerrain = new Terrain;
		if(!pTerrain)
		{
			return false;
		}

		result = pTerrain->Initialize(pDevice, "Data/heightmap01.bmp", "Data/legend.txt", 
						   "Data/materialmap01.bmp", "Data/colorm01.bmp");
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


		Mesh * pTowerMesh = new Mesh();
		pTowerMesh->Initialize(pDevice, "Data/Model/Tower.obj",textures);
		meshes->push_back(pTowerMesh);

		Mesh * pCreeperMesh = new Mesh();
		pCreeperMesh->Initialize(pDevice, "Data/Model/Hawk.obj",textures);
		meshes->push_back(pCreeperMesh);

		Mesh * pProjectileMesh = new Mesh();
		pProjectileMesh->Initialize(pDevice, "Data/Model/Projectile.obj",textures);
		meshes->push_back(pProjectileMesh);


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
 
		sounds->push_back(pSound);

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
 
		sounds->push_back(pSound);
		
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
 
		sounds->push_back(pSound);

		for (int i = 0; i < 5; i++)
		{
			Tower * pTower = new Tower();
			if(!pTower)
			{
				return false;
			}
			pTower->Initialize(pTowerMesh);
			
			pTower->SetPosition(-5.0f,0.0f,20.0f -(i * 5.0f));


			towers->push_back(pTower);
		}
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
		// Initialize the model object.
		return true;
	}

	void Game::Shutdown()
	{
		// Release the terrain object.
		if(pTerrain)
		{
			pTerrain->Shutdown();
			delete pTerrain;
			pTerrain = 0;
		}

		// Release the color shader object.
		if(pTerrainShader)
		{
			pTerrainShader->Shutdown();
			delete pTerrainShader;
			pTerrainShader = 0;
		}


		// Release the terrain object.
		if(pLight)
		{
			delete pLight;
			pLight = 0;
		}
		
		if(sounds)
		{
			for(vector<Sound*>::iterator iSound = sounds->begin();iSound != sounds->end();iSound++)
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

			sounds->clear();
			delete sounds;
			sounds = 0;
		}


		if(creepers)
		{
			for(UINT iCreeper = 0;iCreeper < creepers->size();iCreeper++)
			{
				Creeper* pCreeper = creepers->at(iCreeper);
				// Release the terrain object.
				if(pCreeper)
				{
					delete pCreeper;
					pCreeper = 0;
				}
			}

			creepers->clear();
			delete creepers;
			creepers = 0;
		}

		if(towers)
		{
			for(UINT iTower = 0;iTower < towers->size();iTower++)
			{
				Tower* pTower = towers->at(iTower);
				if(pTower)
				{
					delete pTower;
					pTower = 0;
				}
			}

			towers->clear();
			delete towers;
			towers = 0;
		}

		

		if(meshes)
		{
			 for(vector<Mesh*>::iterator iMesh = meshes->begin();iMesh != meshes->end();iMesh++)
			 {
				 Mesh * pMesh = (Mesh*)*iMesh;
				 pMesh->Release();
				 delete pMesh;
			 }
		}

		if(textures)
		{
			for(UINT iTexture = 0;iTexture < textures->size();iTexture++)
			{
				Texture * pTexture = textures->at(iTexture);
				// Release the terrain object.
				if(pTexture)
				{
					pTexture->Release();
					delete pTexture;
					pTexture = 0;
				}
			}

			textures->clear();
			delete textures;
			textures = 0;
		}
	}

	Terrain * Game::GetTerrain()
	{
		return pTerrain;
	}

	 
	Mesh * Game::GetMesh(string fileName)
	 {
		 for(vector<Mesh*>::iterator iMesh = meshes->begin();iMesh != meshes->end();iMesh++)
		 {
			 Mesh * pMesh = (Mesh*)*iMesh;
			 if(pMesh && pMesh->GetFilename().compare(fileName) == 0)
			 {
				 return pMesh;
			 }
		 }
		 return NULL;
	 }

	Sound * Game::GetSound(string fileName)
	{
		 for(vector<Sound*>::iterator iSound = sounds->begin();iSound != sounds->end();iSound++)
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
		bool result;
	
		time += frameTime / 1000;
		if(time > previousWaveTime + waveDelay)
		{
			previousWaveTime = time;

			Creeper * creeperType = GetWaveType();

			if(creeperType->GetBoss())
				waveCount = 3;
			else
				waveCount = 10;

			for (int i = 0; i < waveCount; i++)
			{
				// Create the model object.
				Creeper* pCreeper = new Creeper(creeperType->GetHealth(), creeperType->GetFlying(), creeperType->GetFast(), creeperType->GetBoss());

				if(!pCreeper)
				{
					return false;
				}
				pCreeper->Initialize(GetMesh("Data/Model/Hawk.obj"));

				float scale = 1.0f;
				scale += ((rand() % 20) - 10.0f) * 0.01f;

				if(creeperType->GetFlying()) {
					pCreeper->SetPosition(-1.0f,10.0f,192.0f -(i * 4.0f));
				}
				else if(creeperType->GetBoss())	{
					scale += 1 + ((rand() % 20) - 10.0f) * 0.01f;
					pCreeper->SetPosition(-1.0f,0.0f,192.0f -(i * 10.0f));
					
				}
				else {
					pCreeper->SetPosition(-1.0f,0.0f,192.0f -(i * 4.0f));	
				}

				pCreeper->SetScale(scale,scale,scale);
				creepers->push_back(pCreeper);
			}
			level++;
		}

		if(sounds)
		{
			D3DXVECTOR3 p;
			pCamera->GetPosition(p);
			for(vector<Sound*>::iterator iSound = sounds->begin();iSound != sounds->end();iSound++)
			{
				Sound* pSound = (Sound*) *iSound;
				pSound->Update(p);
			}
		}
		
		if(creepers)
		{
			for(UINT iCreeper = 0;iCreeper < creepers->size();iCreeper++)
			{
				Creeper* pCreeper = creepers->at(iCreeper);
				if(pCreeper->GetHealth() < 0.0f)
				{
					gold += level -1;
					delete pCreeper;
					pCreeper = 0;
					creepers->erase(creepers->begin() + iCreeper);
				}
			}
		}

		
		if(pLight)
		{
			result = pLight->Update(frameTime);
		}

		if(creepers)
		{
			for(UINT iCreeper = 0;iCreeper < creepers->size();iCreeper++)
			{
				Creeper* pCreeper = creepers->at(iCreeper);
				pCreeper->Update(frameTime);
				pCreeper->UpdateOnMap((float)pTerrain->GetWidth(),(float)pTerrain->GetHeight());
			}
		}

		if(towers)
		{
			for(UINT iTower = 0;iTower < towers->size();iTower++)
			{
				Tower* pTower = towers->at(iTower);
				pTower->DetermineTarget(creepers);
				pTower->Update(pDevice,time,frameTime,pTerrain,creepers,projectiles);
			}
		}

		if(projectiles)
		{
			for(UINT iProjectile = 0;iProjectile < projectiles->size();)
			{
				Projectile * pProjectile = projectiles->at(iProjectile);
				if(pProjectile->IsHit() || !pProjectile->IsOnMap())
				{
					delete pProjectile;
					pProjectile = 0;
					projectiles->erase(projectiles->begin() + iProjectile);
				}
				iProjectile++;
			}
		}


		if(projectiles)
		{
			for(UINT iProjectile = 0;iProjectile < projectiles->size();iProjectile++)
			{
				Projectile * pProjectile = projectiles->at(iProjectile);
				if(!pProjectile->GetMesh())
				{
					pProjectile->SetLaunchSound(GetSound("Data/Sound/rocket_fire.wav"));
					pProjectile->SetMoveSound(GetSound("Data/Sound/rocket_cruise.wav"));
					pProjectile->SetHitSound(GetSound("Data/Sound/rocket_explode.wav"));
					pProjectile->Initialize(GetMesh("Data/Model/Projectile.obj"));

				}
				pProjectile->Update(frameTime,creepers);
				pProjectile->UpdateOnMap((float)pTerrain->GetWidth(),(float)pTerrain->GetHeight());

			}
		}


		return result;
	}

	Creeper * Game::GetWaveType(){
		float healthmodifier = level;
		bool flying = false;
		bool fast = false;
		bool boss = false;

		// BOSS LEVEL
		if(level % 10 == 0) {
			boss = true;
		}
		// FLYING
		else if(level % 8 == 0) {
			flying = true;
		}
		// FAST
		else if(level % 5 == 0) {
			fast = true;
		}

		healthmodifier = level;

		if(boss)
			healthmodifier = level * 10;

		return new Creeper(healthmodifier, flying, fast, boss);
	}

	bool Game::Render(ID3D11DeviceContext* pDeviceContext,D3DXMATRIX viewMatrix,D3DXMATRIX projectionMatrix)
	{
		bool result;	
		
		// Render the terrain buffers using the terrain shader.
		result = pTerrain->Render(pDeviceContext, pTerrainShader, viewMatrix, projectionMatrix, pLight->GetAmbientColor(), 
					   pLight->GetDiffuseColor(), pLight->GetDirection());
		if(!result)
		{
			return false;
		}
	

		if(creepers)
		{
			for(UINT iCreeper = 0;iCreeper < creepers->size();iCreeper++)
			{
				Creeper* pCreeper = creepers->at(iCreeper);
				pCreeper->Render(pDeviceContext);

				// Render the model using the light shader.
				pLightShader->Render(pDeviceContext, pCreeper->GetMesh()->GetIndexCount(), pCreeper->GetWorldMatrix(), viewMatrix, projectionMatrix, 
						    pCreeper->GetMesh()->GetTexture(), pLight->GetAmbientColor(), pLight->GetDiffuseColor(), pLight->GetDirection() );
			}
		}

		if(towers)
		{
			for(UINT iTower = 0;iTower < towers->size();iTower++)
			{
				Tower* pTower = towers->at(iTower);

				pTower->Render(pDeviceContext);

				// Render the model using the light shader.
				pLightShader->Render(pDeviceContext, pTower->GetMesh()->GetIndexCount(), pTower->GetWorldMatrix(), viewMatrix, projectionMatrix, 
					pTower->GetMesh()->GetTexture(),pLight->GetAmbientColor(), pLight->GetDiffuseColor(),  pLight->GetDirection());
			}
		}

		if(projectiles)
		{
			for(UINT iProjectile = 0;iProjectile < projectiles->size();iProjectile++)
			{
				Projectile * pProjectile = projectiles->at(iProjectile);
				pProjectile->Render(pDeviceContext);
				pLightShader->Render(pDeviceContext, pProjectile->GetMesh()->GetIndexCount(), pProjectile->GetWorldMatrix(), viewMatrix, projectionMatrix,  pProjectile->GetMesh()->GetTexture(),pLight->GetAmbientColor(), pLight->GetDiffuseColor(), pLight->GetDirection() );
			}
		}

		return result;
	}
}
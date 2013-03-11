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
		pTerrainShader = 0;
		pTerrain = 0;
		pLight = 0;
		pLightShader = 0;

		time = 0.0f;
		previousWaveTime = -20.0f;
		waveDelay = 20.0f;
		int waveCount = 10;
		
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

		
		for (int i = 0; i < 5; i++)
		{
			Tower * pTower = new Tower();
			if(!pTower)
			{
				return false;
			}
			result = pTower->Initialize(pDevice, "data/cube.txt", L"data/Tower.dds");
			pTower->SetPosition(-5.0f,2.5f,20.0f -(i * 5.0f));

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

		if(creepers)
		{
			for(UINT iCreeper = 0;iCreeper < creepers->size();iCreeper++)
			{
				Creeper* pCreeper = creepers->at(iCreeper);
				// Release the terrain object.
				if(pCreeper)
				{
					pCreeper->Shutdown();
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
				// Release the terrain object.
				if(pTower)
				{
					pTower->Shutdown();
					delete pTower;
					pTower = 0;
				}
			}

			towers->clear();
			delete towers;
			towers = 0;
		}
	}

	Terrain * Game::GetTerrain()
	{
		return pTerrain;
	}

	bool Game::Update(ID3D11Device * pDevice,float frameTime)
	{
		bool result;
	
		time += frameTime / 1000;
		if(time > previousWaveTime + waveDelay)
		{
			previousWaveTime = time;
		
			for (int i = 0; i < 10; i++)
			{
				// Create the model object.
				Creeper* pCreeper = new Creeper;
				if(!pCreeper)
				{
					return false;
				}
				result = pCreeper->Initialize(pDevice, "data/cube.txt", L"data/Creeper.dds");
			
				pCreeper->SetPosition(-1.0f,1.5f,192.0f -(i * 4.0f));

				float scale = 1.0f;
				scale += ((rand() % 20) - 10.0f) * 0.01f;

				pCreeper->SetScale(scale,scale,scale);
				creepers->push_back(pCreeper);
			}
		}

		
		if(creepers)
		{
			for(UINT iCreeper = 0;iCreeper < creepers->size();iCreeper++)
			{
				Creeper* pCreeper = creepers->at(iCreeper);
				if(pCreeper->GetHealth() < 0.0f)
				{
					pCreeper->Shutdown();
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
				pTower->Update(pDevice,time,frameTime,creepers);
			}
		}



		return result;
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
				pLightShader->Render(pDeviceContext, pCreeper->GetIndexCount(), pCreeper->GetWorldMatrix(), viewMatrix, projectionMatrix, 
						   pCreeper->GetTexture(), pLight->GetAmbientColor(), pLight->GetDiffuseColor(), pLight->GetDirection() );
			}
		}

		if(towers)
		{
			for(UINT iTower = 0;iTower < towers->size();iTower++)
			{
				Tower* pTower = towers->at(iTower);

				pTower->Render(pDeviceContext,pLightShader,viewMatrix, projectionMatrix,pLight);

				// Render the model using the light shader.
				pLightShader->Render(pDeviceContext, pTower->GetIndexCount(), pTower->GetWorldMatrix(), viewMatrix, projectionMatrix, 
						   pTower->GetTexture(),pLight->GetAmbientColor(), pLight->GetDiffuseColor(),  pLight->GetDirection());
			}
		}

		return result;
	}
}
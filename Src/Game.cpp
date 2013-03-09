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
	}
	Game::Game(const Game&)
	{
	}
	Game::~Game()
	{
		delete creepers;
		delete towers;
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

		// Initialize the terrain object.
		result = pTerrain->Initialize(pDevice);
		if(!result)
		{
			return false;
		}

		// Create the color shader object.
		pColorShader = new ColorShader;
		if(!pColorShader)
		{
			return false;
		}

		// Initialize the color shader object.
		result = pColorShader->Initialize(pDevice, hwnd);
		if(!result)
		{
			MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
			return false;
		}

		for (int i = 0; i < 20; i++)
		{
			// Create the model object.
			Creeper* pCreeper = new Creeper;
			if(!pCreeper)
			{
				return false;
			}
			result = pCreeper->Initialize(pDevice, "data/cube.txt", L"data/seafloor.dds");
			
			pCreeper->SetPosition(-50.0f + (i *5.0f),1.0f,50.0f);

			creepers->push_back(pCreeper);
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
		if(pColorShader)
		{
			pColorShader->Shutdown();
			delete pColorShader;
			pColorShader = 0;
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
	}

	Terrain * Game::GetTerrain()
	{
		return pTerrain;
	}

	bool Game::Render(ID3D11DeviceContext* pDeviceContext,D3DXMATRIX worldMatrix,D3DXMATRIX viewMatrix,D3DXMATRIX projectionMatrix,float frameTime)
	{
		bool result;
		
		pLight->Update(frameTime);
		
		// Render the terrain buffers.
		pTerrain->Render(pDeviceContext);

		// Render the model using the color shader.
		result = pColorShader->Render(pDeviceContext, pTerrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
		if(!result)
		{
			return false;
		}
		

		if(creepers)
		{
			for(UINT iCreeper = 0;iCreeper < creepers->size();iCreeper++)
			{
				Creeper* pCreeper = creepers->at(iCreeper);

				pCreeper->Update(frameTime);
				pCreeper->Render(pDeviceContext);
				
				D3DXMATRIX modelMatrix;
				D3DXMatrixIdentity(&modelMatrix);
				D3DXMatrixTranslation(&modelMatrix,pCreeper->GetPosition()->x,pCreeper->GetPosition()->y,pCreeper->GetPosition()->z);

				// Render the model using the light shader.
				result = pLightShader->Render(pDeviceContext, pCreeper->GetIndexCount(), modelMatrix, viewMatrix, projectionMatrix, 
						   pCreeper->GetTexture(), pLight->GetDirection(), pLight->GetDiffuseColor());
			}
		}


		if(!result)
		{
			return false;
		}

		return result;
	}
}
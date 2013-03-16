////////////////////////////////////////////////////////////////////////////////
// Filename: Game.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GAME_H_
#define _GAME_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "stdafx.h"

using namespace TD;

////////////////////////////////////////////////////////////////////////////////
// Class name: Game
////////////////////////////////////////////////////////////////////////////////
class Game
{
public:
	struct GameConfig
	{
	public:
		char towerConfig[256];
		char creeperConfig[256];
		char terrainConfig[256];
	}config;



	struct CreeperConfig
	{
	public:
		char towerConfig[256];
		char creeperConfig[256];
		char terrainConfig[256];
	};

public:
	Game();
	Game(const Game&);
	~Game();
	bool Initialize(ID3D11Device*,HWND);
	void Release();
	Terrain * GetTerrain();
	bool Update(ID3D11Device * pDevice,Camera * pCamera,float frameTime);
	Creeper * GetWaveType();
	bool Render(ID3D11DeviceContext*,D3DXMATRIX&,D3DXMATRIX&);
	float GetTime();
	int GetLevel();
	int GetGold();
	float GetWaveDelay();
	float GetPreviousWaveTime();
	



	Sound * GetSound(string fileName);
	void MouseLeftMove(bool,float,float, D3DXVECTOR3&,D3DXMATRIX&);
	D3DXVECTOR3 Intersection(float mouseX, float mouseY, D3DXVECTOR3&,D3DXMATRIX&);
	GameConfig LoadCfg(char * filename);
private:
	Terrain* pTerrain;
	TerrainShader* pTerrainShader;
	
	Light* pLight;
	LightShader* pLightShader;

	vector<Tower*> towerList;
	vector<Projectile*> projectileList;

	vector<Tower*> towersInGame;
	vector<Projectile*> projectilesInGame;

	vector<Creeper*> creeperList;
	vector<Creeper*> creepersInGame;

	Tower * pTowerPlacement;

	vector<Texture*> textures;
	vector<Mesh*> meshes;
	vector<Sound*> sounds;


	float time;
	float previousWaveTime;
	float waveDelay;
	int waveCount;
	int level;
	int gold;
};

#endif
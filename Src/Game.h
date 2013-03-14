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
	Game();
	Game(const Game&);
	~Game();
	bool Initialize(ID3D11Device*,HWND);
	void Shutdown();
	Terrain * GetTerrain();
	bool Update(ID3D11Device * pDevice,Camera * pCamera,float frameTime);
	Creeper * GetWaveType();
	bool Render(ID3D11DeviceContext*,D3DXMATRIX&,D3DXMATRIX&);
	float GetTime();
	int GetLevel();
	int GetGold();
	float GetWaveDelay();
	float GetPreviousWaveTime();
	Mesh * GetMesh(string fileName);
	Sound * GetSound(string fileName);
	void MouseLeftMove(bool,float,float, D3DXVECTOR3&,D3DXMATRIX&);
	D3DXVECTOR3 Intersection(float mouseX, float mouseY, D3DXVECTOR3&,D3DXMATRIX&);
private:
	Terrain* pTerrain;
	TerrainShader* pTerrainShader;
	
	Light* pLight;
	LightShader* pLightShader;

	vector<Creeper*> * creepers;
	vector<Tower*> * towers;
	Tower * pTowerPlacement;

	vector<Texture*> * textures;
	vector<Projectile*> * projectiles;
	vector<Mesh*> * meshes;
	vector<Sound*>	* sounds;


	float time;
	float previousWaveTime;
	float waveDelay;
	int waveCount;
	int level;
	int gold;
};

#endif
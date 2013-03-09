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
	bool Render(ID3D11DeviceContext*,D3DXMATRIX,D3DXMATRIX,D3DXMATRIX,float);
private:
	Terrain* pTerrain;
	ColorShader* pColorShader;
	
	Light* pLight;
	LightShader* pLightShader;

	vector<Creeper*> * creepers;
	vector<Tower*> * towers;
};

#endif
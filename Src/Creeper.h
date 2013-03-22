////////////////////////////////////////////////////////////////////////////////
// Filename: Creeper.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CREEPER_H_
#define _CREEPER_H_

#include "stdafx.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: Creeper
////////////////////////////////////////////////////////////////////////////////
class Creeper : public Model
{
public:
	struct Config
	{
	public:
		char name[32];
		char mesh[256];
		bool flying;
		float speed;
		int health;
		bool boss;
		int gold;

		Config()
		{
		}

		Config(Config & other)
		{
			strcpy(name,other.name);
			strcpy(mesh,other.mesh);
			flying = other.flying;
			speed = other.speed;
			health = other.health;
			boss = other.boss;
			gold = other.gold;
		}
	}config;


	float fateOutTime;
	bool reachedEnding;

private:
	bool onMap;
	D3DXVECTOR3 destination;
	list<POINT> path;
public:
	static vector<Config*> * LoadCfg(char*);

	Creeper();
	Creeper(const Creeper&);
	~Creeper();
	void Initialize(ID3D11Device * pDevice, vector<Mesh *> * pMeshes,vector<Texture *> * pTextures, Config * config);
	void Update(Terrain * pTerrain,float frameTime);
	void Render(ID3D11DeviceContext* deviceContext);
	void Hit(int);
	int GetHealth();
	void SetHealth(int);
	float GetSpeed();
	void SetSpeed(float);
	void SetDestination(D3DXVECTOR3);
	bool ReachedEnding();
	bool IsFlying();
	bool IsBoss();
	void SetPath(list<POINT>);

};

#endif
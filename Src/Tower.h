////////////////////////////////////////////////////////////////////////////////
// Filename: Tower.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TOWER_H_
#define _TOWER_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "stdafx.h"


class Model;
class Creeper;
class Mesh;
class Texture;
class Projectile;
class Terrain;

////////////////////////////////////////////////////////////////////////////////
// Class name: Tower
////////////////////////////////////////////////////////////////////////////////
class Tower : public Model
{
public:
	struct Config
	{
	public:
		char name[32];
		char mesh[256];
		float reloadtime;
		float range;
		float projectileSpeed;
		int damage;
		int costs;
		Config()
		{
		}

		Config(Config & other)
		{
			strcpy(name,other.name);

			strcpy(mesh,other.mesh);
			reloadtime = other.reloadtime;
			range = other.range;
			projectileSpeed = other.projectileSpeed;
			damage = other.damage;
			costs = other.costs;
		}
	}config;
private:
	Creeper * pTarget;
	float lastAtackTime;
public:
	static vector<Config*> * LoadCfg(char*);
	Tower();
	Tower(const Tower&);
	~Tower();
	void Initialize(ID3D11Device * pDevice, vector<Mesh *> * pMeshes,vector<Texture *> * pTextures, Config * config);
	void Update(ID3D11Device*,float,float,Terrain *,vector<Creeper*>*,vector<Projectile*>*);
	void DetermineTarget(vector<Creeper*>*);
	bool HasTarget();
	void Attack(ID3D11Device*,vector<Projectile*>* pProjectile);
	vector<Projectile*> GetProjectiles();
};

#endif
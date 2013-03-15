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

////////////////////////////////////////////////////////////////////////////////
// Class name: Tower
////////////////////////////////////////////////////////////////////////////////
class Tower : public Model
{
private:
	Creeper * pTarget;
	float reloadTime;
	float lastAtackTime;
	float range;
	float projectileSpeed;
public:
	Tower();
	Tower(const Tower&);
	~Tower();
	void Update(ID3D11Device*,float,float,Terrain *,vector<Creeper*>*,vector<Projectile*>*);
	void DetermineTarget(vector<Creeper*>*);
	bool HasTarget();
	void Attack(ID3D11Device*,vector<Projectile*>* pProjectile);
	vector<Projectile*> GetProjectiles();
};

#endif
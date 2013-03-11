////////////////////////////////////////////////////////////////////////////////
// Filename: Tower.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TOWER_H_
#define _TOWER_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "stdafx.h"



////////////////////////////////////////////////////////////////////////////////
// Class name: Tower
////////////////////////////////////////////////////////////////////////////////
class Tower : public Model
{
private:
	Creeper * pTarget;
	vector<Projectile*> * projectiles;
	float reloadTime;
	float lastAtackTime;
	float range;
public:
	Tower();
	Tower(const Tower&);
	~Tower();
	void Update(ID3D11Device*,float,float);
	void Render(ID3D11DeviceContext* deviceContext, LightShader *, D3DXMATRIX,D3DXMATRIX, Light *);

	void DetermineTarget(vector<Creeper*>*);
	bool HasTarget();
	void Attack(ID3D11Device*);
	vector<Projectile*> GetProjectiles();
};

#endif
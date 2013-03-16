////////////////////////////////////////////////////////////////////////////////
// Filename: Model.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "stdafx.h"

class Model;
class Creeper;
class Mesh;

////////////////////////////////////////////////////////////////////////////////
// Class name: Projectile
////////////////////////////////////////////////////////////////////////////////
class Projectile : public Model
{
private:
	D3DXVECTOR3 direction;
	bool isHit;
	int damage;
	Sound * pLaunchSound;
	Sound * pMoveSound;
	Sound * pHitSound;

public:
	Projectile();
	Projectile(const Projectile&);
	~Projectile();
	void Initialize(Mesh * pMesh);
	bool IsHit();
	void SetTarget(D3DXVECTOR3);
	void SetDamage(int damage);
	void SetLaunchSound(Sound * pSound);
	void SetMoveSound(Sound * pSound);
	void SetHitSound(Sound * pSound);
	void Update(float,vector<Creeper*>*);
	void Render(ID3D11DeviceContext*);
};


#endif
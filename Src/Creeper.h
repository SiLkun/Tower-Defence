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
private:
	bool onMap;
	float health;
	bool flying;
	bool fast;
	bool boss;
public:
	Creeper();
	Creeper(float healthModifier, bool flying, bool fast, bool boss);
	Creeper(const Creeper&);
	~Creeper();
	void Update(float);
	void Render(ID3D11DeviceContext* deviceContext);
	void Hit(float);
	float GetHealth();
	bool IsFlying();
	bool IsFast();
	bool IsBoss();
};

#endif
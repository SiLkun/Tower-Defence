////////////////////////////////////////////////////////////////////////////////
// Filename: Light.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LIGHT_H_
#define _LIGHT_H_

//////////////
// INCLUDES //
//////////////
#include "stdafx.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: Light
////////////////////////////////////////////////////////////////////////////////
class Light
{
public:
	Light();
	Light(const Light&);
	~Light();

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);

	D3DXVECTOR4 GetAmbientColor();
	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();

	bool Update(float);
private:
	D3DXVECTOR4 ambientColor;
	D3DXVECTOR4 diffuseColor;
	D3DXVECTOR3 direction;
	float yaw;
};

#endif
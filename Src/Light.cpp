////////////////////////////////////////////////////////////////////////////////
// Filename: Light.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{

	Light::Light()
	{
	}


	Light::Light(const Light& other)
	{
	}


	Light::~Light()
	{
	}


	void Light::SetDiffuseColor(float red, float green, float blue, float alpha)
	{
		diffuseColor = D3DXVECTOR4(red, green, blue, alpha);
		return;
	}


	void Light::SetDirection(float x, float y, float z)
	{
		direction = D3DXVECTOR3(x, y, z);
		return;
	}


	D3DXVECTOR4 Light::GetDiffuseColor()
	{
		return diffuseColor;
	}


	D3DXVECTOR3 Light::GetDirection()
	{
		return direction;
	}
}
////////////////////////////////////////////////////////////////////////////////
// Filename: Light.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{

	Light::Light()
	{
		yaw = 0;
	}


	Light::Light(const Light& other)
	{
	}


	Light::~Light()
	{
	}

	bool Light::Update(float frameTime)
	{
		yaw += frameTime *0.0001f;

		D3DXMATRIX rotationMatrix;
		D3DXMatrixIdentity(&rotationMatrix);
		D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, D3DX_PI  *0.3f  , 0.0f);

		direction.x = 0;
		direction.y = 0;
		direction.z = 1;

		// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
		D3DXVec3TransformCoord(&direction, &direction, &rotationMatrix);

		return true;
	}

	void Light::SetAmbientColor(float red, float green, float blue, float alpha)
	{
		ambientColor = D3DXVECTOR4(red, green, blue, alpha);
		return;
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

	
	D3DXVECTOR4 Light::GetAmbientColor()
	{
		return ambientColor;
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
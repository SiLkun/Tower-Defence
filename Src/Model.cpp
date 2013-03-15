////////////////////////////////////////////////////////////////////////////////
// Filename: Model.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{
	Model::Model()
	{
		position.x = 0;
		position.y = 0;
		position.z = 0;

		direction.x = 0;
		direction.y = 0;
		direction.z = 0;

		acceleration.x = 0;
		acceleration.y = 0;
		acceleration.z = 0;

		scale.x = 1;
		scale.y = 1;
		scale.z = 1;

		speed = 0.0f; 


		pMesh = 0;



	}


	Model::Model(const Model& other)
	{
	}


	Model::~Model()
	{
	}

	void Model::Initialize(Mesh * pMesh)
	{
		this->pMesh = pMesh;
	}

	void Model::Update(float frameTime)
	{
		acceleration += direction * speed * frameTime;
		position += acceleration;
		D3DXMatrixIdentity(&worldMatrix);
		D3DXMatrixTranslation(&worldMatrix,position.x,position.y,position.z);
		D3DXMATRIX scaleMatrix;
		D3DXMatrixScaling (&scaleMatrix,scale.x,scale.y,scale.z);
		D3DXMatrixMultiply(&worldMatrix,&scaleMatrix,&worldMatrix);
	}

	void Model::Render(ID3D11DeviceContext* deviceContext)
	{
		if(isOnMap)
		{
			pMesh->Render(deviceContext);
		}

		return;
	}

	D3DXVECTOR3 Model::GetPosition()
	{
		return position;
	}

	void Model::SetPosition(D3DXVECTOR3 p)
	{
		position = p;
	}
	
	D3DXVECTOR3 Model::GetDirection()
	{
		return direction;
	}

	void Model::SetDirection(D3DXVECTOR3 p)
	{
		D3DXVec3Normalize(&direction,&p);
	}

	D3DXVECTOR3 Model::GetAcceleration()
	{
		return acceleration;
	}

	void Model::SetAcceleration(D3DXVECTOR3 a)
	{
		acceleration = a;
	}

	D3DXVECTOR3 Model::GetScale()
	{
		return scale;
	}

	void Model::SetScale(D3DXVECTOR3 s)
	{
		this->scale = s;
	}

	float Model::GetSpeed()
	{
		return speed;
	}

	void Model::SetSpeed(float speed)
	{
		this->speed = speed;
	}

	D3DXMATRIX Model::GetWorldMatrix()
	{
		return worldMatrix;
	}

	void Model::UpdateOnMap(float terrainWidth,float terrainHeight)
	{
		if(position.x > -terrainWidth/2 && position.x < terrainWidth/2 && position.z > -terrainHeight/2 && position.z < terrainHeight/2 )
		{
			isOnMap = true;
		}
		else
		{
			isOnMap = false;
		}

	}

	bool Model::IsOnMap()
	{
		return isOnMap;
	}

	Mesh * Model::GetMesh()
	{
		return pMesh;
	}
}
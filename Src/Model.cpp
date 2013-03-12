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

		scale.x = 1;
		scale.y = 1;
		scale.z = 1;

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

	void Model::Update()
	{
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

	D3DXVECTOR3 * Model::GetPosition()
	{
		return &position;
	}

	void Model::SetPosition(float x,float y,float z)
	{
		position.x = x;
		position.y = y;
		position.z = z;
	}
	
	D3DXVECTOR3 * Model::GetScale()
	{
		return &scale;
	}

	void Model::SetScale(float x,float y,float z)
	{
		scale.x = x;
		scale.y = y;
		scale.z = z;
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
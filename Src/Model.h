////////////////////////////////////////////////////////////////////////////////
// Filename: Model.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODEL_H_
#define _MODEL_H_

#include "stdafx.h"

class Texture;
class Mesh;

////////////////////////////////////////////////////////////////////////////////
// Class name: Model
////////////////////////////////////////////////////////////////////////////////
class Model
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 textCoord;
		D3DXVECTOR3 normal;
	};

	struct Material
	{
		D3DXVECTOR3 ambient;
		D3DXVECTOR3 diffuse;
		D3DXVECTOR3 specular;
		string texture;
	} material;

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	Mesh * pMesh;
public:
	Model();
	Model(const Model&);
	~Model();
	void Initialize(Mesh * pMesh);

	void Render(ID3D11DeviceContext*);

	D3DXVECTOR3 GetPosition();
	void SetPosition(D3DXVECTOR3);

	D3DXVECTOR3 GetDirection();
	void SetDirection(D3DXVECTOR3);

	D3DXVECTOR3 GetAcceleration();
	void SetAcceleration(D3DXVECTOR3);
	
	D3DXVECTOR3 GetScale();
	void SetScale(D3DXVECTOR3);

	float GetSpeed();
	void SetSpeed(float);

	void Update(float frameTime);
	D3DXMATRIX GetWorldMatrix();
	void UpdateOnMap(float terrainWidth,float terrainHeight);
	bool IsOnMap();
	Mesh * GetMesh();
protected:
	D3DXVECTOR3 position;
	D3DXVECTOR3 direction;
	D3DXVECTOR3 acceleration;
	D3DXVECTOR3 scale;
	float speed; 

	D3DXMATRIX worldMatrix;
	bool isOnMap;
};

#endif
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

	D3DXVECTOR3 * GetPosition();
	void SetPosition(float,float,float);
	D3DXVECTOR3 * GetScale();
	void SetScale(float,float,float);
	void Update();
	D3DXMATRIX GetWorldMatrix();
	void UpdateOnMap(float terrainWidth,float terrainHeight);
	bool IsOnMap();
	Mesh * GetMesh();
protected:
	D3DXVECTOR3 position;
	D3DXVECTOR3 scale;
	D3DXMATRIX worldMatrix;
	bool isOnMap;
};

#endif
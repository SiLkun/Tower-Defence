////////////////////////////////////////////////////////////////////////////////
// Filename: terrain.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "stdafx.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Terrain
////////////////////////////////////////////////////////////////////////////////
class Terrain
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
		D3DXVECTOR4 color;
	};

	struct HeightMapType 
	{ 
		float x, y, z;
		float nx, ny, nz;
		float r, g, b;
		int rIndex, gIndex, bIndex;
	};

	struct VectorType 
	{ 
		float x, y, z;
	};

	struct MaterialGroupType 
	{ 
		int textureIndex1, textureIndex2, alphaIndex;
		int red, green, blue;
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount;
		VertexType* vertices;
		unsigned long* indices;
	};

public:
	Terrain();
	Terrain(const Terrain&);
	~Terrain();

	bool Initialize(ID3D11Device*, char*, char*, char*, char*);
	void Shutdown();

	bool Render(ID3D11DeviceContext*, TerrainShader*, D3DXMATRIX, D3DXMATRIX, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3);
	
	D3DXVECTOR3 * GetPosition();
	void SetPosition(float,float,float);

	int GetWidth();
	int GetHeight();
	D3DXMATRIX GetWorldMatrix();
private:
	bool LoadHeightMap(char*);
	void NormalizeHeightMap();
	bool CalculateNormals();
	void ShutdownHeightMap();

	bool LoadColorMap(char*);

	bool LoadMaterialFile(char*, char*, ID3D11Device*);
	bool LoadMaterialMap(char*);
	bool LoadMaterialBuffers(ID3D11Device*);
	void ReleaseMaterials();

private:
	int terrainWidth, terrainHeight;
	HeightMapType* pHeightMap;
	int pTextureCount, pMaterialCount;
	Texture* pTextures;
	MaterialGroupType* pMaterials;
	D3DXMATRIX worldMatrix;
	D3DXVECTOR3 position;
};

#endif
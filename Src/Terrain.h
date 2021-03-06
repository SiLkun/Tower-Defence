////////////////////////////////////////////////////////////////////////////////
// Filename: terrain.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "stdafx.h"


class Texture;

////////////////////////////////////////////////////////////////////////////////
// Class name: Terrain
////////////////////////////////////////////////////////////////////////////////
class Terrain
{
private:
	struct Config
	{
	public:
		char legend[256];
		char heightmap[256];
		char materialmap[256];
		char colormap[256];
	}config;

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
		bool occupied;
		bool buildable;
		bool walkable;
		int estimatedCost;
		int moveCost;
		int totalCost;
		HeightMapType * pParent;
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

	bool Initialize(ID3D11Device*, char*);
	void Release();

	Config LoadCfg(char * filename);
	bool Render(ID3D11DeviceContext*, TerrainShader*, D3DXMATRIX, D3DXMATRIX, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3);
	
	D3DXVECTOR3 * GetPosition();
	void SetPosition(float,float,float);

	int GetWidth();
	int GetHeight();
	float GetHeight(int x,int z);
	bool GetOccupied(int x,int z);
	void SetOccupied(int x,int z,bool);
	bool GetWalkable(int x,int z);
	bool GetBuildable(int x,int z);
	D3DXMATRIX GetWorldMatrix();
	int Terrain::GetGridIndex(int x,int z);
	list<POINT>  FindPath (int posX,int posY,int destX,int destY);
	HeightMapType* NextNode (vector<HeightMapType*> * ,vector<HeightMapType*> * ,int destX,int destZ);
private:
	bool LoadHeightMap(char*);
	void NormalizeHeightMap();
	bool CalculateNormals();
	void ReleaseHeightMap();

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
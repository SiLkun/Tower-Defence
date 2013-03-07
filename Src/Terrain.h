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
	    D3DXVECTOR4 color;
	};

public:
	Terrain();
	Terrain(const Terrain&);
	~Terrain();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	
private:
	int terrainWidth, terrainHeight;
	int vertexCount, indexCount;
	ID3D11Buffer * pVertexBuffer, * pIndexBuffer;
};

#endif
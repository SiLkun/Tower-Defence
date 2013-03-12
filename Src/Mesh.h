////////////////////////////////////////////////////////////////////////////////
// Filename: Mesh.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MESH_H_
#define _MESH_H_

#include "stdafx.h"

class Texture;

////////////////////////////////////////////////////////////////////////////////
// Class name: Mesh
////////////////////////////////////////////////////////////////////////////////
class Mesh
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

	string modelFilename;
public:
	Mesh();
	Mesh(const Mesh&);
	~Mesh();
	bool Initialize(ID3D11Device*, string,vector<Texture*> *);

	void Release();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	int GetFaceCount();
	ID3D11ShaderResourceView* GetTexture();

	bool IsInitialized();
	string GetFilename();

private:
	bool LoadObj(const char* obj);
	bool LoadMaterial(const char* );

	bool InitializeBuffers(ID3D11Device*);
	bool InitializeMaterial(ID3D11Device*,vector<Texture*> *);

	void RenderBuffers(ID3D11DeviceContext*);

	void ShutdownBuffers();

	void ReleaseMaterial();
	void ReleaseModel();
protected:
	ID3D11Buffer *pVertexBuffer, * pIndexBuffer;
	int vertexCount, faceCount;
	Texture* pTexture;
	ModelType* pModel;
	VertexType* vertices;
	unsigned long* indices;
	string materialFile;
	bool isInitialized;
};

#endif
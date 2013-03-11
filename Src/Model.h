////////////////////////////////////////////////////////////////////////////////
// Filename: Model.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODEL_H_
#define _MODEL_H_

#include "stdafx.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: Model
////////////////////////////////////////////////////////////////////////////////
class Model
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
public:
	Model();
	Model(const Model&);
	~Model();
	bool Initialize(ID3D11Device*, char*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
	D3DXVECTOR3 * GetPosition();
	void SetPosition(float,float,float);
	void Update();
	D3DXMATRIX GetWorldMatrix();
private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();
protected:
	ID3D11Buffer *pVertexBuffer, * pIndexBuffer;
	int vertexCount, indexCount;
	Texture* pTexture;
	D3DXVECTOR3 position;
	ModelType* pModel;
	D3DXMATRIX worldMatrix;
	bool onMap;
};

#endif
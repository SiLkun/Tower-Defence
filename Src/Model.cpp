////////////////////////////////////////////////////////////////////////////////
// Filename: Model.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{
	Model::Model()
	{
		pVertexBuffer = 0;
		pIndexBuffer = 0;
		pTexture = 0;

		pModel = 0;
	}


	Model::Model(const Model& other)
	{
	}


	Model::~Model()
	{
	}

	bool Model::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
	{
		bool result;

		// Load in the model data,
		result = LoadModel(modelFilename);
		if(!result)
		{
			return false;
		}

		// Initialize the vertex and index buffers.
		result = InitializeBuffers(device);
		if(!result)
		{
			return false;
		}

		// Load the texture for this model.
		result = LoadTexture(device, textureFilename);
		if(!result)
		{
			return false;
		}

		return true;
	}


	void Model::Shutdown()
	{
		// Release the model texture.
		ReleaseTexture();

		// Shutdown the vertex and index buffers.
		ShutdownBuffers();

		// Release the model data.
		ReleaseModel();

		return;
	}


	void Model::Render(ID3D11DeviceContext* deviceContext)
	{
		// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
		RenderBuffers(deviceContext);

		return;
	}


	int Model::GetIndexCount()
	{
		return indexCount;
	}


	ID3D11ShaderResourceView* Model::GetTexture()
	{
		return pTexture->GetTexture();
	}


	bool Model::InitializeBuffers(ID3D11Device* device)
	{
		VertexType* vertices;
		unsigned long* indices;
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;
		int i;

		// Create the vertex array.
		vertices = new VertexType[vertexCount];
		if(!vertices)
		{
			return false;
		}

		// Create the index array.
		indices = new unsigned long[indexCount];
		if(!indices)
		{
			return false;
		}

		// Load the vertex array and index array with data.
		for(i=0; i<vertexCount; i++)
		{
			vertices[i].position = D3DXVECTOR3(pModel[i].x, pModel[i].y, pModel[i].z);
			vertices[i].texture = D3DXVECTOR2(pModel[i].tu, pModel[i].tv);
			vertices[i].normal = D3DXVECTOR3(pModel[i].nx, pModel[i].ny, pModel[i].nz);

			indices[i] = i;
		}

		// Set up the description of the static vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &pVertexBuffer);
		if(FAILED(result))
		{
			return false;
		}

		// Set up the description of the static index buffer.
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		result = device->CreateBuffer(&indexBufferDesc, &indexData, &pIndexBuffer);
		if(FAILED(result))
		{
			return false;
		}

		// Release the arrays now that the vertex and index buffers have been created and loaded.
		delete [] vertices;
		vertices = 0;

		delete [] indices;
		indices = 0;

		return true;
	}


	void Model::ShutdownBuffers()
	{
		// Release the index buffer.
		if(pIndexBuffer)
		{
			pIndexBuffer->Release();
			pIndexBuffer = 0;
		}

		// Release the vertex buffer.
		if(pVertexBuffer)
		{
			pVertexBuffer->Release();
			pVertexBuffer = 0;
		}

		return;
	}


	void Model::RenderBuffers(ID3D11DeviceContext* deviceContext)
	{
		unsigned int stride;
		unsigned int offset;


		// Set vertex buffer stride and offset.
		stride = sizeof(VertexType); 
		offset = 0;
    
		// Set the vertex buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

		// Set the index buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		return;
	}


	bool Model::LoadTexture(ID3D11Device* device, WCHAR* filename)
	{
		bool result;


		// Create the texture object.
		pTexture = new Texture;
		if(!pTexture)
		{
			return false;
		}

		// Initialize the texture object.
		result = pTexture->Initialize(device, filename);
		if(!result)
		{
			return false;
		}

		return true;
	}


	void Model::ReleaseTexture()
	{
		// Release the texture object.
		if(pTexture)
		{
			pTexture->Shutdown();
			delete pTexture;
			pTexture = 0;
		}

		return;
	}

	bool Model::LoadModel(char* filename)
	{
		ifstream fin;
		char input;
		int i;


		// Open the model file.
		fin.open(filename);
	
		// If it could not open the file then exit.
		if(fin.fail())
		{
			return false;
		}

		// Read up to the value of vertex count.
		fin.get(input);
		while(input != ':')
		{
			fin.get(input);
		}

		// Read in the vertex count.
		fin >> vertexCount;

		// Set the number of indices to be the same as the vertex count.
		indexCount = vertexCount;

		// Create the model using the vertex count that was read in.
		pModel = new ModelType[vertexCount];
		if(!pModel)
		{
			return false;
		}

		// Read up to the beginning of the data.
		fin.get(input);
		while(input != ':')
		{
			fin.get(input);
		}
		fin.get(input);
		fin.get(input);

		// Read in the vertex data.
		for(i=0; i<vertexCount; i++)
		{
			fin >> pModel[i].x >> pModel[i].y >> pModel[i].z;
			fin >> pModel[i].tu >> pModel[i].tv;
			fin >> pModel[i].nx >> pModel[i].ny >> pModel[i].nz;
		}

		// Close the model file.
		fin.close();

		return true;
	}

	void Model::ReleaseModel()
	{
		if(pModel)
		{
			delete [] pModel;
			pModel = 0;
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

}
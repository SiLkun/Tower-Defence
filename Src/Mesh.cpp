////////////////////////////////////////////////////////////////////////////////
// Filename: Mesh.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{
	Mesh::Mesh()
	{
		pVertexBuffer = 0;
		pIndexBuffer = 0;
		pTexture = 0;

		pModel = 0;

		materialFile = "";

		isInitialized = false;
		vertexCount = 0;
		faceCount = 0;
	}


	Mesh::Mesh(const Mesh& other)
	{
	}


	Mesh::~Mesh()
	{
	}

	bool Mesh::Initialize(ID3D11Device* device, string modelFilename,vector<Texture*> * pTextures)
	{
		bool result = true;
		this->modelFilename = modelFilename;

		// Load in the model data,
		result = LoadObj(modelFilename.c_str());
		if(!result)
		{
			return false;
		}

		modelFilename = modelFilename.substr(0,modelFilename.find_last_of("/"));

		// Initialize the vertex and index buffers.
		result = InitializeBuffers(device);
		if(!result)
		{
			return false;
		}

		modelFilename += "/";

		int lastSlash = materialFile.find_last_of("/");
		if(lastSlash >= 0)
		{
			materialFile = materialFile.substr(lastSlash + 1,materialFile.length() - lastSlash - 1);
		}

		modelFilename += materialFile;

		// Load the texture for this model.
		result = LoadMaterial(modelFilename.c_str());
		if(!result)
		{
			return false;
		}

		result = InitializeMaterial(device,pTextures);
		if(!result)
		{
			return false;
		}

		isInitialized = true;

		return result;
	}



	bool Mesh::LoadObj(const char* obj)
	{
		ifstream fin;
		char input;

		vertexCount = 0;
		faceCount = 0;


		int textureCount = 0;
		int normalCount = 0;
		// Open the file.
	
		fin.open(obj);

		// Check if it was successful in opening the file.
		if(fin.fail() == true)
		{
			return false;
		}

		// Read from the file and continue to read until the end of the file is reached.
		fin.get(input);
		while(!fin.eof())
		{
			// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
			if(input == 'v')
			{
				fin.get(input);
				if(input == ' ') { vertexCount++; }
				if(input == 't') { textureCount++; }
				if(input == 'n') { normalCount++; }
			}

			// If the line starts with 'f' then increment the face count.
			if(input == 'f')
			{
				fin.get(input);
				if(input == ' ') { faceCount++; }
			}
		
			// Otherwise read in the remainder of the line.
			while(input != '\n')
			{
				fin.get(input);
			}

			// Start reading the beginning of the next line.
			fin.get(input);
		}



		int vertexIndex, texcoordIndex, normalIndex,faceIndex;
		char input2;

		D3DXVECTOR3 * positions;
		D3DXVECTOR2 * textCoords;
		D3DXVECTOR3 * normals;
		
		struct FaceType
		{
			int vIndex[3];
			int tIndex[3];
			int nIndex[3];
		}*faces;

		// Initialize the four data structures.
		positions = new D3DXVECTOR3[vertexCount];
		if(!positions)
		{
			return false;
		}

		textCoords = new D3DXVECTOR2[textureCount];
		if(!textCoords)
		{
			return false;
		}

		normals = new D3DXVECTOR3[normalCount];
		if(!normals)
		{
			return false;
		}

		faces = new FaceType[faceCount];
		if(!faces)
		{
			return false;
		}

		// Initialize the indexes.
		vertexIndex = 0;
		texcoordIndex = 0;
		normalIndex = 0;
		faceIndex = 0;

		fin.clear() ;
		fin.seekg(0, ios::beg) ;

		// Check if it was successful in opening the file.
		if(fin.fail() == true)
		{
			return false;
		}

		// Read in the vertices, texture coordinates, and normals into the data structures.
		// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
		fin.get(input);
		while(!fin.eof())
		{
			if(input == 'v')
			{
				fin.get(input);

				// Read in the vertices.
				if(input == ' ') 
				{ 
					fin >> positions[vertexIndex].x >> positions[vertexIndex].y >> positions[vertexIndex].z;

					// Invert the Z vertex to change to left hand system.
					//positions[vertexIndex].z = positions[vertexIndex].z * -1.0f;
					vertexIndex++; 
				}

				// Read in the texture uv coordinates.
				if(input == 't') 
				{ 
					fin >> textCoords[texcoordIndex].x >> textCoords[texcoordIndex].y;

					// Invert the V texture coordinates to left hand system.
					//textCoords[texcoordIndex].y = 1.0f - textCoords[texcoordIndex].y;
					texcoordIndex++; 
				}

				// Read in the normals.
				if(input == 'n') 
				{ 
					fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;

					// Invert the Z normal to change to left hand system.
					//normals[normalIndex].z = normals[normalIndex].z * -1.0f;
					normalIndex++; 
				}
			}
			else if(input == 'm')
			{
				while(input != ' ')
				{
					fin.get(input);
				}

				fin.get(input);
				while(input != '\n' && input != ' ')
				{	
					materialFile += input;
					fin.get(input);
				}
			}
			else if(input == 'f') // Read in the faces.
			{
				fin.get(input);
				if(input == ' ')
				{
					// Read the face data in backwards to convert it to a left hand system from right hand system.
					fin >> faces[faceIndex].vIndex[0] >> input2 >> faces[faceIndex].tIndex[0] >> input2 >> faces[faceIndex].nIndex[0]
						>> faces[faceIndex].vIndex[1] >> input2 >> faces[faceIndex].tIndex[1] >> input2 >> faces[faceIndex].nIndex[1]
						>> faces[faceIndex].vIndex[2] >> input2 >> faces[faceIndex].tIndex[2] >> input2 >> faces[faceIndex].nIndex[2];
					faceIndex++;
				}
			}


			// Read in the remainder of the line.
			while(input != '\n')
			{
				fin.get(input);
			}



			// Start reading the beginning of the next line.
			fin.get(input);
		}

		// Close the file.
		fin.close();
		fin.clear();

		// Create the index array.
		indices = new unsigned long[faceCount*3];
		if(!indices)
		{
			return false;
		}

		vertices = new VertexType[faceCount*3];
		if(!vertices)
		{
			return false;
		}

		// Now loop through all the faces and output the three vertices for each face.
		for(int i=0; i<faceCount;i++)
		{
			for (int j = 0; j < 3; j++)
			{
				vertices[i*3+j].position = positions[faces[i].vIndex[j] - 1];
				vertices[i*3+j].textCoord = textCoords[faces[i].tIndex[j] - 1];
				vertices[i*3+j].normal = normals[faces[i].nIndex[j] - 1];
				indices[i*3+j] = i*3+j;		
			}
		}
		delete [] faces;
		faces = 0;

		delete [] positions;
		positions = 0;

		delete [] textCoords;
		textCoords = 0;

		delete [] normals;
		normals = 0;

		vertexCount = faceCount*3;
		return true;
	}

	bool Mesh::InitializeBuffers(ID3D11Device* device)
	{
		if(vertices == NULL||  indices == NULL)
			return false;

		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;

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
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * faceCount * 3;
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


	bool Mesh::LoadMaterial(const char*  mtl)
	{
		char input;

		ifstream fin;
		fin.open(mtl);
		// Check if it was successful in opening the file.
		if(fin.fail() == true)
		{
			return false;
		}

		fin.get(input);
		while(!fin.eof())
		{
			string tag = "";
			while(input != ' ')
			{
				tag += input;
				fin.get(input);
			}
			if(tag.compare("Ka") == 0)
			{
				fin >> material.ambient.x  >> material.ambient.y >> material.ambient.z;
			}

			if(tag.compare("Kd") == 0)
			{
				fin >> material.diffuse.x  >> material.diffuse.y >> material.diffuse.z;
			}
		
			if(tag.compare("Ks") == 0)
			{
				fin >> material.specular.x  >> material.specular.y >> material.specular.z;
			}
		
			if(tag.compare("map_Kd") == 0)
			{
				//material.texture = mtl;
				//material.texture = material.texture.substr(0,material.texture.find_last_of("/"));
				material.texture = "Data/Texture/";
				fin.get(input);

				string filename = "";
				while(input != '\n')
				{
					filename += input;
					fin.get(input);
				}
				int lastSlash = filename.find_last_of("\\");
				material.texture += filename.substr(lastSlash + 1, filename.length() - lastSlash -1);
			}

			while(input != '\n')
			{
				fin.get(input);
			}
			// Start reading the beginning of the next line.
			fin.get(input);
		}


		return true;
	}
	bool Mesh::InitializeMaterial(ID3D11Device* pDevice,vector<Texture*> * pTextures)
	{
		for (vector<Texture*>::iterator iterator = pTextures->begin(); iterator != pTextures->end(); iterator++)
		{
			pTexture = (Texture*)*iterator;

			if(pTexture->GetFileName().compare(material.texture) == 0)
			{
				break;
			}
			pTexture = 0;
		}

		if(pTexture == NULL)
		{
			pTexture = new Texture;
			pTextures->push_back(pTexture);
		}

		if(!pTexture)
		{
			return false;
		}

		// Load the texture or alpha map.
		bool result = pTexture->Initialize(pDevice,(char*) material.texture.c_str());
		return result;

	}


	void Mesh::Release()
	{
		// Release the model texture.
		ReleaseMaterial();

		// Release the vertex and index buffers.
		ReleaseBuffers();

		// Release the model data.
		ReleaseModel();

		return;
	}

				
	void Mesh::Render(ID3D11DeviceContext* deviceContext)
	{
		if(isInitialized)
		{
			RenderBuffers(deviceContext);
		}

		return;
	}


	int Mesh::GetIndexCount()
	{
		return faceCount *3;
	}

	int Mesh::GetFaceCount()
	{
		return faceCount;
	}


	ID3D11ShaderResourceView* Mesh::GetTexture()
	{
		if(pTexture)
			return pTexture->GetTexture();
		else 
			return NULL;
	}


	void Mesh::ReleaseBuffers()
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


	void Mesh::RenderBuffers(ID3D11DeviceContext* deviceContext)
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





	void Mesh::ReleaseMaterial()
	{
		if(pTexture)
		{
			pTexture->Release();
			pTexture = 0;

		}

		return;
	}


	void Mesh::ReleaseModel()
	{
		if(pModel)
		{
			delete [] pModel;
			pModel = 0;
		}

		return;
	}

	bool Mesh::IsInitialized()
	{
		return isInitialized;
	}

	string Mesh::GetFilename()
	{
		return modelFilename;
	}

	Mesh * Mesh::GetMesh(vector<Mesh*> * pMeshes, string fileName)
	 {
		 for(vector<Mesh*>::iterator iMesh = pMeshes->begin();iMesh != pMeshes->end();iMesh++)
		 {
			 Mesh * pMesh = (Mesh*)*iMesh;
			 if(pMesh && pMesh->GetFilename().compare(fileName) == 0)
			 {
				 return pMesh;
			 }
		 }
		 return NULL;
	 }
}
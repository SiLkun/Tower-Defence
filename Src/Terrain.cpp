////////////////////////////////////////////////////////////////////////////////
// Filename: Terrain.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{

	Terrain::Terrain()
	{
		pHeightMap = 0;
		pTextures = 0;
		pMaterials = 0;

		position.x = 0;
		position.y = 0;
		position.z = 0;

	}



	Terrain::Terrain(const Terrain& other)
	{

	}


	Terrain::~Terrain()
	{
	}


	bool Terrain::Initialize(ID3D11Device* device, char* filename)
	{
		bool result;

		config = LoadCfg(filename);

		// Load in the height map for the terrain.
		result = LoadHeightMap(config.heightmap);
		if(!result)
		{
			return false;
		}

		// Normalize the height of the height map.
		NormalizeHeightMap();

		// Calculate the normals for the terrain data.
		result = CalculateNormals();
		if(!result)
		{
			return false;
		}

		// Load in the color map for the terrain.
		result = LoadColorMap(config.colormap);
		if(!result)
		{
			return false;
		}

		// Initialize the material groups for the terrain.
		result = LoadMaterialFile(config.legend, config.materialmap, device);
		if(!result)
		{
			return false;
		}

		D3DXMatrixIdentity(&worldMatrix);
		D3DXMatrixTranslation(&worldMatrix,position.x,position.y,position.z);
		return true;
	}



	void Terrain::Release()
	{
		// Release the materials for the terrain.
		ReleaseMaterials();

		// Release the height map data.
		ReleaseHeightMap();
		return;
	}

	Terrain::Config Terrain::LoadCfg(char * filename)
	{
		Config config;

		ifstream fin;

		fin.open(filename);
		if(fin.fail())
		{
			return config;
		}

		char input[256];
		while(!fin.eof())
		{
			fin >> input;
			if(strcmp(input,"legend") == 0)
			{
				fin >> config.legend;
			}
			else if(strcmp(input,"heightmap") == 0)
			{
				fin >> config.heightmap;
			}
			else if(strcmp(input,"materialmap") == 0)
			{
				fin >> config.materialmap;
			}
			else if(strcmp(input,"colormap") == 0)
			{
				fin >> config.colormap;
			}
		}

		return config;
	}

	bool Terrain::LoadHeightMap(char* filename)
	{
		FILE* filePtr;
		int error;
		unsigned int count;
		BITMAPFILEHEADER bitmapFileHeader;
		BITMAPINFOHEADER bitmapInfoHeader;
		int imageSize, i, j, k, index;
		unsigned char* bitmapImage;
		unsigned char height;


		// Open the height map file in binary.
		error = fopen_s(&filePtr, filename, "rb");
		if(error != 0)
		{
			return false;
		}

		// Read in the file header.
		count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
		if(count != 1)
		{
			return false;
		}

		// Read in the bitmap info header.
		count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
		if(count != 1)
		{
			return false;
		}

		// Save the dimensions of the terrain.
		terrainWidth = bitmapInfoHeader.biWidth;
		terrainHeight = bitmapInfoHeader.biHeight;

		// Calculate the size of the bitmap image data.
		imageSize = terrainWidth * terrainHeight * 3;

		// Allocate memory for the bitmap image data.
		bitmapImage = new unsigned char[imageSize];
		if(!bitmapImage)
		{
			return false;
		}

		// Move to the beginning of the bitmap data.
		fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

		// Read in the bitmap image data.
		count = fread(bitmapImage, 1, imageSize, filePtr);
		if(count != imageSize)
		{
			return false;
		}

		// Close the file.
		error = fclose(filePtr);
		if(error != 0)
		{
			return false;
		}

		// Create the structure to hold the height map data.
		pHeightMap = new HeightMapType[terrainWidth * terrainHeight];
		if(!pHeightMap)
		{
			return false;
		}

		// Initialize the position in the image data buffer.
		k=0;

		// Read the image data into the height map.
		for(j=0; j<terrainHeight; j++)
		{
			for(i=0; i<terrainWidth; i++)
			{
				height = bitmapImage[k] / 20.0f;
			
				index = (terrainHeight * j) + i;

				pHeightMap[index].x = (float)i - terrainWidth/2;
				pHeightMap[index].y = (float)height;
				pHeightMap[index].z = (float)j  - terrainHeight/2;

				pHeightMap[index].buildable = 1;
				pHeightMap[index].walkable = 1;
				pHeightMap[index].occupied = 0;

				pHeightMap[index].pParent = 0;
				k+=3;
			}
		}

		// Release the bitmap image data.
		delete [] bitmapImage;
		bitmapImage = 0;

		return true;
	}


	void Terrain::NormalizeHeightMap()
	{
		int i, j;


		for(j=0; j<terrainHeight; j++)
		{
			for(i=0; i<terrainWidth; i++)
			{
				pHeightMap[(terrainHeight * j) + i].y /= 15.0f;
			}
		}

		return;
	}


	bool Terrain::CalculateNormals()
	{
		int i, j, index1, index2, index3, index, count;
		float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
		VectorType* normals;


		// Create a temporary array to hold the un-normalized normal vectors.
		normals = new VectorType[(terrainHeight-1) * (terrainWidth-1)];
		if(!normals)
		{
			return false;
		}

		// Go through all the faces in the mesh and calculate their normals.
		for(j=0; j<(terrainHeight-1); j++)
		{
			for(i=0; i<(terrainWidth-1); i++)
			{
				index1 = (j * terrainHeight) + i;
				index2 = (j * terrainHeight) + (i+1);
				index3 = ((j+1) * terrainHeight) + i;

				// Get three vertices from the face.
				vertex1[0] = pHeightMap[index1].x;
				vertex1[1] = pHeightMap[index1].y;
				vertex1[2] = pHeightMap[index1].z;
		
				vertex2[0] = pHeightMap[index2].x;
				vertex2[1] = pHeightMap[index2].y;
				vertex2[2] = pHeightMap[index2].z;
		
				vertex3[0] = pHeightMap[index3].x;
				vertex3[1] = pHeightMap[index3].y;
				vertex3[2] = pHeightMap[index3].z;

				// Calculate the two vectors for this face.
				vector1[0] = vertex1[0] - vertex3[0];
				vector1[1] = vertex1[1] - vertex3[1];
				vector1[2] = vertex1[2] - vertex3[2];
				vector2[0] = vertex3[0] - vertex2[0];
				vector2[1] = vertex3[1] - vertex2[1];
				vector2[2] = vertex3[2] - vertex2[2];

				index = (j * (terrainHeight-1)) + i;

				// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
				normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
				normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
				normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);
			}
		}

		// Now go through all the vertices and take an average of each face normal 	
		// that the vertex touches to get the averaged normal for that vertex.
		for(j=0; j<terrainHeight; j++)
		{
			for(i=0; i<terrainWidth; i++)
			{
				// Initialize the sum.
				sum[0] = 0.0f;
				sum[1] = 0.0f;
				sum[2] = 0.0f;

				// Initialize the count.
				count = 0;

				// Bottom left face.
				if(((i-1) >= 0) && ((j-1) >= 0))
				{
					index = ((j-1) * (terrainHeight-1)) + (i-1);

					sum[0] += normals[index].x;
					sum[1] += normals[index].y;
					sum[2] += normals[index].z;
					count++;
				}

				// Bottom right face.
				if((i < (terrainWidth-1)) && ((j-1) >= 0))
				{
					index = ((j-1) * (terrainHeight-1)) + i;

					sum[0] += normals[index].x;
					sum[1] += normals[index].y;
					sum[2] += normals[index].z;
					count++;
				}

				// Upper left face.
				if(((i-1) >= 0) && (j < (terrainHeight-1)))
				{
					index = (j * (terrainHeight-1)) + (i-1);

					sum[0] += normals[index].x;
					sum[1] += normals[index].y;
					sum[2] += normals[index].z;
					count++;
				}

				// Upper right face.
				if((i < (terrainWidth-1)) && (j < (terrainHeight-1)))
				{
					index = (j * (terrainHeight-1)) + i;

					sum[0] += normals[index].x;
					sum[1] += normals[index].y;
					sum[2] += normals[index].z;
					count++;
				}
			
				// Take the average of the faces touching this vertex.
				sum[0] = (sum[0] / (float)count);
				sum[1] = (sum[1] / (float)count);
				sum[2] = (sum[2] / (float)count);

				// Calculate the length of this normal.
				length = sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));
			
				// Get an index to the vertex location in the height map array.
				index = (j * terrainHeight) + i;

				// Normalize the final shared normal for this vertex and store it in the height map array.
				pHeightMap[index].nx = (sum[0] / length);
				pHeightMap[index].ny = (sum[1] / length);
				pHeightMap[index].nz = (sum[2] / length);
			}
		}

		// Release the temporary normals.
		delete [] normals;
		normals = 0;

		return true;
	}


	void Terrain::ReleaseHeightMap()
	{
		if(pHeightMap)
		{
			delete [] pHeightMap;
			pHeightMap = 0;
		}

		return;
	}


	bool Terrain::LoadColorMap(char* filename)
	{
		int error, imageSize, i, j, k, index, colorMapWidth, colorMapHeight;
		FILE* filePtr;
		unsigned int count;
		BITMAPFILEHEADER bitmapFileHeader;
		BITMAPINFOHEADER bitmapInfoHeader;
		unsigned char* bitmapImage;


		// Open the color map file in binary.
		error = fopen_s(&filePtr, filename, "rb");
		if(error != 0)
		{
			return false;
		}

		// Read in the file header.
		count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
		if(count != 1)
		{
			return false;
		}

		// Read in the bitmap info header.
		count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
		if(count != 1)
		{
			return false;
		}

		// Make sure the color map dimensions are the same as the terrain dimensions for easy 1 to 1 mapping.
		colorMapWidth = bitmapInfoHeader.biWidth;
		colorMapHeight = bitmapInfoHeader.biHeight;

		if((colorMapWidth != terrainWidth) || (colorMapHeight != terrainHeight))
		{
			return false;
		}

		// Calculate the size of the bitmap image data.
		imageSize = colorMapWidth * colorMapHeight * 3;

		// Allocate memory for the bitmap image data.
		bitmapImage = new unsigned char[imageSize];
		if(!bitmapImage)
		{
			return false;
		}

		// Move to the beginning of the bitmap data.
		fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

		// Read in the bitmap image data.
		count = fread(bitmapImage, 1, imageSize, filePtr);
		if(count != imageSize)
		{
			return false;
		}

		// Close the file.
		error = fclose(filePtr);
		if(error != 0)
		{
			return false;
		}

		// Initialize the position in the image data buffer.
		k=0;

		// Read the image data into the color map portion of the height map structure.
		for(j=0; j<colorMapHeight; j++)
		{
			for(i=0; i<colorMapWidth; i++)
			{
				index = (colorMapHeight * j) + i;

				pHeightMap[index].b = (float)bitmapImage[k]   / 255.0f;
				pHeightMap[index].g = (float)bitmapImage[k+1] / 255.0f;
				pHeightMap[index].r = (float)bitmapImage[k+2] / 255.0f;

				k+=3;
			}
		}

		// Release the bitmap image data.
		delete [] bitmapImage;
		bitmapImage = 0;

		return true;
	}

	bool Terrain::LoadMaterialFile(char* filename, char* materialMapFilename, ID3D11Device* device)
	{
		ifstream fin;
		char input;
		int i;
		char inputFilename[128];
		bool result;


		// Open the materials information text file.
		fin.open(filename);
		if(fin.fail())
		{
			return false;
		}

		// Read up to the value of texture count.
		fin.get(input);
		while(input != ':')
		{
			fin.get(input);
		}

		// Read in the texture count.
		fin >> pTextureCount;

		// Create the texture object array.
		pTextures = new Texture[pTextureCount];
		if(!pTextures)
		{
			return false;
		}

		// Load each of the textures in.
		for(i=0; i<pTextureCount; i++)
		{
			fin.get(input);
			while(input != ':')
			{
				fin.get(input);
			}

			fin >> inputFilename;



			// Load the texture or alpha map.
			result = pTextures[i].Initialize(device, inputFilename);
			if(!result)
			{
				return false;
			}
		}

		// Read up to the value of the material count.
		fin.get(input);
		while(input != ':')
		{
			fin.get(input);
		}

		// Read in the material count.
		fin >> pMaterialCount;

		// Create the material group array.
		pMaterials = new MaterialGroupType[pMaterialCount];
		if(!pMaterials)
		{
			return false;
		}

		// Initialize the material group array.
		for(i=0; i<pMaterialCount; i++)
		{
			pMaterials[i].vertexBuffer = 0;
			pMaterials[i].indexBuffer = 0;
			pMaterials[i].vertices = 0;
			pMaterials[i].indices = 0;
		}

		// Load each of the material group indexes in.
		for(i=0; i<pMaterialCount; i++)
		{
			fin.get(input);
			while(input != ':')
			{
				fin.get(input);
			}

			fin >> pMaterials[i].textureIndex1 >> pMaterials[i].textureIndex2 >> pMaterials[i].alphaIndex;
			fin >> pMaterials[i].red >> pMaterials[i].green >> pMaterials[i].blue;
		}

		// Close the materials information text file.
		fin.close();
	
		// Now load the material index map.
		result = LoadMaterialMap(materialMapFilename);
		if(!result)
		{
			return false;
		}

		// Load the vertex buffer for each material group with the terrain data.
		result = LoadMaterialBuffers(device);
		if(!result)
		{
			return false;
		}

		return true;
	}

	bool Terrain::LoadMaterialMap(char* filename)
	{
		int error, imageSize, i, j, k, index;
		FILE* filePtr;
		unsigned int count;
		BITMAPFILEHEADER bitmapFileHeader;
		BITMAPINFOHEADER bitmapInfoHeader;
		unsigned char* bitmapImage;


		// Open the material map file in binary.
		error = fopen_s(&filePtr, filename, "rb");
		if(error != 0)
		{
			return false;
		}

		// Read in the file header.
		count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
		if(count != 1)
		{
			return false;
		}

		// Read in the bitmap info header.
		count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
		if(count != 1)
		{
			return false;
		}

		// Make sure the material index map dimensions are the same as the terrain dimensions for 1 to 1 mapping.
		if((bitmapInfoHeader.biWidth != terrainWidth) || (bitmapInfoHeader.biHeight != terrainHeight))
		{
			return false;
		}

		// Calculate the size of the bitmap image data.
		imageSize = bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight * 3;

		// Allocate memory for the bitmap image data.
		bitmapImage = new unsigned char[imageSize];
		if(!bitmapImage)
		{
			return false;
		}

		// Move to the beginning of the bitmap data.
		fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

		// Read in the bitmap image data.
		count = fread(bitmapImage, 1, imageSize, filePtr);
		if(count != imageSize)
		{
			return false;
		}

		// Close the file.
		error = fclose(filePtr);
		if(error != 0)
		{
			return false;
		}

		// Initialize the position in the image data buffer so each vertice has an material index associated with it.
		k=0;

		// Read the material index data into the height map structure.
		for(j=0; j<terrainHeight; j++)
		{
			for(i=0; i<terrainWidth; i++)
			{
				index = (terrainHeight * j) + i;

				pHeightMap[index].rIndex = (int)bitmapImage[k+2];
				pHeightMap[index].gIndex = (int)bitmapImage[k+1];
				pHeightMap[index].bIndex = (int)bitmapImage[k];

				k+=3;
			}
		}

		// Release the bitmap image data.
		delete [] bitmapImage;
		bitmapImage = 0;

		return true;
	}

	bool Terrain::LoadMaterialBuffers(ID3D11Device* device)
	{
		int maxVertexCount, maxIndexCount, i, j, index1, index2, index3, index4, redIndex, greenIndex, blueIndex, index, vIndex;
		bool found;
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;


		// Create the value for the maximum number of vertices a material group could possibly have.
		maxVertexCount = (terrainWidth - 1) * (terrainHeight - 1) * 6;

		// Set the index count to the same as the maximum vertex count.
		maxIndexCount = maxVertexCount;

		// Initialize vertex and index arrays for each material group to the maximum size.
		for(i=0; i<pMaterialCount; i++)
		{
			// Create the temporary vertex array for this material group.
			pMaterials[i].vertices = new VertexType[maxVertexCount];
			if(!pMaterials[i].vertices)
			{
				return false;
			}

			// Create the temporary index array for this material group.
			pMaterials[i].indices = new unsigned long[maxIndexCount];
			if(!pMaterials[i].indices)
			{
				return false;
			}

			// Initialize the counts to zero.
			pMaterials[i].vertexCount = 0;
			pMaterials[i].indexCount = 0;
		}

		// Now loop through the terrain and build the vertex arrays for each material group.
		for(j=0; j<(terrainHeight-1); j++)
		{
			for(i=0; i<(terrainWidth-1); i++)
			{
				index1 = (terrainHeight * j) + i;          // Bottom left.
				index2 = (terrainHeight * j) + (i+1);      // Bottom right.
				index3 = (terrainHeight * (j+1)) + i;      // Upper left.
				index4 = (terrainHeight * (j+1)) + (i+1);  // Upper right.

				// Query the upper left corner vertex for the material index.
				redIndex   = pHeightMap[index3].rIndex;
				greenIndex = pHeightMap[index3].gIndex;
				blueIndex  = pHeightMap[index3].bIndex;

				// Find which material group this vertex belongs to.
				index = 0;
				found = false;
				while(!found)
				{
					if((redIndex == pMaterials[index].red) && (greenIndex == pMaterials[index].green) && (blueIndex == pMaterials[index].blue))
					{
						found = true;
					}
					else if(pMaterialCount == index +1)
					{
						found = true;
					}
					else
					{
						index++;
					}

					
				}

				// Set the index position in the vertex and index array to the count.
				vIndex = pMaterials[index].vertexCount;

				// Upper left.
				pMaterials[index].vertices[vIndex].position = D3DXVECTOR3(pHeightMap[index3].x, pHeightMap[index3].y, pHeightMap[index3].z);
				pMaterials[index].vertices[vIndex].texture = D3DXVECTOR2(0.0f, 0.0f);
				pMaterials[index].vertices[vIndex].normal = D3DXVECTOR3(pHeightMap[index3].nx, pHeightMap[index3].ny, pHeightMap[index3].nz);
				pMaterials[index].vertices[vIndex].color = D3DXVECTOR4(pHeightMap[index3].r, pHeightMap[index3].g, pHeightMap[index3].b, 1.0f);
				pMaterials[index].indices[vIndex] = vIndex;
				vIndex++;

				// Upper right.
				pMaterials[index].vertices[vIndex].position = D3DXVECTOR3(pHeightMap[index4].x, pHeightMap[index4].y, pHeightMap[index4].z);
				pMaterials[index].vertices[vIndex].texture = D3DXVECTOR2(1.0f, 0.0f);
				pMaterials[index].vertices[vIndex].normal = D3DXVECTOR3(pHeightMap[index4].nx, pHeightMap[index4].ny, pHeightMap[index4].nz);
				pMaterials[index].vertices[vIndex].color = D3DXVECTOR4(pHeightMap[index4].r, pHeightMap[index4].g, pHeightMap[index4].b, 1.0f);
				pMaterials[index].indices[vIndex] = vIndex;
				vIndex++;

				// Bottom left.
				pMaterials[index].vertices[vIndex].position = D3DXVECTOR3(pHeightMap[index1].x, pHeightMap[index1].y, pHeightMap[index1].z);
				pMaterials[index].vertices[vIndex].texture = D3DXVECTOR2(0.0f, 1.0f);
				pMaterials[index].vertices[vIndex].normal = D3DXVECTOR3(pHeightMap[index1].nx, pHeightMap[index1].ny, pHeightMap[index1].nz);
				pMaterials[index].vertices[vIndex].color = D3DXVECTOR4(pHeightMap[index1].r, pHeightMap[index1].g, pHeightMap[index1].b, 1.0f);
				pMaterials[index].indices[vIndex] = vIndex;
				vIndex++;

				// Bottom left.
				pMaterials[index].vertices[vIndex].position = D3DXVECTOR3(pHeightMap[index1].x, pHeightMap[index1].y, pHeightMap[index1].z);
				pMaterials[index].vertices[vIndex].texture = D3DXVECTOR2(0.0f, 1.0f);
				pMaterials[index].vertices[vIndex].normal = D3DXVECTOR3(pHeightMap[index1].nx, pHeightMap[index1].ny, pHeightMap[index1].nz);
				pMaterials[index].vertices[vIndex].color = D3DXVECTOR4(pHeightMap[index1].r, pHeightMap[index1].g, pHeightMap[index1].b, 1.0f);
				pMaterials[index].indices[vIndex] = vIndex;
				vIndex++;

				// Upper right.
				pMaterials[index].vertices[vIndex].position = D3DXVECTOR3(pHeightMap[index4].x, pHeightMap[index4].y, pHeightMap[index4].z);
				pMaterials[index].vertices[vIndex].texture = D3DXVECTOR2(1.0f, 0.0f);
				pMaterials[index].vertices[vIndex].normal = D3DXVECTOR3(pHeightMap[index4].nx, pHeightMap[index4].ny, pHeightMap[index4].nz);
				pMaterials[index].vertices[vIndex].color = D3DXVECTOR4(pHeightMap[index4].r, pHeightMap[index4].g, pHeightMap[index4].b, 1.0f);
				pMaterials[index].indices[vIndex] = vIndex;
				vIndex++;

				// Bottom right.
				pMaterials[index].vertices[vIndex].position = D3DXVECTOR3(pHeightMap[index2].x, pHeightMap[index2].y, pHeightMap[index2].z);
				pMaterials[index].vertices[vIndex].texture = D3DXVECTOR2(1.0f, 1.0f);
				pMaterials[index].vertices[vIndex].normal = D3DXVECTOR3(pHeightMap[index2].nx, pHeightMap[index2].ny, pHeightMap[index2].nz);
				pMaterials[index].vertices[vIndex].color = D3DXVECTOR4(pHeightMap[index2].r, pHeightMap[index2].g, pHeightMap[index2].b, 1.0f);
				pMaterials[index].indices[vIndex] = vIndex;
				vIndex++;

				// Increment the vertex and index array counts.
				pMaterials[index].vertexCount += 6;
				pMaterials[index].indexCount += 6;
			}
		}

		// Now create the vertex and index buffers from the vertex and index arrays for each material group.
		for(i=0; i<pMaterialCount; i++)
		{
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.ByteWidth = sizeof(VertexType) * pMaterials[i].vertexCount;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = 0;
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.StructureByteStride = 0;

			vertexData.pSysMem = pMaterials[i].vertices;
			vertexData.SysMemPitch = 0;
			vertexData.SysMemSlicePitch = 0;

			if(pMaterials[i].vertexCount > 0)
			{
				result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &pMaterials[i].vertexBuffer);
				if(FAILED(result))
				{
					return false;
				}
			

				indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
				indexBufferDesc.ByteWidth = sizeof(unsigned long) * pMaterials[i].indexCount;
				indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
				indexBufferDesc.CPUAccessFlags = 0;
				indexBufferDesc.MiscFlags = 0;
				indexBufferDesc.StructureByteStride = 0;

				indexData.pSysMem = pMaterials[i].indices;
				indexData.SysMemPitch = 0;
				indexData.SysMemSlicePitch = 0;

				result = device->CreateBuffer(&indexBufferDesc, &indexData, &pMaterials[i].indexBuffer);
				if(FAILED(result))
				{
					return false;
				}
			}
			delete [] pMaterials[i].vertices;
			pMaterials[i].vertices = 0;

			delete [] pMaterials[i].indices;
			pMaterials[i].indices = 0;
		}

		return true;
	}

	void Terrain::ReleaseMaterials()
	{
		int i;


		// Release the material groups.
		if(pMaterials)
		{
			for(i=0; i<pMaterialCount; i++)
			{
				if(pMaterials[i].vertexBuffer)
				{
					pMaterials[i].vertexBuffer->Release();
					pMaterials[i].vertexBuffer = 0;
				}

				if(pMaterials[i].indexBuffer)
				{
					pMaterials[i].indexBuffer->Release();
					pMaterials[i].indexBuffer = 0;
				}

				if(pMaterials[i].vertices)
				{
					delete [] pMaterials[i].vertices;
					pMaterials[i].vertices = 0;
				}

				if(pMaterials[i].indices)
				{
					delete [] pMaterials[i].indices;
					pMaterials[i].indices = 0;
				}
			}

			delete [] pMaterials;
			pMaterials = 0;
		}

		// Release the terrain textures and alpha maps.
		if(pTextures)
		{
			for(i=0; i<pTextureCount; i++)
			{
				pTextures[i].Release();
			}

			delete [] pTextures;
			pTextures = 0;
		}

		return;
	}

	bool Terrain::Render(ID3D11DeviceContext* deviceContext, TerrainShader* shader, D3DXMATRIX viewMatrix,
				  D3DXMATRIX projectionMatrix, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 lightDirection)
	{
		unsigned int stride;
		unsigned int offset;
		int i;
		bool result;


		// Set vertex buffer stride and offset.
		stride = sizeof(VertexType); 
		offset = 0;

		// Set the shader parameters that it will use for rendering.
		result = shader->SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, ambientColor, diffuseColor, lightDirection);
		if(!result)
		{
			return false;
		}

		// Set the type of primitive that should be rendered from the vertex buffers, in this case triangles.
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Render each material group.
		for(i=0; i<pMaterialCount; i++)
		{
			// Set the vertex buffer to active in the input assembler so it can be rendered.
			deviceContext->IASetVertexBuffers(0, 1, &pMaterials[i].vertexBuffer, &stride, &offset);

			// Set the index buffer to active in the input assembler so it can be rendered.
			deviceContext->IASetIndexBuffer(pMaterials[i].indexBuffer, DXGI_FORMAT_R32_UINT, 0);

			// If the material group has a valid second texture index then this is a blended terrain polygon.
			if(pMaterials[i].textureIndex2 != -1)
			{
				result = shader->SetShaderTextures(deviceContext,  pTextures[pMaterials[i].textureIndex1].GetTexture(),  
								   pTextures[pMaterials[i].textureIndex2].GetTexture(),  pTextures[pMaterials[i].alphaIndex].GetTexture(),
								   true);
			}

			// If not then it is just a single textured polygon.
			else
			{
				result = shader->SetShaderTextures(deviceContext, pTextures[pMaterials[i].textureIndex1].GetTexture(), NULL, NULL, false);
			}

			// Check if the textures were set or not.
			if(!result)
			{
				return false;
			}

			// Now render the prepared buffers with the shader.
			shader->RenderShader(deviceContext, pMaterials[i].indexCount);
		}

		return true;
	}

	
	D3DXVECTOR3 * Terrain::GetPosition()
	{
		return &position;
	}

	void Terrain::SetPosition(float x,float y,float z)
	{
		position.x = x;
		position.y = y;
		position.z = z;
	}

	int  Terrain::GetWidth()
	{
		return terrainWidth;
	}

	int  Terrain::GetHeight()
	{
		return terrainHeight;
	}

	float Terrain::GetHeight(int x,int z)
	{
		x += terrainWidth/2;
		z += terrainHeight/2;

		float value = 0.0f;

		if(0 <= x && x < terrainWidth && 0 <= z && z < terrainHeight)
		{
			int index = (terrainHeight * z) + x;
			value = pHeightMap[index].y;
		}
		return value;
	}

	bool Terrain::GetOccupied(int x,int z)
	{
		x += terrainWidth/2;
		z += terrainHeight/2;

		bool value = false;
		if(0 <= x && x < terrainWidth && 0 <= z && z < terrainHeight)
		{
			int index = (terrainHeight * z) + x;
			value = pHeightMap[index].occupied;
		}
		return value;
	}

	void Terrain::SetOccupied(int x,int z,bool occupied)
	{
		x += terrainWidth/2;
		z += terrainHeight/2;

		if(0 <= x && x < terrainWidth && 0 <= z && z < terrainHeight)
		{
			int index = (terrainHeight * z) + x;
			pHeightMap[index].occupied = occupied;
		}
	}

	
	bool Terrain::GetBuildable(int x,int z)
	{
		x += terrainWidth/2;
		z += terrainHeight/2;

		bool value = false;
		if(0 <= x && x < terrainWidth && 0 <= z && z < terrainHeight)
		{
			int index = (terrainHeight * z) + x;
			value = pHeightMap[index].buildable;
		}
		return value;
	}
	
	bool Terrain::GetWalkable(int x,int z)
	{
		x += terrainWidth/2;
		z += terrainHeight/2;

		bool value = false;
		if(0 <= x && x < terrainWidth && 0 <= z && z < terrainHeight)
		{
			int index = (terrainHeight * z) + x;
			value = pHeightMap[index].walkable;
		}
		return value;
	}

	int Terrain::GetGridIndex(int x,int z)
	{
		x += terrainWidth/2;
		z += terrainHeight/2;

		return (terrainHeight * z) + x;
	}

	D3DXMATRIX Terrain::GetWorldMatrix()
	{
		return worldMatrix;
	}



	//-----------------------------------------------------------------------------
	// Name: FindPath
	// Desc: Finds a path using A*
	//-----------------------------------------------------------------------------
	list<POINT>  Terrain::FindPath (int posX,int posZ,int destX,int destZ)
	{
		vector<HeightMapType*> openNodes;
		vector<HeightMapType*> closedNodes;

		int index;
		for(int j=0; j<terrainHeight; j++)
		{
			for(int i=0; i<terrainWidth; i++)
			{	
				index = (terrainHeight * j) + i;
				pHeightMap[index].pParent = 0;
			}
		}


		HeightMapType * node = &pHeightMap[GetGridIndex(posX,posZ)];
		node->moveCost = 0;
		node->pParent = 0;
		openNodes.push_back(node);

		//for (int i = 0; i < 10; i++)
		{
			node = NextNode(&openNodes,&closedNodes,destX,destZ);
		}

		

		list<POINT> path;

		while(node)
		{
			POINT p;
			p.x = node->x;
			p.y = node->z;
			path.push_front(p);
			node = node->pParent;
		}

		return path;
	}



	//-----------------------------------------------------------------------------
	// Name: NextNode
	// Desc: checks next node using A*
	//-----------------------------------------------------------------------------
	Terrain::HeightMapType *   Terrain::NextNode (vector<HeightMapType*> * pOpenNodes,vector<HeightMapType*> * pClosedNodes,int destX,int destZ)
	{
		list<Terrain::HeightMapType *> * path = NULL;

		HeightMapType * node = (HeightMapType *) pOpenNodes->front();
		pOpenNodes->erase(pOpenNodes->begin());
		pClosedNodes->push_back(node);

		if((int)node->x != destX || (int)node->z != destZ)
		{

			int directions[8][3] = {{0,1,10},{1,1,14},{-1,1,14},{1,0,10},{-1,0,10},{-1,-1,14},{1,-1,14},{0,-1,10}};
		
		
			for (int i = 0; i < 8; i++)
			{
				int index = GetGridIndex(node->x + directions[i][0],node->z + directions[i][1]);

				if(0 < index && index < terrainWidth * terrainHeight)
				{
					HeightMapType * nextNode = &pHeightMap[index];
					
					int alreadyClosed = false;
					for (vector<HeightMapType*>::iterator it = pClosedNodes->begin(); it != pClosedNodes->end(); it++)
					{
						HeightMapType * closedNode = (HeightMapType*)*it;
						if((int)nextNode->x == (int)closedNode->x && (int)nextNode->z == (int)closedNode->z)
						{
							alreadyClosed = true;
							break;
						}
					}
					
					
					
					
					if(!alreadyClosed && nextNode->walkable && !nextNode->occupied)
					{
						int moveCost = node->moveCost + directions[i][2];
						int estimatedCost = abs(destX - nextNode->x) + abs(destZ - nextNode->z) * 10;
						int totalCost = moveCost + estimatedCost;
						
						bool shorter = true;
						if(nextNode->pParent && nextNode->pParent->totalCost < node->totalCost)
						{
							shorter = false;
						}

						if(shorter)
						{
							nextNode->moveCost = moveCost;
							nextNode->estimatedCost = estimatedCost;
							nextNode->totalCost = totalCost;
							nextNode->pParent = node;
					
							vector<HeightMapType*>::iterator it = pOpenNodes->begin();
							int alreadyAdded = false;
							for (; it != pOpenNodes->end(); it++)
							{
								HeightMapType * p = (HeightMapType*)*it;
								if((int)nextNode->x == (int)p->x && (int)nextNode->z == (int)p->z)
								{
									nextNode = NULL;
									break;
								}
							}


						

							if(nextNode)
							{			
								for (vector<HeightMapType*>::iterator it = pOpenNodes->begin(); it != pOpenNodes->end(); it++)
								{
									HeightMapType * insertNodeHere = (HeightMapType*)*it;
									if(nextNode->estimatedCost  < insertNodeHere->estimatedCost)
									{
									
										pOpenNodes->insert(it,nextNode);
										nextNode = NULL;
										break;
									}
								}
							}

							if(nextNode)
							{
								pOpenNodes->push_back(nextNode);
							}
						}
					}
				}
			}

			bool result = false;
			vector<HeightMapType*>::iterator i = pOpenNodes->begin();
			for (; i != pOpenNodes->end(); i++)
			{
				node = NextNode(pOpenNodes,pClosedNodes,destX,destZ);

				if(node)
					break;
			}
		}

		return node;
	}
}
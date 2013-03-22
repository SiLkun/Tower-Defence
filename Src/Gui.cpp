///////////////////////////////////////////////////////////////////////////////
// Filename: textclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{

	Gui::Gui()
	{
		pFont = 0;
		pFontShader = 0;

		pFpsSentence = 0;
		pCpuSentence = 0;
		pGoldSentence = 0;
		pLevelSentence = 0;
		pTimeSentence = 0;
		pNextWaveTimeSentence = 0;

		pVertexBuffer = 0;
		pIndexBuffer = 0;
		pTexture = 0;
		pTextureLoadScreen = 0;
	}


	Gui::Gui(const Gui& other)
	{
	}


	Gui::~Gui()
	{
	}



	bool Gui::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, 
							   D3DXMATRIX baseViewMatrix)
	{
		bool result;


		// Store the screen width and height.
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;

		// Store the base view matrix.
		pBaseViewMatrix = baseViewMatrix;

		// Create the font object.
		pFont = new Font;
		if(!pFont)
		{
			return false;
		}

		// Initialize the font object.
		result = pFont->Initialize(device, "Data/Config/font.cfg", "Data/Texture/font.dds");
		if(!result)
		{
			MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
			return false;
		}

		// Create the font shader object.
		pFontShader = new FontShader;
		if(!pFontShader)
		{
			return false;
		}

		// Initialize the font shader object.
		result = pFontShader->Initialize(device, hwnd);
		if(!result)
		{
			MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
			return false;
		}

		// Initialize the first sentence.
		result = InitializeSentence(&pCpuSentence, 16, device);
		if(!result)
		{
			return false;
		}

		// Initialize the first sentence.
		result = InitializeSentence(&pFpsSentence, 16, device);
		if(!result)
		{
			return false;
		}

		result = InitializeSentence(&pTimeSentence, 32, device);
		if(!result)
		{
			return false;
		}

		result = InitializeSentence(&pGoldSentence, 32, device);
		if(!result)
		{
			return false;
		}

		result = InitializeSentence(&pLevelSentence, 32, device);
		if(!result)
		{
			return false;
		}

		result = InitializeSentence(&pNextWaveTimeSentence, 64, device);
		if(!result)
		{
			return false;
		}

		result = InitializeSentence(&pLivesSentence, 64, device);
		if(!result)
		{
			return false;
		}
		
		
		VertexType* vertices;
		unsigned long* indices;
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		int i;

		// Set the number of vertices in the vertex array.
		vertexCount = 6;

		// Set the number of indices in the index array.
		indexCount = vertexCount;

		// Create the vertex array.
		vertices = new VertexType[vertexCount];
		if(!vertices)
		{
			return false;
		}

		// Calculate the screen coordinates of the left side of the bitmap.
		float left = (float)((screenWidth / 2) * -1) ;

		// Calculate the screen coordinates of the right side of the bitmap.
		float right = left + 1920.0f;

		// Calculate the screen coordinates of the top of the bitmap.
		float top = (float)(screenHeight / 2);

		// Calculate the screen coordinates of the bottom of the bitmap.
		float bottom = top - 1080.0f;

		// Load the vertex array with data.
		// First triangle.
		vertices[0].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
		vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

		vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
		vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

		vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);  // Bottom left.
		vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

		// Second triangle.
		vertices[3].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
		vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

		vertices[4].position = D3DXVECTOR3(right, top, 0.0f);  // Top right.
		vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

		vertices[5].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
		vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);


		// Create the index array.
		indices = new unsigned long[indexCount];
		if(!indices)
		{
			return false;
		}

		// Initialize vertex array to zeros at first.
		//memset(vertices, 0, (sizeof(VertexType) * vertexCount));

		// Load the index array with data.
		for(i=0; i<indexCount; i++)
		{
			indices[i] = i;
		}

		// Set up the description of the static vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		device->CreateBuffer(&vertexBufferDesc, &vertexData, &pVertexBuffer);
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
		device->CreateBuffer(&indexBufferDesc, &indexData, &pIndexBuffer);

		pTexture = new Texture();
		pTexture->Initialize(device,"Data/Texture/Interface.png");

		pTextureLoadScreen = new Texture();
		pTextureLoadScreen->Initialize(device,"Data/Texture/LoadScreen.png");

		pTextureGameOverScreen = new Texture();
		pTextureGameOverScreen->Initialize(device,"Data/Texture/GameOver.jpg");

		
		// Release the arrays now that the vertex and index buffers have been created and loaded.
		delete [] vertices;
		vertices = 0;

		delete [] indices;
		indices = 0;
	
		return true;
	}


	void Gui::Release()
	{
		// Release the first sentence.
		ReleaseSentence(&pFpsSentence);

		// Release the second sentence.
		ReleaseSentence(&pCpuSentence);


		ReleaseSentence(&pGoldSentence);
		ReleaseSentence(&pLevelSentence);


		ReleaseSentence(&pTimeSentence);

		ReleaseSentence(&pNextWaveTimeSentence);
		ReleaseSentence(&pLivesSentence);

		
		// Release the font shader object.
		if(pFontShader)
		{
			pFontShader->Release();
			delete pFontShader;
			pFontShader = 0;
		}

		// Release the font object.
		if(pFont)
		{
			pFont->Release();
			delete pFont;
			pFont = 0;
		}

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


		if(pTexture)
		{
			pTexture->Release();
			delete pTexture;
			pTexture = 0;
		}

		if(pTextureLoadScreen)
		{
			pTextureLoadScreen->Release();
			delete pTextureLoadScreen;
			pTextureLoadScreen = 0;
		}
		
		return;
	}


	bool Gui::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
	{
		bool result;

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

		// Create a pixel color vector with the input sentence color.
		D3DXVECTOR4 pixelColor = D3DXVECTOR4(1.0f,1.0f,1.0f, 1.0f);

		// Render the text using the font shader.
		result = pFontShader->Render(deviceContext, indexCount, worldMatrix, pBaseViewMatrix, orthoMatrix, pTexture->GetTexture(), pixelColor);

		if(!result)
		{
			false;
		}

		// Draw the first sentence.
		result = RenderSentence(deviceContext, pCpuSentence, worldMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}

		// Draw the second sentence.
		result = RenderSentence(deviceContext, pFpsSentence, worldMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}


		result = RenderSentence(deviceContext, pTimeSentence, worldMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}

		result = RenderSentence(deviceContext, pLevelSentence, worldMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}

		result = RenderSentence(deviceContext, pGoldSentence, worldMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}

		result = RenderSentence(deviceContext, pNextWaveTimeSentence, worldMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}
		
		result = RenderSentence(deviceContext, pLivesSentence, worldMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}
		

		return true;
	}

	bool Gui::RenderLoadScreen(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
	{
		bool result;

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

		// Create a pixel color vector with the input sentence color.
		D3DXVECTOR4 pixelColor = D3DXVECTOR4(1.0f,1.0f,1.0f, 1.0f);

		// Render the text using the font shader.
		result = pFontShader->Render(deviceContext, indexCount, worldMatrix, pBaseViewMatrix, orthoMatrix, pTextureLoadScreen->GetTexture(), pixelColor);



		return result;
	}

	bool Gui::RenderGameOver(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
	{
		bool result;

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

		// Create a pixel color vector with the input sentence color.
		D3DXVECTOR4 pixelColor = D3DXVECTOR4(1.0f,1.0f,1.0f, 1.0f);

		// Render the text using the font shader.
		result = pFontShader->Render(deviceContext, indexCount, worldMatrix, pBaseViewMatrix, orthoMatrix, pTextureGameOverScreen->GetTexture(), pixelColor);



		return result;
	}
	bool Gui::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
	{
		VertexType* vertices;
		unsigned long* indices;
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;
		int i;


		// Create a new sentence object.
		*sentence = new SentenceType;
		if(!*sentence)
		{
			return false;
		}

		// Initialize the sentence buffers to null.
		(*sentence)->vertexBuffer = 0;
		(*sentence)->indexBuffer = 0;

		// Set the maximum length of the sentence.
		(*sentence)->maxLength = maxLength;

		// Set the number of vertices in the vertex array.
		(*sentence)->vertexCount = 6 * maxLength;

		// Set the number of indexes in the index array.
		(*sentence)->indexCount = (*sentence)->vertexCount;

		// Create the vertex array.
		vertices = new VertexType[(*sentence)->vertexCount];
		if(!vertices)
		{
			return false;
		}

		// Create the index array.
		indices = new unsigned long[(*sentence)->indexCount];
		if(!indices)
		{
			return false;
		}

		// Initialize vertex array to zeros at first.
		memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));

		// Initialize the index array.
		for(i=0; i<(*sentence)->indexCount; i++)
		{
			indices[i] = i;
		}

		// Set up the description of the dynamic vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Create the vertex buffer.
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
		if(FAILED(result))
		{
			return false;
		}

		// Set up the description of the static index buffer.
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
		if(FAILED(result))
		{
			return false;
		}

		// Release the vertex array as it is no longer needed.
		delete [] vertices;
		vertices = 0;

		// Release the index array as it is no longer needed.
		delete [] indices;
		indices = 0;

		return true;
	}


	bool Gui::UpdateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue,
								   ID3D11DeviceContext* deviceContext)
	{
		int numLetters;
		VertexType* vertices;
		float drawX, drawY;
		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		VertexType* verticesPtr;


		// Store the color of the sentence.
		sentence->red = red;
		sentence->green = green;
		sentence->blue = blue;

		// Get the number of letters in the sentence.
		numLetters = (int)strlen(text);

		// Check for possible buffer overflow.
		if(numLetters > sentence->maxLength)
		{
			return false;
		}

		// Create the vertex array.
		vertices = new VertexType[sentence->vertexCount];
		if(!vertices)
		{
			return false;
		}

		// Initialize vertex array to zeros at first.
		memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));

		// Calculate the X and Y pixel position on the screen to start drawing to.
		drawX = (float)(((screenWidth / 2) * -1) + positionX);
		drawY = (float)((screenHeight / 2) - positionY);

		// Use the font class to build the vertex array from the sentence text and sentence draw location.
		pFont->BuildVertexArray((void*)vertices, text, drawX, drawY);

		// Lock the vertex buffer so it can be written to.
		result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if(FAILED(result))
		{
			return false;
		}

		// Get a pointer to the data in the vertex buffer.
		verticesPtr = (VertexType*)mappedResource.pData;

		// Copy the data into the vertex buffer.
		memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence->vertexCount));

		// Unlock the vertex buffer.
		deviceContext->Unmap(sentence->vertexBuffer, 0);

		// Release the vertex array as it is no longer needed.
		delete [] vertices;
		vertices = 0;

		return true;
	}


	void Gui::ReleaseSentence(SentenceType** sentence)
	{
		if(*sentence)
		{
			// Release the sentence vertex buffer.
			if((*sentence)->vertexBuffer)
			{
				(*sentence)->vertexBuffer->Release();
				(*sentence)->vertexBuffer = 0;
			}

			// Release the sentence index buffer.
			if((*sentence)->indexBuffer)
			{
				(*sentence)->indexBuffer->Release();
				(*sentence)->indexBuffer = 0;
			}

			// Release the sentence.
			delete *sentence;
			*sentence = 0;
		}

		return;
	}


	bool Gui::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix, 
								   D3DXMATRIX orthoMatrix)
	{
		unsigned int stride, offset;
		D3DXVECTOR4 pixelColor;
		bool result;


		// Set vertex buffer stride and offset.
		stride = sizeof(VertexType); 
		offset = 0;

		// Set the vertex buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

		// Set the index buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Create a pixel color vector with the input sentence color.
		pixelColor = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.0f);

		// Render the text using the font shader.
		result = pFontShader->Render(deviceContext, sentence->indexCount, worldMatrix, pBaseViewMatrix, orthoMatrix, pFont->GetTexture(), 
									  pixelColor);
		if(!result)
		{
			false;
		}

		return true;
	}


	bool Gui::SetFps(int fps, ID3D11DeviceContext* deviceContext)
	{
		char tempString[16];
		char fpsString[16];
		float red, green, blue;
		bool result;


		// Truncate the fps to below 10,000.
		if(fps > 9999)
		{
			fps = 9999;
		}

		// Convert the fps integer to string format.
		_itoa_s(fps, tempString, 10);

		// Setup the fps string.
		strcpy_s(fpsString, "Fps: ");
		strcat_s(fpsString, tempString);

		// If fps is 60 or above set the fps color to green.
		if(fps >= 60)
		{
			red = 0.0f;
			green = 1.0f;
			blue = 0.0f;
		}

		// If fps is below 60 set the fps color to yellow.
		if(fps < 60)
		{
			red = 1.0f;
			green = 1.0f;
			blue = 0.0f;
		}

		// If fps is below 30 set the fps color to red.
		if(fps < 30)
		{
			red = 1.0f;
			green = 0.0f;
			blue = 0.0f;
		}

		// Update the sentence vertex buffer with the new string information.
		result = UpdateSentence(pFpsSentence, fpsString, 20, 40, red, green, blue, deviceContext);
		if(!result)
		{
			return false;
		}

		return true;
	}

	bool Gui::SetCpu(int cpu, ID3D11DeviceContext* deviceContext)
	{
		char tempString[16];
		char cpuString[16];
		bool result;


		// Convert the cpu integer to string format.
		_itoa_s(cpu, tempString, 10);

		// Setup the cpu string.
		strcpy_s(cpuString, "Cpu: ");
		strcat_s(cpuString, tempString);
		strcat_s(cpuString, "%");

		// Update the sentence vertex buffer with the new string information.
		result = UpdateSentence(pCpuSentence, cpuString, 20, 55, 0.0f, 1.0f, 0.0f, deviceContext);
		if(!result)
		{
			return false;
		}

		return true;
	}

	bool Gui::SetTime(int time, ID3D11DeviceContext* deviceContext)
	{
		char timeString[32];
		float red, green, blue;
		bool result;

		int seconds = time % 60;
		time /=60;
		int minutes = time % 60;
		time /=60;
		int hours = time % 60;


		
		char tempString[3];

		strcpy_s(timeString, "Time: ");
		_itoa_s(hours, tempString, 10);
		strcat_s(timeString, tempString);
		strcat_s(timeString, ":");
		_itoa_s(minutes, tempString, 10);
		strcat_s(timeString, tempString);
		strcat_s(timeString, ":");
		_itoa_s(seconds, tempString, 10);
		strcat_s(timeString, tempString);

		red = 1.0f;
		green = 1.0f;
		blue = 1.0f;


		// Update the sentence vertex buffer with the new string information.
		result = UpdateSentence(pTimeSentence, timeString, 20, 70, red, green, blue, deviceContext);
		if(!result)
		{
			return false;
		}

		return true;
	}

	bool Gui::SetLevel(int level, ID3D11DeviceContext* deviceContext)
	{
		char levelString[32];
		float red, green, blue;
		bool result;

		char tempString[3];

		strcpy_s(levelString, "Level: ");
		_itoa_s(level, tempString, 10);
		strcat_s(levelString, tempString);
		
		red = 0.0f;
		green = 1.0f;
		blue = 1.0f;


		// Update the sentence vertex buffer with the new string information.
		result = UpdateSentence(pLevelSentence, levelString, 20, 85, red, green, blue, deviceContext);
		if(!result)
		{
			return false;
		}

		return true;
	}

	bool Gui::SetGold(int gold, ID3D11DeviceContext* deviceContext)
	{
		char goldString[32];
		float red, green, blue;
		bool result;

		char tempString[16];

		strcpy_s(goldString, "Gold: ");
		_itoa_s(gold, tempString, 10);
		strcat_s(goldString, tempString);
		
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;


		// Update the sentence vertex buffer with the new string information.
		result = UpdateSentence(pGoldSentence, goldString, 20, 105, red, green, blue, deviceContext);
		if(!result)
		{
			return false;
		}

		return true;
	}

	bool Gui::SetNextWaveTime(int time, ID3D11DeviceContext* deviceContext)
	{
		char timeString[64];
		float red, green, blue;
		bool result;

		int seconds = time % 60;
		time /=60;
		int minutes = time % 60;
		time /=60;
		int hours = time % 60;


		
		char tempString[4];

		strcpy_s(timeString, "Next Wave Coming in: ");
		_itoa_s(hours, tempString, 10);
		strcat_s(timeString, tempString);
		strcat_s(timeString, ":");
		_itoa_s(minutes, tempString, 10);
		strcat_s(timeString, tempString);
		strcat_s(timeString, ":");
		_itoa_s(seconds, tempString, 10);
		strcat_s(timeString, tempString);

		red = 1.0f;
		green = 1.0f;
		blue = 1.0f;


		// Update the sentence vertex buffer with the new string information.
		result = UpdateSentence(pNextWaveTimeSentence, timeString, 20, 135, red, green, blue, deviceContext);
		if(!result)
		{
			return false;
		}

		return true;
	}

	bool Gui::SetLivesLeft(int livesLeft, ID3D11DeviceContext* deviceContext)
	{
		char timeString[64];
		float red, green, blue;
		bool result;
		
		char tempString[32];

		strcpy_s(timeString, "Lives Left:");
		_itoa_s(livesLeft, tempString, 10);
		strcat_s(timeString, tempString);

		red = 1.0f;
		green = 1.0f;
		blue = 1.0f;


		// Update the sentence vertex buffer with the new string information.
		result = UpdateSentence(pLivesSentence, timeString, 20, 150, red, green, blue, deviceContext);
		if(!result)
		{
			return false;
		}

		return true;
	}

}
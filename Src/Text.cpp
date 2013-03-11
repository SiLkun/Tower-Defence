///////////////////////////////////////////////////////////////////////////////
// Filename: textclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{

	Text::Text()
	{
		pFont = 0;
		pFontShader = 0;

		pFpsSentence = 0;
		pCpuSentence = 0;
		pVideocardName = 0;
		pVideocardMemory = 0;
		pTimeSentence = 0;
		pNextWaveTimeSentence = 0;

	}


	Text::Text(const Text& other)
	{
	}


	Text::~Text()
	{
	}



	bool Text::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, 
							   D3DXMATRIX baseViewMatrix)
	{
		bool result;


		// Store the screen width and height.
		pScreenWidth = screenWidth;
		pScreenHeight = screenHeight;

		// Store the base view matrix.
		pBaseViewMatrix = baseViewMatrix;

		// Create the font object.
		pFont = new Font;
		if(!pFont)
		{
			return false;
		}

		// Initialize the font object.
		result = pFont->Initialize(device, "Data/fontdata.txt", L"Data/font.dds");
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

		result = InitializeSentence(&pVideocardName, 1024, device);
		if(!result)
		{
			return false;
		}

		result = InitializeSentence(&pVideocardMemory, 1024, device);
		if(!result)
		{
			return false;
		}
		result = InitializeSentence(&pTimeSentence, 32, device);
		if(!result)
		{
			return false;
		}

		result = InitializeSentence(&pNextWaveTimeSentence, 64, device);
		if(!result)
		{
			return false;
		}
		
		return true;
	}


	void Text::Shutdown()
	{
		// Release the first sentence.
		ReleaseSentence(&pFpsSentence);

		// Release the second sentence.
		ReleaseSentence(&pCpuSentence);

		ReleaseSentence(&pVideocardName);

		ReleaseSentence(&pVideocardMemory);

		// Release the font shader object.
		if(pFontShader)
		{
			pFontShader->Shutdown();
			delete pFontShader;
			pFontShader = 0;
		}

		// Release the font object.
		if(pFont)
		{
			pFont->Shutdown();
			delete pFont;
			pFont = 0;
		}

		return;
	}


	bool Text::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
	{
		bool result;


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

		result = RenderSentence(deviceContext, pVideocardName, worldMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}

		result = RenderSentence(deviceContext, pVideocardMemory, worldMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}

		result = RenderSentence(deviceContext, pTimeSentence, worldMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}

		result = RenderSentence(deviceContext, pNextWaveTimeSentence, worldMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}
		
		return true;
	}

	bool Text::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
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


	bool Text::UpdateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue,
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
		drawX = (float)(((pScreenWidth / 2) * -1) + positionX);
		drawY = (float)((pScreenHeight / 2) - positionY);

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


	void Text::ReleaseSentence(SentenceType** sentence)
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


	bool Text::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix, 
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

	bool Text::SetVideoCardInfo(char* videoCardName, int videoCardMemory, ID3D11DeviceContext* deviceContext)
{
	char dataString[150];
	bool result;
	char tempString[16];
	char memoryString[32];


	// Setup the video card name string.
	strcpy_s(dataString, "Video Card: ");
	strcat_s(dataString, videoCardName);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(pVideocardName, dataString, 10, 10, 1.0f, 1.0f, 1.0f, deviceContext);
	if(!result)
	{
		return false;
	}

	// Truncate the memory value to prevent buffer over flow.
	if(videoCardMemory > 9999999)
	{
		videoCardMemory = 9999999;
	}

	// Convert the video memory integer value to a string format.
	_itoa_s(videoCardMemory, tempString, 10);

	// Setup the video memory string.
	strcpy_s(memoryString, "Video Memory: ");
	strcat_s(memoryString, tempString);
	strcat_s(memoryString, " MB");

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(pVideocardMemory, memoryString, 10, 25, 1.0f, 1.0f, 1.0f, deviceContext);
	if(!result)
	{
		return false;
	}

	return true;
}


	bool Text::SetFps(int fps, ID3D11DeviceContext* deviceContext)
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

	bool Text::SetCpu(int cpu, ID3D11DeviceContext* deviceContext)
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

	bool Text::SetTime(int time, ID3D11DeviceContext* deviceContext)
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

	bool Text::SetNextWaveTime(int time, ID3D11DeviceContext* deviceContext)
	{
		char timeString[64];
		float red, green, blue;
		bool result;

		int seconds = time % 60;
		time /=60;
		int minutes = time % 60;
		time /=60;
		int hours = time % 60;


		
		char tempString[3];

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
		result = UpdateSentence(pNextWaveTimeSentence, timeString, 20, 95, red, green, blue, deviceContext);
		if(!result)
		{
			return false;
		}

		return true;
	}
	
}
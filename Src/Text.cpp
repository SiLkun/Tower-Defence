///////////////////////////////////////////////////////////////////////////////
// Filename: textclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{

	Text::Text()
	{
		pFont = 0;
		sentence1 = 0;
		sentence2 = 0;
		sentence3 = 0;
		sentence4 = 0;
		sentence5 = 0;
		sentence6 = 0;
		sentence7 = 0;
		sentence8 = 0;
		sentence9 = 0;
		sentence10 = 0;
	}


	Text::Text(const Text& other)
	{
	}


	Text::~Text()
	{
	}


	bool Text::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, HWND hwnd, int screenWidth, int screenHeight, 
							   D3DXMATRIX baseViewMatrix)
	{
		bool result;


		// Store the screen width and height for calculating pixel location during the sentence updates.
		screenWidth = screenWidth;
		screenHeight = screenHeight;

		// Store the base view matrix for 2D text rendering.
		this->baseViewMatrix = baseViewMatrix;

		// Create the font object.
		pFont = new Font;
		if(!pFont)
		{
			return false;
		}

		// Initialize the font object.
		result = pFont->Initialize(pDevice, "Data/fontdata.txt", L"Data/font.dds");
		if(!result)
		{
			MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
			return false;
		}

		// Initialize the first sentence.
		result = InitializeSentence(&sentence1, 150, pDevice);
		if(!result)
		{
			return false;
		}

		// Initialize the second sentence.
		result = InitializeSentence(&sentence2, 32, pDevice);
		if(!result)
		{
			return false;
		}

		// Initialize the third sentence.
		result = InitializeSentence(&sentence3, 16, pDevice);
		if(!result)
		{
			return false;
		}

		// Initialize the fourth sentence.
		result = InitializeSentence(&sentence4, 16, pDevice);
		if(!result)
		{
			return false;
		}

		// Initialize the fifth sentence.
		result = InitializeSentence(&sentence5, 16, pDevice);
		if(!result)
		{
			return false;
		}

		// Initialize the sixth sentence.
		result = InitializeSentence(&sentence6, 16, pDevice);
		if(!result)
		{
			return false;
		}

		// Initialize the seventh sentence.
		result = InitializeSentence(&sentence7, 16, pDevice);
		if(!result)
		{
			return false;
		}

		// Initialize the eighth sentence.
		result = InitializeSentence(&sentence8, 16, pDevice);
		if(!result)
		{
			return false;
		}

		// Initialize the ninth sentence.
		result = InitializeSentence(&sentence9, 16, pDevice);
		if(!result)
		{
			return false;
		}

		// Initialize the tenth sentence.
		result = InitializeSentence(&sentence10, 16, pDevice);
		if(!result)
		{
			return false;
		}

		return true;
	}


	void Text::Shutdown()
	{
		// Release the font object.
		if(pFont)
		{
			pFont->Shutdown();
			delete pFont;
			pFont = 0;
		}

		// Release the sentences.
		ReleaseSentence(&sentence1);
		ReleaseSentence(&sentence2);
		ReleaseSentence(&sentence3);
		ReleaseSentence(&sentence4);
		ReleaseSentence(&sentence5);
		ReleaseSentence(&sentence6);
		ReleaseSentence(&sentence7);
		ReleaseSentence(&sentence8);
		ReleaseSentence(&sentence9);
		ReleaseSentence(&sentence10);

		return;
	}


	bool Text::Render(ID3D11DeviceContext* pDeviceContext, FontShader* FontShader, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
	{
		bool result;


		// Draw the sentences.
		result = RenderSentence(sentence1, pDeviceContext, FontShader, worldMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}

		result = RenderSentence(sentence2, pDeviceContext, FontShader, worldMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}

		result = RenderSentence(sentence3, pDeviceContext, FontShader, worldMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}

		result = RenderSentence(sentence4, pDeviceContext, FontShader, worldMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}

		result = RenderSentence(sentence5, pDeviceContext, FontShader, worldMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}

		result = RenderSentence(sentence6, pDeviceContext, FontShader, worldMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}

		result = RenderSentence(sentence7, pDeviceContext, FontShader, worldMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}

		result = RenderSentence(sentence8, pDeviceContext, FontShader, worldMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}

		result = RenderSentence(sentence9, pDeviceContext, FontShader, worldMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}

		result = RenderSentence(sentence10, pDeviceContext, FontShader, worldMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}

		return true;
	}


	bool Text::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* pDevice)
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
		result = pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
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
		result = pDevice->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
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


	bool Text::UpdateSentence(SentenceType* sentence, char* pText, int positionX, int positionY, float red, float green, float blue,
								   ID3D11DeviceContext* pDeviceContext)
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
		numLetters = (int)strlen(pText);

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
		pFont->BuildVertexArray((void*)vertices, pText, drawX, drawY);

		// Lock the vertex buffer so it can be written to.
		result = pDeviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if(FAILED(result))
		{
			return false;
		}

		// Get a pointer to the data in the vertex buffer.
		verticesPtr = (VertexType*)mappedResource.pData;

		// Copy the data into the vertex buffer.
		memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence->vertexCount));

		// Unlock the vertex buffer.
		pDeviceContext->Unmap(sentence->vertexBuffer, 0);

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


	bool Text::RenderSentence(SentenceType* sentence, ID3D11DeviceContext* pDeviceContext, FontShader* FontShader, D3DXMATRIX worldMatrix, 
								   D3DXMATRIX orthoMatrix)
	{
		unsigned int stride, offset;
		D3DXVECTOR4 pixelColor;
		bool result;


		// Set vertex buffer stride and offset.
		stride = sizeof(VertexType); 
		offset = 0;

		// Set the vertex buffer to active in the input assembler so it can be rendered.
		pDeviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

		// Set the index buffer to active in the input assembler so it can be rendered.
		pDeviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Create a pixel color vector with the input sentence color.
		pixelColor = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.0f);

		// Render the text using the font shader.
		result = FontShader->Render(pDeviceContext, sentence->indexCount, worldMatrix, baseViewMatrix, orthoMatrix, pFont->GetTexture(), pixelColor);
		if(!result)
		{
			false;
		}

		return true;
	}


	bool Text::SetVideoCardInfo(char* videoCardName, int videoCardMemory, ID3D11DeviceContext* pDeviceContext)
	{
		char dataString[150];
		bool result;
		char tempString[16];
		char memoryString[32];


		// Setup the video card name string.
		strcpy_s(dataString, "Video Card: ");
		strcat_s(dataString, videoCardName);

		// Update the sentence vertex buffer with the new string information.
		result = UpdateSentence(sentence1, dataString, 10, 10, 1.0f, 1.0f, 1.0f, pDeviceContext);
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
		result = UpdateSentence(sentence2, memoryString, 10, 30, 1.0f, 1.0f, 1.0f, pDeviceContext);
		if(!result)
		{
			return false;
		}

		return true;
	}


	bool Text::SetFps(int fps, ID3D11DeviceContext* pDeviceContext)
	{
		char tempString[16];
		char fpsString[16];
		bool result;


		// Truncate the fps to prevent a buffer over flow.
		if(fps > 9999)
		{
			fps = 9999;
		}

		// Convert the fps integer to string format.
		_itoa_s(fps, tempString, 10);

		// Setup the fps string.
		strcpy_s(fpsString, "Fps: ");
		strcat_s(fpsString, tempString);

		// Update the sentence vertex buffer with the new string information.
		result = UpdateSentence(sentence3, fpsString, 10, 70, 0.0f, 1.0f, 0.0f, pDeviceContext);
		if(!result)
		{
			return false;
		}

		return true;
	}


	bool Text::SetCpu(int cpu, ID3D11DeviceContext* pDeviceContext)
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
		result = UpdateSentence(sentence4, cpuString, 10, 90, 0.0f, 1.0f, 0.0f, pDeviceContext);
		if(!result)
		{
			return false;
		}

		return true;
	}


	bool Text::SetCameraPosition(float posX, float posY, float posZ, ID3D11DeviceContext* pDeviceContext)
	{
		int positionX, positionY, positionZ;
		char tempString[16];
		char dataString[16];
		bool result;


		// Convert the position from floating point to integer.
		positionX = (int)posX;
		positionY = (int)posY;
		positionZ = (int)posZ;

		// Truncate the position if it exceeds either 9999 or -9999.
		if(positionX > 9999) { positionX = 9999; }
		if(positionY > 9999) { positionY = 9999; }
		if(positionZ > 9999) { positionZ = 9999; }

		if(positionX < -9999) { positionX = -9999; }
		if(positionY < -9999) { positionY = -9999; }
		if(positionZ < -9999) { positionZ = -9999; }

		// Setup the X position string.
		_itoa_s(positionX, tempString, 10);
		strcpy_s(dataString, "X: ");
		strcat_s(dataString, tempString);

		result = UpdateSentence(sentence5, dataString, 10, 130, 0.0f, 1.0f, 0.0f, pDeviceContext);
		if(!result)
		{
			return false;
		}
	
		// Setup the Y position string.
		_itoa_s(positionY, tempString, 10);
		strcpy_s(dataString, "Y: ");
		strcat_s(dataString, tempString);

		result = UpdateSentence(sentence6, dataString, 10, 150, 0.0f, 1.0f, 0.0f, pDeviceContext);
		if(!result)
		{
			return false;
		}

		// Setup the Z position string.
		_itoa_s(positionZ, tempString, 10);
		strcpy_s(dataString, "Z: ");
		strcat_s(dataString, tempString);

		result = UpdateSentence(sentence7, dataString, 10, 170, 0.0f, 1.0f, 0.0f, pDeviceContext);
		if(!result)
		{
			return false;
		}

		return true;
	}


	bool Text::SetCameraRotation(float rotX, float rotY, float rotZ, ID3D11DeviceContext* pDeviceContext)
	{
		int rotationX, rotationY, rotationZ;
		char tempString[16];
		char dataString[16];
		bool result;


		// Convert the rotation from floating point to integer.
		rotationX = (int)rotX;
		rotationY = (int)rotY;
		rotationZ = (int)rotZ;

		// Setup the X rotation string.
		_itoa_s(rotationX, tempString, 10);
		strcpy_s(dataString, "rX: ");
		strcat_s(dataString, tempString);

		result = UpdateSentence(sentence8, dataString, 10, 210, 0.0f, 1.0f, 0.0f, pDeviceContext);
		if(!result)
		{
			return false;
		}

		// Setup the Y rotation string.
		_itoa_s(rotationY, tempString, 10);
		strcpy_s(dataString, "rY: ");
		strcat_s(dataString, tempString);

		result = UpdateSentence(sentence9, dataString, 10, 230, 0.0f, 1.0f, 0.0f, pDeviceContext);
		if(!result)
		{
			return false;
		}

		// Setup the Z rotation string.
		_itoa_s(rotationZ, tempString, 10);
		strcpy_s(dataString, "rZ: ");
		strcat_s(dataString, tempString);

		result = UpdateSentence(sentence10, dataString, 10, 250, 0.0f, 1.0f, 0.0f, pDeviceContext);
		if(!result)
		{
			return false;
		}

		return true;
	}
}
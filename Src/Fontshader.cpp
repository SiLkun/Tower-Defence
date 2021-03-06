////////////////////////////////////////////////////////////////////////////////
// Filename: fontshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{

	FontShader::FontShader()
	{
		pVertexShader = 0;
		pPixelShader = 0;
		pLayout = 0;
		pConstantBuffer = 0;
		pSampleState = 0;
		pPixelBuffer = 0;
	}


	FontShader::FontShader(const FontShader& other)
	{
	}


	FontShader::~FontShader()
	{
	}


	bool FontShader::Initialize(ID3D11Device* device, HWND hwnd)
	{
		bool result;


		// Initialize the vertex and pixel shaders.
		result = InitializeShader(device, hwnd, L"Data/Shader/Shader.vs", L"Data/Shader/font.ps");
		if(!result)
		{
			return false;
		}

		return true;
	}


	void FontShader::Release()
	{
		// Release the vertex and pixel shaders as well as the related objects.
		ReleaseShader();

		return;
	}


	bool FontShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, 
								 D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR4 pixelColor)
	{
		bool result;


		// Set the shader parameters that it will use for rendering.
		result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, pixelColor);
		if(!result)
		{
			return false;
		}

		// Now render the prepared buffers with the shader.
		RenderShader(deviceContext, indexCount);

		return true;
	}


	bool FontShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
	{
		HRESULT result;
		ID3D10Blob* errorMessage;
		ID3D10Blob* vertexShaderBuffer;
		ID3D10Blob* pixelShaderBuffer;
		D3D11_INPUT_ELEMENT_DESC polygonLayout[4];
		unsigned int numElements;
		D3D11_BUFFER_DESC constantBufferDesc;
		D3D11_SAMPLER_DESC samplerDesc;
		D3D11_BUFFER_DESC pixelBufferDesc;


		// Initialize the pointers this function will use to null.
		errorMessage = 0;
		vertexShaderBuffer = 0;
		pixelShaderBuffer = 0;

		// Compile the vertex shader code.
		result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "VS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
									   &vertexShaderBuffer, &errorMessage, NULL);
		if(FAILED(result))
		{
			// If the shader failed to compile it should have writen something to the error message.
			if(errorMessage)
			{
				OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
			}
			// If there was  nothing in the error message then it simply could not find the shader file itself.
			else
			{
				MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
			}

			return false;
		}

		// Compile the pixel shader code.
		result = D3DX11CompileFromFile(psFilename, NULL, NULL, "PS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
									   &pixelShaderBuffer, &errorMessage, NULL);
		if(FAILED(result))
		{
			// If the shader failed to compile it should have writen something to the error message.
			if(errorMessage)
			{
				OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
			}
			// If there was  nothing in the error message then it simply could not find the file itself.
			else
			{
				MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
			}

			return false;
		}

		// Create the vertex shader from the buffer.
		result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, 
											&pVertexShader);
		if(FAILED(result))
		{
			return false;
		}

		// Create the vertex shader from the buffer.
		result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, 
										   &pPixelShader);
		if(FAILED(result))
		{
			return false;
		}

		// Create the vertex input layout description.
		polygonLayout[0].SemanticName = "POSITION";
		polygonLayout[0].SemanticIndex = 0;
		polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[0].InputSlot = 0;
		polygonLayout[0].AlignedByteOffset = 0;
		polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[0].InstanceDataStepRate = 0;

		polygonLayout[1].SemanticName = "TEXCOORD";
		polygonLayout[1].SemanticIndex = 0;
		polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		polygonLayout[1].InputSlot = 0;
		polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[1].InstanceDataStepRate = 0;

		polygonLayout[2].SemanticName = "NORMAL";
		polygonLayout[2].SemanticIndex = 0;
		polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[2].InputSlot = 0;
		polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[2].InstanceDataStepRate = 0;

		polygonLayout[3].SemanticName = "COLOR";
		polygonLayout[3].SemanticIndex = 0;
		polygonLayout[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		polygonLayout[3].InputSlot = 0;
		polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[3].InstanceDataStepRate = 0;

		// Get a count of the elements in the layout.
		numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

		// Create the vertex input layout.
		result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), 
										   vertexShaderBuffer->GetBufferSize(), &pLayout);
		if(FAILED(result))
		{
			return false;
		}

		// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
		vertexShaderBuffer->Release();
		vertexShaderBuffer = 0;

		pixelShaderBuffer->Release();
		pixelShaderBuffer = 0;

		// Setup the description of the dynamic constant buffer that is in the vertex shader.
		constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferDesc.ByteWidth = sizeof(ConstantBufferType);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constantBufferDesc.MiscFlags = 0;
		constantBufferDesc.StructureByteStride = 0;

		// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
		result = device->CreateBuffer(&constantBufferDesc, NULL, &pConstantBuffer);
		if(FAILED(result))
		{
			return false;
		}

		// Create a texture sampler state description.
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Create the texture sampler state.
		result = device->CreateSamplerState(&samplerDesc, &pSampleState);
		if(FAILED(result))
		{
			return false;
		}

		// Setup the description of the dynamic pixel constant buffer that is in the pixel shader.
		pixelBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		pixelBufferDesc.ByteWidth = sizeof(PixelBufferType);
		pixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		pixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		pixelBufferDesc.MiscFlags = 0;
		pixelBufferDesc.StructureByteStride = 0;

		// Create the pixel constant buffer pointer so we can access the pixel shader constant buffer from within this class.
		result = device->CreateBuffer(&pixelBufferDesc, NULL, &pPixelBuffer);
		if(FAILED(result))
		{
			return false;
		}

		return true;
	}


	void FontShader::ReleaseShader()
	{
		// Release the pixel constant buffer.
		if(pPixelBuffer)
		{
			pPixelBuffer->Release();
			pPixelBuffer = 0;
		}

		// Release the sampler state.
		if(pSampleState)
		{
			pSampleState->Release();
			pSampleState = 0;
		}

		// Release the constant buffer.
		if(pConstantBuffer)
		{
			pConstantBuffer->Release();
			pConstantBuffer = 0;
		}

		// Release the layout.
		if(pLayout)
		{
			pLayout->Release();
			pLayout = 0;
		}

		// Release the pixel shader.
		if(pPixelShader)
		{
			pPixelShader->Release();
			pPixelShader = 0;
		}

		// Release the vertex shader.
		if(pVertexShader)
		{
			pVertexShader->Release();
			pVertexShader = 0;
		}

		return;
	}


	void FontShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
	{
		char* compileErrors;
		unsigned long bufferSize, i;
		ofstream fout;


		// Get a pointer to the error message text buffer.
		compileErrors = (char*)(errorMessage->GetBufferPointer());

		// Get the length of the message.
		bufferSize = errorMessage->GetBufferSize();

		// Open a file to write the error message to.
		fout.open("shader-error.txt");

		// Write out the error message.
		for(i=0; i<bufferSize; i++)
		{
			fout << compileErrors[i];
		}

		// Close the file.
		fout.close();

		// Release the error message.
		errorMessage->Release();
		errorMessage = 0;

		// Pop a message up on the screen to notify the user to check the text file for compile errors.
		MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

		return;
	}


	bool FontShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, 
											  D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR4 pixelColor)
	{
		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ConstantBufferType* dataPtr;
		unsigned int bufferNumber;
		PixelBufferType* dataPtr2;


		// Lock the constant buffer so it can be written to.
		result = deviceContext->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if(FAILED(result))
		{
			return false;
		}

		// Get a pointer to the data in the constant buffer.
		dataPtr = (ConstantBufferType*)mappedResource.pData;

		// Transpose the matrices to prepare them for the shader.
		D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
		D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
		D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

		// Copy the matrices into the constant buffer.
		dataPtr->world = worldMatrix;
		dataPtr->view = viewMatrix;
		dataPtr->projection = projectionMatrix;

		// Unlock the constant buffer.
		deviceContext->Unmap(pConstantBuffer, 0);

		// Set the position of the constant buffer in the vertex shader.
		bufferNumber = 0;

		// Now set the constant buffer in the vertex shader with the updated values.
		deviceContext->VSSetConstantBuffers(bufferNumber, 1, &pConstantBuffer);

		// Set shader texture resource in the pixel shader.
		deviceContext->PSSetShaderResources(0, 1, &texture);

		// Lock the pixel constant buffer so it can be written to.
		result = deviceContext->Map(pPixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if(FAILED(result))
		{
			return false;
		}

		// Get a pointer to the data in the pixel constant buffer.
		dataPtr2 = (PixelBufferType*)mappedResource.pData;

		// Copy the pixel color into the pixel constant buffer.
		dataPtr2->pixelColor = pixelColor;

		// Unlock the pixel constant buffer.
		deviceContext->Unmap(pPixelBuffer, 0);

		// Set the position of the pixel constant buffer in the pixel shader.
		bufferNumber = 0;

		// Now set the pixel constant buffer in the pixel shader with the updated value.
		deviceContext->PSSetConstantBuffers(bufferNumber, 1, &pPixelBuffer);

		return true;
	}


	void FontShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
	{
		// Set the vertex input layout.
		deviceContext->IASetInputLayout(pLayout);

		// Set the vertex and pixel shaders that will be used to render the triangles.
		deviceContext->VSSetShader(pVertexShader, NULL, 0);
		deviceContext->PSSetShader(pPixelShader, NULL, 0);

		// Set the sampler state in the pixel shader.
		deviceContext->PSSetSamplers(0, 1, &pSampleState);

		// Render the triangles.
		deviceContext->DrawIndexed(indexCount, 0, 0);

		return;
	}
}
////////////////////////////////////////////////////////////////////////////////
// Filename: texture.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{

	Texture::Texture()
	{
		pTexture = 0;
	}


	Texture::Texture(const Texture& other)
	{
	}


	Texture::~Texture()
	{
	}


	bool Texture::Initialize(ID3D11Device* pDevice, WCHAR* filename)
	{
		HRESULT result;


		// Load the texture in.
		result = D3DX11CreateShaderResourceViewFromFile(pDevice, filename, NULL, NULL, &pTexture, NULL);
		if(FAILED(result))
		{
			return false;
		}

		return true;
	}


	void Texture::Shutdown()
	{
		// Release the texture resource.
		if(pTexture)
		{
			pTexture->Release();
			pTexture = 0;
		}

		return;
	}


	ID3D11ShaderResourceView* Texture::GetTexture()
	{
		return pTexture;
	}
}
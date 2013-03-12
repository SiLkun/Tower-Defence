////////////////////////////////////////////////////////////////////////////////
// Filename: texture.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

namespace TD
{

	Texture::Texture()
	{
		pTexture = 0;
		lock = 0;
	}


	Texture::Texture(const Texture& other)
	{
	}


	Texture::~Texture()
	{
	}


	bool Texture::Initialize(ID3D11Device* pDevice, char* fileName)
	{
		this->fileName = fileName;
		lock++;
		if(pTexture == NULL)
		{
			HRESULT result = D3DX11CreateShaderResourceViewFromFileA(pDevice, fileName, NULL, NULL, &pTexture, NULL);
			if(FAILED(result))
			{
				return false;
			}
		}

		return true;
	}

	int Texture::Release()
	{
		--lock;

		// Release the texture resource.
		if(pTexture && lock <= 0)
		{
			pTexture->Release();
			pTexture = 0;
		}
		return lock;
	}


	ID3D11ShaderResourceView* Texture::GetTexture()
	{
		return pTexture;
	}

	string Texture::GetFileName()
	{
		return fileName;
	}



}
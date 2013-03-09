////////////////////////////////////////////////////////////////////////////////
// Filename: Terrainshader.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TERRAINSHADER_H_
#define _TERRAINSHADER_H_

#include "stdafx.h"



////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainShader
////////////////////////////////////////////////////////////////////////////////
class TerrainShader
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct LightBufferType
	{
		D3DXVECTOR4 ambientColor;
		D3DXVECTOR4 diffuseColor;
		D3DXVECTOR3 lightDirection;
		float padding;
	};

	struct TextureInfoBufferType
	{
		bool useAlplha;
		D3DXVECTOR3 padding2;
	};

public:
	TerrainShader();
	TerrainShader(const TerrainShader&);
	~TerrainShader();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3);
	bool SetShaderTextures(ID3D11DeviceContext*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, bool);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

private:
	ID3D11VertexShader* pVertexShader;
	ID3D11PixelShader* pPixelShader;
	ID3D11InputLayout* pLayout;
	ID3D11SamplerState* pSampleState;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* pTextureInfoBuffer;
};

#endif
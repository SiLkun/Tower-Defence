////////////////////////////////////////////////////////////////////////////////
// Filename: d3d.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _D3D_H_
#define _D3D_H_



//////////////
// INCLUDES //
//////////////
#include "stdafx.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: D3D
////////////////////////////////////////////////////////////////////////////////
class D3D
{
public:
	D3D();
	D3D(const D3D&);
	~D3D();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Release();
	
	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(D3DXMATRIX&);
	void GetWorldMatrix(D3DXMATRIX&);
	void GetOrthoMatrix(D3DXMATRIX&);

	void GetVideoCardInfo(char*, int&);

	void TurnZBufferOn();
	void TurnZBufferOff();
	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

private:
	bool vsync_enabled;
	int videoCardMemory;
	char videoCardDescription[128];
	IDXGISwapChain* pSwapChain;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
	ID3D11RenderTargetView* pRenderTargetView;
	ID3D11Texture2D* pDepthStencilBuffer;
	ID3D11DepthStencilState* pDepthStencilState;
	ID3D11DepthStencilView* pDepthStencilView;
	ID3D11RasterizerState* pRasterState;
	D3DXMATRIX projectionMatrix;
	D3DXMATRIX worldMatrix;
	D3DXMATRIX orthoMatrix;
	ID3D11DepthStencilState* pDepthDisabledStencilState;
	ID3D11BlendState* pAlphaEnableBlendingState;
	ID3D11BlendState* pAlphaDisableBlendingState;
};

#endif
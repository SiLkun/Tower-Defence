////////////////////////////////////////////////////////////////////////////////
// Filename: D3D.cpp
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

namespace TD
{

	D3D::D3D()
	{
		pSwapChain = 0;
		pDevice = 0;
		pDeviceContext = 0;
		pRenderTargetView = 0;
		pDepthStencilBuffer = 0;
		pDepthStencilState = 0;
		pDepthStencilView = 0;
		pRasterState = 0;
		pDepthDisabledStencilState = 0;
		pAlphaEnableBlendingState = 0;
		pAlphaDisableBlendingState = 0;
	}


	D3D::D3D(const D3D& other)
	{
	}


	D3D::~D3D()
	{
	}


	bool D3D::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, 
							  float screenDepth, float screenNear)
	{
		HRESULT result;
		IDXGIFactory* factory;
		IDXGIAdapter* adapter;
		IDXGIOutput* adapterOutput;
		unsigned int numModes, i, numerator, denominator, stringLength;
		DXGI_MODE_DESC* displayModeList;
		DXGI_ADAPTER_DESC adapterDesc;
		int error;
		DXGI_SWAP_CHAIN_DESC pSwapChainDesc;
		D3D_FEATURE_LEVEL featureLevel;
		ID3D11Texture2D* backBufferPtr;
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC pDepthStencilViewDesc;
		D3D11_RASTERIZER_DESC rasterDesc;
		D3D11_VIEWPORT viewport;
		float fieldOfView, screenAspect;
		D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
		D3D11_BLEND_DESC blendStateDescription;


		// Store the vsync setting.
		vsync_enabled = vsync;

		// Create a DirectX graphics interface factory.
		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		if(FAILED(result))
		{
			return false;
		}

		// Use the factory to create an adapter for the primary graphics interface (video card).
		result = factory->EnumAdapters(0, &adapter);
		if(FAILED(result))
		{
			return false;
		}

		// Enumerate the primary adapter output (monitor).
		result = adapter->EnumOutputs(0, &adapterOutput);
		if(FAILED(result))
		{
			return false;
		}

		// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		if(FAILED(result))
		{
			return false;
		}

		// Create a list to hold all the possible display modes for this monitor/video card combination.
		displayModeList = new DXGI_MODE_DESC[numModes];
		if(!displayModeList)
		{
			return false;
		}

		// Now fill the display mode list structures.
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
		if(FAILED(result))
		{
			return false;
		}

		// Now go through all the display modes and find the one that matches the screen width and height.
		// When a match is found store the numerator and denominator of the refresh rate for that monitor.
		for(i=0; i<numModes; i++)
		{
			if(displayModeList[i].Width == (unsigned int)screenWidth)
			{
				if(displayModeList[i].Height == (unsigned int)screenHeight)
				{
					numerator = displayModeList[i].RefreshRate.Numerator;
					denominator = displayModeList[i].RefreshRate.Denominator;
				}
			}
		}

		// Get the adapter (video card) description.
		result = adapter->GetDesc(&adapterDesc);
		if(FAILED(result))
		{
			return false;
		}

		// Store the dedicated video card memory in megabytes.
		videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		// Convert the name of the video card to a character array and store it.
		error = wcstombs_s(&stringLength, videoCardDescription, 128, adapterDesc.Description, 128);
		if(error != 0)
		{
			return false;
		}

		// Release the display mode list.
		delete [] displayModeList;
		displayModeList = 0;

		// Release the adapter output.
		adapterOutput->Release();
		adapterOutput = 0;

		// Release the adapter.
		adapter->Release();
		adapter = 0;

		// Release the factory.
		factory->Release();
		factory = 0;

		// Initialize the swap chain description.
		ZeroMemory(&pSwapChainDesc, sizeof(pSwapChainDesc));

		// Set to a single back buffer.
		pSwapChainDesc.BufferCount = 1;

		// Set the width and height of the back buffer.
		pSwapChainDesc.BufferDesc.Width = screenWidth;
		pSwapChainDesc.BufferDesc.Height = screenHeight;

		// Set regular 32-bit surface for the back buffer.
		pSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		// Set the refresh rate of the back buffer.
		if(vsync_enabled)
		{
			pSwapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
			pSwapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
		}
		else
		{
			pSwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
			pSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}

		// Set the usage of the back buffer.
		pSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		// Set the handle for the window to render to.
		pSwapChainDesc.OutputWindow = hwnd;

		// Turn multisampling off.
		pSwapChainDesc.SampleDesc.Count = 1;
		pSwapChainDesc.SampleDesc.Quality = 0;

		// Set to full screen or windowed mode.
		if(fullscreen)
		{
			pSwapChainDesc.Windowed = false;
		}
		else
		{
			pSwapChainDesc.Windowed = true;
		}

		// Set the scan line ordering and scaling to unspecified.
		pSwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		pSwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// Discard the back buffer contents after presenting.
		pSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		// Don't set the advanced flags.
		pSwapChainDesc.Flags = 0;

		// Set the feature level to DirectX 11.
		featureLevel = D3D_FEATURE_LEVEL_11_0;

		// Create the swap chain, Direct3D pDevice, and Direct3D pDevice context.
		result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0 , &featureLevel, 1, 
											   D3D11_SDK_VERSION, &pSwapChainDesc, &pSwapChain, &pDevice, NULL, &pDeviceContext);
		if(FAILED(result))
		{
			return false;
		}

		// Get the pointer to the back buffer.
		result = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
		if(FAILED(result))
		{
			return false;
		}

		// Create the render target view with the back buffer pointer.
		result = pDevice->CreateRenderTargetView(backBufferPtr, NULL, &pRenderTargetView);
		if(FAILED(result))
		{
			return false;
		}

		// Release pointer to the back buffer as we no longer need it.
		backBufferPtr->Release();
		backBufferPtr = 0;

		// Initialize the description of the depth buffer.
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		// Set up the description of the depth buffer.
		depthBufferDesc.Width = screenWidth;
		depthBufferDesc.Height = screenHeight;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		// Create the texture for the depth buffer using the filled out description.
		result = pDevice->CreateTexture2D(&depthBufferDesc, NULL, &pDepthStencilBuffer);
		if(FAILED(result))
		{
			return false;
		}

		// Initialize the description of the stencil state.
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		// Set up the description of the stencil state.
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing.
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing.
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create the depth stencil state.
		result = pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState);
		if(FAILED(result))
		{
			return false;
		}

		// Set the depth stencil state.
		pDeviceContext->OMSetDepthStencilState(pDepthStencilState, 1);

		// Initialize the depth stencil view.
		ZeroMemory(&pDepthStencilViewDesc, sizeof(pDepthStencilViewDesc));

		// Set up the depth stencil view description.
		pDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		pDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		pDepthStencilViewDesc.Texture2D.MipSlice = 0;

		// Create the depth stencil view.
		result = pDevice->CreateDepthStencilView(pDepthStencilBuffer, &pDepthStencilViewDesc, &pDepthStencilView);
		if(FAILED(result))
		{
			return false;
		}

		// Bind the render target view and depth stencil buffer to the output render pipeline.
		pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

		// Setup the raster description which will determine how and what polygons will be drawn.
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		// Create the rasterizer state from the description we just filled out.
		result = pDevice->CreateRasterizerState(&rasterDesc, &pRasterState);
		if(FAILED(result))
		{
			return false;
		}

		// Now set the rasterizer state.
		pDeviceContext->RSSetState(pRasterState);
	
		// Setup the viewport for rendering.
		viewport.Width = (float)screenWidth;
		viewport.Height = (float)screenHeight;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		// Create the viewport.
		pDeviceContext->RSSetViewports(1, &viewport);

		// Setup the projection matrix.
		fieldOfView = (float)D3DX_PI / 4.0f;
		screenAspect = (float)screenWidth / (float)screenHeight;

		// Create the projection matrix for 3D rendering.
		D3DXMatrixPerspectiveFovLH(&projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

		// Initialize the world matrix to the identity matrix.
		D3DXMatrixIdentity(&worldMatrix);

		// Create an orthographic projection matrix for 2D rendering.
		D3DXMatrixOrthoLH(&orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

		// Clear the second depth stencil state before setting the parameters.
		ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

		// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
		// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
		depthDisabledStencilDesc.DepthEnable = false;
		depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthDisabledStencilDesc.StencilEnable = true;
		depthDisabledStencilDesc.StencilReadMask = 0xFF;
		depthDisabledStencilDesc.StencilWriteMask = 0xFF;
		depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create the state using the pDevice.
		result = pDevice->CreateDepthStencilState(&depthDisabledStencilDesc, &pDepthDisabledStencilState);
		if(FAILED(result))
		{
			return false;
		}

		// Clear the blend state description.
		ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

		// Create an alpha enabled blend state description.
		blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
		blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		// Create the blend state using the description.
		result = pDevice->CreateBlendState(&blendStateDescription, &pAlphaEnableBlendingState);
		if(FAILED(result))
		{
			return false;
		}

		// Modify the description to create an alpha disabled blend state description.
		blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

		// Create the second blend state using the description.
		result = pDevice->CreateBlendState(&blendStateDescription, &pAlphaDisableBlendingState);
		if(FAILED(result))
		{
			return false;
		}

		return true;
	}


	void D3D::Shutdown()
	{
		// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
		if(pSwapChain)
		{
			pSwapChain->SetFullscreenState(false, NULL);
		}

		if(pAlphaEnableBlendingState)
		{
			pAlphaEnableBlendingState->Release();
			pAlphaEnableBlendingState = 0;
		}

		if(pAlphaDisableBlendingState)
		{
			pAlphaDisableBlendingState->Release();
			pAlphaDisableBlendingState = 0;
		}

		if(pDepthDisabledStencilState)
		{
			pDepthDisabledStencilState->Release();
			pDepthDisabledStencilState = 0;
		}

		if(pRasterState)
		{
			pRasterState->Release();
			pRasterState = 0;
		}

		if(pDepthStencilView)
		{
			pDepthStencilView->Release();
			pDepthStencilView = 0;
		}

		if(pDepthStencilState)
		{
			pDepthStencilState->Release();
			pDepthStencilState = 0;
		}

		if(pDepthStencilBuffer)
		{
			pDepthStencilBuffer->Release();
			pDepthStencilBuffer = 0;
		}

		if(pRenderTargetView)
		{
			pRenderTargetView->Release();
			pRenderTargetView = 0;
		}

		if(pDeviceContext)
		{
			pDeviceContext->Release();
			pDeviceContext = 0;
		}

		if(pDevice)
		{
			pDevice->Release();
			pDevice = 0;
		}

		if(pSwapChain)
		{
			pSwapChain->Release();
			pSwapChain = 0;
		}

		return;
	}


	void D3D::BeginScene(float red, float green, float blue, float alpha)
	{
		float color[4];


		// Setup the color to clear the buffer to.
		color[0] = red;
		color[1] = green;
		color[2] = blue;
		color[3] = alpha;

		// Clear the back buffer.
		pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);
    
		// Clear the depth buffer.
		pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		return;
	}


	void D3D::EndScene()
	{
		// Present the back buffer to the screen since rendering is complete.
		if(vsync_enabled)
		{
			// Lock to screen refresh rate.
			pSwapChain->Present(1, 0);
		}
		else
		{
			// Present as fast as possible.
			pSwapChain->Present(0, 0);
		}

		return;
	}


	ID3D11Device* D3D::GetDevice()
	{
		return pDevice;
	}


	ID3D11DeviceContext* D3D::GetDeviceContext()
	{
		return pDeviceContext;
	}


	void D3D::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
	{
		projectionMatrix = this->projectionMatrix;
		return;
	}


	void D3D::GetWorldMatrix(D3DXMATRIX& worldMatrix)
	{
		worldMatrix = this->worldMatrix;
		return;
	}


	void D3D::GetOrthoMatrix(D3DXMATRIX& orthoMatrix)
	{
		orthoMatrix = this->orthoMatrix;
		return;
	}


	void D3D::GetVideoCardInfo(char* cardName, int& memory)
	{
		strcpy_s(cardName, 128, videoCardDescription);
		memory = videoCardMemory;
		return;
	}


	void D3D::TurnZBufferOn()
	{
		pDeviceContext->OMSetDepthStencilState(pDepthStencilState, 1);
		return;
	}


	void D3D::TurnZBufferOff()
	{
		pDeviceContext->OMSetDepthStencilState(pDepthDisabledStencilState, 1);
		return;
	}


	void D3D::TurnOnAlphaBlending()
	{
		float blendFactor[4];
	

		// Setup the blend factor.
		blendFactor[0] = 0.0f;
		blendFactor[1] = 0.0f;
		blendFactor[2] = 0.0f;
		blendFactor[3] = 0.0f;
	
		// Turn on the alpha blending.
		pDeviceContext->OMSetBlendState(pAlphaEnableBlendingState, blendFactor, 0xffffffff);

		return;
	}


	void D3D::TurnOffAlphaBlending()
	{
		float blendFactor[4];
	

		// Setup the blend factor.
		blendFactor[0] = 0.0f;
		blendFactor[1] = 0.0f;
		blendFactor[2] = 0.0f;
		blendFactor[3] = 0.0f;
	
		// Turn off the alpha blending.
		pDeviceContext->OMSetBlendState(pAlphaDisableBlendingState, blendFactor, 0xffffffff);

		return;
	}
}
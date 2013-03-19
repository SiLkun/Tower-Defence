////////////////////////////////////////////////////////////////////////////////
// Filename: text.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXT_H_
#define _TEXT_H_


#include "stdafx.h"

class Texture;

////////////////////////////////////////////////////////////////////////////////
// Class name: Gui
////////////////////////////////////////////////////////////////////////////////
class Gui
{
private:
	struct SentenceType
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};


	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
	};

public:
	Gui();
	Gui(const Gui&);
	~Gui();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, D3DXMATRIX);
	void Release();
	bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);
	bool RenderLoadScreen(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);
	bool RenderGameOver(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);

	bool SetFps(int, ID3D11DeviceContext*);
	bool SetCpu(int, ID3D11DeviceContext*);
	bool SetTime(int time, ID3D11DeviceContext* deviceContext);
	bool SetLevel(int level, ID3D11DeviceContext* deviceContext);
	bool SetGold(int gold, ID3D11DeviceContext* deviceContext);
	bool SetNextWaveTime(int time, ID3D11DeviceContext* deviceContext);
	bool SetLivesLeft(int livesLeft, ID3D11DeviceContext* deviceContext);
	
private:
	bool InitializeSentence(SentenceType**, int, ID3D11Device*);
	bool UpdateSentence(SentenceType*, char*, int, int, float, float, float, ID3D11DeviceContext*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*, D3DXMATRIX, D3DXMATRIX);

private:
	Font* pFont;
	FontShader* pFontShader;
	int screenWidth, screenHeight;
	D3DXMATRIX pBaseViewMatrix;
	SentenceType* pFpsSentence;
	SentenceType* pCpuSentence;
	SentenceType* pGoldSentence;
	SentenceType* pLevelSentence;
	SentenceType* pTimeSentence;
	SentenceType* pNextWaveTimeSentence;
	SentenceType* pLivesSentence;


	ID3D11Buffer *pVertexBuffer, *pIndexBuffer;
	int vertexCount, indexCount;
	Texture* pTexture;
	Texture* pTextureLoadScreen;
	Texture* pTextureGameOverScreen;

	
};

#endif
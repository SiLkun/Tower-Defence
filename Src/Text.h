////////////////////////////////////////////////////////////////////////////////
// Filename: text.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXT_H_
#define _TEXT_H_


#include "stdafx.h"



////////////////////////////////////////////////////////////////////////////////
// Class name: Text
////////////////////////////////////////////////////////////////////////////////
class Text
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
	Text();
	Text(const Text&);
	~Text();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, D3DXMATRIX);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);

	bool SetFps(int, ID3D11DeviceContext*);
	bool SetCpu(int, ID3D11DeviceContext*);
	bool SetVideoCardInfo(char*, int, ID3D11DeviceContext*);
	bool SetTime(int time, ID3D11DeviceContext* deviceContext);
	bool SetNextWaveTime(int time, ID3D11DeviceContext* deviceContext);

	
private:
	bool InitializeSentence(SentenceType**, int, ID3D11Device*);
	bool UpdateSentence(SentenceType*, char*, int, int, float, float, float, ID3D11DeviceContext*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*, D3DXMATRIX, D3DXMATRIX);

private:
	Font* pFont;
	FontShader* pFontShader;
	int pScreenWidth, pScreenHeight;
	D3DXMATRIX pBaseViewMatrix;
	SentenceType* pFpsSentence;
	SentenceType* pCpuSentence;
	SentenceType* pVideocardName;
	SentenceType* pVideocardMemory;
	SentenceType* pTimeSentence;
	SentenceType* pNextWaveTimeSentence;

	
};

#endif
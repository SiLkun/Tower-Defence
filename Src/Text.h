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
	bool Render(ID3D11DeviceContext*, FontShader*, D3DXMATRIX, D3DXMATRIX);

	bool SetVideoCardInfo(char*, int, ID3D11DeviceContext*);
	bool SetFps(int, ID3D11DeviceContext*);
	bool SetCpu(int, ID3D11DeviceContext*);
	bool SetCameraPosition(float, float, float, ID3D11DeviceContext*);
	bool SetCameraRotation(float, float, float, ID3D11DeviceContext*);

private:
	bool InitializeSentence(SentenceType**, int, ID3D11Device*);
	bool UpdateSentence(SentenceType*, char*, int, int, float, float, float, ID3D11DeviceContext*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(SentenceType*, ID3D11DeviceContext*, FontShader*, D3DXMATRIX, D3DXMATRIX);

private:
	int screenWidth, screenHeight;
	D3DXMATRIX baseViewMatrix;
	Font* pFont;
	SentenceType *sentence1, *sentence2, *sentence3, *sentence4, *sentence5;
	SentenceType *sentence6, *sentence7, *sentence8, *sentence9, *sentence10;
};

#endif
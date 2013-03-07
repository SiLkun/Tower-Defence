///////////////////////////////////////////////////////////////////////////////
// Filename: fontclass.cpp
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

namespace TD
{

	Font::Font()
	{
		pFont = 0;
		pTexture = 0;
	}


	Font::Font(const Font& other)
	{
	}


	Font::~Font()
	{
	}


	bool Font::Initialize(ID3D11Device* pDevice, char* fontFilename, WCHAR* textureFilename)
	{
		bool result;


		// Load in the text file containing the font data.
		result = LoadFontData(fontFilename);
		if(!result)
		{
			return false;
		}

		// Load the texture that has the font characters on it.
		result = LoadTexture(pDevice, textureFilename);
		if(!result)
		{
			return false;
		}

		return true;
	}


	void Font::Shutdown()
	{
		// Release the font texture.
		ReleaseTexture();

		// Release the font data.
		ReleaseFontData();

		return;
	}


	bool Font::LoadFontData(char* filename)
	{
		ifstream fin;
		int i;
		char temp;


		// Create the font spacing buffer.
		pFont = new FontType[95];
		if(!pFont)
		{
			return false;
		}

		// Read in the font size and spacing between chars.
		fin.open(filename);
		if(fin.fail())
		{
			return false;
		}

		// Read in the 95 used ascii characters for text.
		for(i=0; i<95; i++)
		{
			fin.get(temp);
			while(temp != ' ')
			{
				fin.get(temp);
			}
			fin.get(temp);
			while(temp != ' ')
			{
				fin.get(temp);
			}

			fin >> pFont[i].left;
			fin >> pFont[i].right;
			fin >> pFont[i].size;
		}

		// Close the file.
		fin.close();

		return true;
	}


	void Font::ReleaseFontData()
	{
		// Release the font data array.
		if(pFont)
		{
			delete [] pFont;
			pFont = 0;
		}

		return;
	}


	bool Font::LoadTexture(ID3D11Device* pDevice, WCHAR* filename)
	{
		bool result;


		// Create the texture object.
		pTexture = new Texture;
		if(!pTexture)
		{
			return false;
		}

		// Initialize the texture object.
		result = pTexture->Initialize(pDevice, filename);
		if(!result)
		{
			return false;
		}

		return true;
	}


	void Font::ReleaseTexture()
	{
		// Release the texture object.
		if(pTexture)
		{
			pTexture->Shutdown();
			delete pTexture;
			pTexture = 0;
		}

		return;
	}


	ID3D11ShaderResourceView* Font::GetTexture()
	{
		return pTexture->GetTexture();
	}


	void Font::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
	{
		VertexType* vertexPtr;
		int numLetters, index, i, letter;


		// Coerce the input vertices into a VertexType structure.
		vertexPtr = (VertexType*)vertices;

		// Get the number of letters in the sentence.
		numLetters = (int)strlen(sentence);

		// Initialize the index to the vertex array.
		index = 0;

		// Draw each letter onto a quad.
		for(i=0; i<numLetters; i++)
		{
			letter = ((int)sentence[i]) - 32;

			// If the letter is a space then just move over three pixels.
			if(letter == 0)
			{
				drawX = drawX + 3.0f;
			}
			else
			{
				// First triangle in quad.
				vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
				vertexPtr[index].texture = D3DXVECTOR2(pFont[letter].left, 0.0f);
				index++;

				vertexPtr[index].position = D3DXVECTOR3((drawX + pFont[letter].size), (drawY - 16), 0.0f);  // Bottom right.
				vertexPtr[index].texture = D3DXVECTOR2(pFont[letter].right, 1.0f);
				index++;

				vertexPtr[index].position = D3DXVECTOR3(drawX, (drawY - 16), 0.0f);  // Bottom left.
				vertexPtr[index].texture = D3DXVECTOR2(pFont[letter].left, 1.0f);
				index++;

				// Second triangle in quad.
				vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
				vertexPtr[index].texture = D3DXVECTOR2(pFont[letter].left, 0.0f);
				index++;

				vertexPtr[index].position = D3DXVECTOR3(drawX + pFont[letter].size, drawY, 0.0f);  // Top right.
				vertexPtr[index].texture = D3DXVECTOR2(pFont[letter].right, 0.0f);
				index++;

				vertexPtr[index].position = D3DXVECTOR3((drawX + pFont[letter].size), (drawY - 16), 0.0f);  // Bottom right.
				vertexPtr[index].texture = D3DXVECTOR2(pFont[letter].right, 1.0f);
				index++;

				// Update the x location for drawing by the size of the letter and one pixel.
				drawX = drawX + pFont[letter].size + 1.0f;
			}
		}

		return;
	}
}
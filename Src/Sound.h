///////////////////////////////////////////////////////////////////////////////
// Filename: Sound.h
///////////////////////////////////////////////////////////////////////////////
#ifndef _SOUND_H_
#define _SOUND_H_

#include "stdafx.h"

///////////////////////////////////////////////////////////////////////////////
// Class name: Sound
///////////////////////////////////////////////////////////////////////////////
class Sound
{
private:
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

public:
	Sound();
	Sound(const Sound&);
	~Sound();

	bool Initialize(char*,HWND);
	void Release();

	void Update(D3DXVECTOR3 p);
	bool Play(D3DXVECTOR3 p);

	string GetFilename();
private:
	bool InitializeDirectSound(HWND);
	void ShutdownDirectSound();

	bool LoadWaveFile(char*, IDirectSoundBuffer8**, IDirectSound3DBuffer8** secondary3DBuffer);
	void ShutdownWaveFile(IDirectSoundBuffer8**, IDirectSound3DBuffer8** secondary3DBuffer);


private:
	IDirectSound8* pDirectSound;
	IDirectSoundBuffer* pPrimaryBuffer;
	IDirectSound3DListener8* pListener;
	IDirectSoundBuffer8* pSecondaryBuffer;
	IDirectSound3DBuffer8* pSecondary3DBuffer;
	string filename;
};

#endif
#ifndef _SOUNDEFFECT_H_
#define _SOUNDEFFECT_H_

#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include "GlobalVariables.h"

class SoundEffect
{
public:
	static SoundEffect* Load(char* file);

	void Play(bool loop);
	void Stop();
	void Volume(int volume);
	void Dispose();

private:
	SoundEffect() { };

	int channel;
	Mix_Chunk* soundChunk;
};

#endif
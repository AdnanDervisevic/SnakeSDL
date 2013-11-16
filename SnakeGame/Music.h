#ifndef _MUSIC_H_
#define _MUSIC_H_

#include "SDL/SDL_mixer.h"

class Music
{
public:
	static bool Load(char* file);

	static void Play(bool loop);
	static void Pause();
	static void Stop();
	static void Dispose();

	static void Volume(int volume);

private:
	Music() { };
	static Mix_Music* music;
};

#endif
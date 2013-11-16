#include "Music.h"

// Initialize the static music variable.
Mix_Music* Music::music = NULL;

// Load the given music file, if a file is already loaded delete from memory and load the new one.
bool Music::Load(char* file)
{
	// If we've already loaded a file, free it from memory.
	if (music != NULL)
	{
		Stop();
		Mix_FreeMusic(music);
		music = NULL;
	}

	// Load the file and return false if it couldn't be loaded, otherwise true.
	if ((music = Mix_LoadMUS(file)) == NULL)
		return false;

	return true;
}

// Play the music.
void Music::Play(bool loop)
{
	// If we're not already playing then start play.
	if (Mix_PlayingMusic() == 0)
	{
		// Check if we shoud loop.
		if (loop)
			Mix_PlayMusic(music, -1);
		else
			Mix_PlayMusic(music, 0);
	}
}

// Pause the music.
void Music::Pause()
{
	// If we've already paused then resume, otherwise pause.
	if (Mix_PlayingMusic() == 1)
		Mix_ResumeMusic();
	else
		Mix_PauseMusic();
}

// Stop the music from playing.
void Music::Stop()
{
	Mix_HaltMusic();
}

// Dispose the music object.
void Music::Dispose()
{
	Stop();

	Mix_FreeMusic(music);
	music = NULL;
}

// Adjust the volume of the music from 0-100%.
void Music::Volume(int volume)
{
	Mix_VolumeMusic(volume);
}
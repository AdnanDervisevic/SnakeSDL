#include "SoundEffect.h"

// Load the sound effect file.
SoundEffect* SoundEffect::Load(char* file)
{
	// Create the sound effect.
	SoundEffect* sound = new SoundEffect();

	// Load the sound effect, if it couldn't be loaded return NULL, otherwise return the sound;
	if ((sound->soundChunk = Mix_LoadWAV(file)) == NULL)
		return NULL;

	sound->channel = -1;

	return sound;
}

// Play the sound effect.
void SoundEffect::Play(bool loop)
{
	SoundEffect::Stop();

	// Check if we're already playing this sound effect.
	if (channel < 0)
	{
		// Check if we shoud loop.
		if (loop)
			channel = Mix_PlayChannel(-1, soundChunk, -1);
		else
			channel = Mix_PlayChannel(-1, soundChunk, 0);
	}
}

// Stop the sound effect.
void SoundEffect::Stop()
{
	// If we're playing then stop the sound effect.
	if (channel >= 0)
	{
		Mix_HaltChannel(channel);
		channel = -1;
	}
}

// Dispose the sound effect.
void SoundEffect::Dispose()
{
	Mix_FreeChunk(soundChunk);
	soundChunk = NULL;
}

// Adjust the volume of the sound effect (0-100%).
void SoundEffect::Volume(int volume)
{
	Mix_VolumeChunk(soundChunk, volume);
}
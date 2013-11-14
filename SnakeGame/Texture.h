#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

class Texture
{
public:
	// Load a file to a surface.
	static SDL_Surface* Load(char* file)
	{
		SDL_Surface* temp_Surface = NULL;
		SDL_Surface* return_Surface = NULL;

		if ((temp_Surface = IMG_Load(file)) == NULL)
			return NULL;

		return_Surface = SDL_DisplayFormatAlpha(temp_Surface);
		SDL_FreeSurface(temp_Surface);

		return return_Surface;
	}
};

#endif
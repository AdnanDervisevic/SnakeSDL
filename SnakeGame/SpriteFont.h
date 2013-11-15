#ifndef _SPRITEFONT_H_
#define _SPRITEFONT_H_

#include "SDL/SDL_ttf.h"

class SpriteFont
{
public:
	// Load a file to a surface.
	static TTF_Font* Load(char* file, int size)
	{
		TTF_Font* font = NULL;

		if ((font = TTF_OpenFont(file, size)) == NULL)
			return NULL;

		return font;
	}
};

#endif
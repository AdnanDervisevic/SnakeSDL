#ifndef _COLOR_H_
#define _COLOR_H_

#include "SDL/SDL.h"

struct Color : SDL_Color
{
public:
	Color(Uint8 r, Uint8 g, Uint8 b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}
};

#endif
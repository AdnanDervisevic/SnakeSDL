#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_

#include "SDL/SDL.h"

struct Rectangle : SDL_Rect
{
public:
	Rectangle(int x, int y, int width, int height)
	{
		this->x = x;
		this->y = y;
		this->w = width;
		this->h = height;
	}

	bool Intersects(const Rectangle& rectagle)
	{
		 return ((((rectagle.x < (this->x + this->w)) && (this->x < (rectagle.x + rectagle.w))) && (rectagle.y < (this->y + this->h))) && (this->y < (rectagle.y + rectagle.h)));
	}
};

#endif
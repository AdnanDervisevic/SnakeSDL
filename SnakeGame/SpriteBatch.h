#ifndef _SPRITEBATCH_H_
#define _SPRITEBATCH_H_

#include "SDL/SDL.h"
#include "Rectangle.h"
#include "Vector2.h"

class SpriteBatch
{
public:
	SpriteBatch(SDL_Surface* backbuffer);
	~SpriteBatch();
	
	bool Draw(SDL_Surface* texture, Vector2& position);
	bool Draw(SDL_Surface* texture, Vector2& position, Rectangle sourceRect);

private:
	SDL_Surface* backbuffer;
};

#endif
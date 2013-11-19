#ifndef _SPRITEBATCH_H_
#define _SPRITEBATCH_H_

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "Rectangle.h"
#include "Vector2.h"
#include "Color.h"
#include <memory>

class SpriteBatch
{
public:
	SpriteBatch(SDL_Surface* backbuffer);
	
	bool Draw(SDL_Surface* texture, Vector2 position);
	bool Draw(SDL_Surface* texture, Vector2 position, Rectangle sourceRect);

	bool DrawString(const char* text, Vector2 position, TTF_Font* font, Color color);

private:
	SDL_Surface* backbuffer;
};

#endif
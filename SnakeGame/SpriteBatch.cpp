#include "SpriteBatch.h"

// Creates a new Spritebatch with the given backbuffer.
SpriteBatch::SpriteBatch(SDL_Surface* backbuffer)
{
	this->backbuffer = backbuffer;
}

// Destroy the spritebatch.
SpriteBatch::~SpriteBatch()
{
	// Free the backbuffer surface.
	SDL_FreeSurface(this->backbuffer);
}

// Draw something onto the backbuffer.
bool SpriteBatch::Draw(SDL_Surface* texture, Vector2& position)
{
	// If the backbuffer or texture is not set, return null.
	if (this->backbuffer == NULL || texture == NULL)
		return false;

	// Convert the vector2 position to a SDL_Rect.
	SDL_Rect destRect;
	destRect.x = static_cast<int>(position.X);
	destRect.y = static_cast<int>(position.Y);

	// Paint the texture on to the backbuffer.
	SDL_BlitSurface(texture, NULL, this->backbuffer, &destRect);
	return true;
}

// Draw a part of something onto the backbuffer
bool SpriteBatch::Draw(SDL_Surface* texture, Vector2& position, Rectangle sourceRect)
{
	// If the backbuffer or texture is not set, return null.
	if (this->backbuffer == NULL || texture == NULL)
		return false;

	// Convert the vector2 position to a SDL_Rect.
	SDL_Rect destRect;
	destRect.x = static_cast<int>(position.X);
	destRect.y = static_cast<int>(position.Y);

	// Paint the texture on to the backbuffer.
	SDL_BlitSurface(texture, &sourceRect, this->backbuffer, &destRect);
	return true;
}
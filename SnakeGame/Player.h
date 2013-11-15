#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "SDL/SDL.h"
#include "Vector2.h"
#include "IDrawableGameComponent.h"

class Player : public IDrawableGameComponent
{
public:
	Player();

	virtual bool Initialize(Vector2 position, SDL_Surface* texture);
	virtual void HandleInput();
	virtual void Update(float elapsedGameTime);
	virtual void Draw(float elapsedGameTime, SpriteBatch* spriteBatch);
	virtual void Cleanup();

private:
	SDL_Surface* texture;
	Vector2 position;
};

#endif
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "SDL/SDL.h"
#include "Vector2.h"
#include "IDrawableGameComponent.h"
#include <vector>
#include "BodyPart.h"

class Player : public IDrawableGameComponent
{
public:
	Player();

	virtual bool Initialize(Vector2 position, SDL_Surface* headTexture, int headSize, SDL_Surface* bodyTexture, int bodySize, SDL_Surface* tailTexture, int tailSize);
	virtual void HandleInput();
	virtual void Update(float elapsedGameTime);
	virtual void Draw(float elapsedGameTime, SpriteBatch* spriteBatch);
	virtual void Cleanup();

	void Turn(Direction direction);

private:
	std::vector<BodyPart> bodyParts;
};

#endif
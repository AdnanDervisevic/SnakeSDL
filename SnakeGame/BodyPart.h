#ifndef _BODYPART_H_
#define _BODYPART_H_

#include "SDL/SDL.h"
#include "Vector2.h"
#include "GlobalVariables.h"
#include "SpriteBatch.h"

class BodyPart
{
public:
	BodyPart();

	bool Initialize(Vector2 position, Vector2 motion, SDL_Surface* texture);
	void Update(float elapsedGameTime, BodyPart& leftNeighbor, BodyPart& rightNeighbor);
	void Draw(float elapsedGameTime, SpriteBatch* spriteBatch);
	void Cleanup();

	SDL_Surface* texture;

	Vector2 position;
	Vector2 targetPosition;

	Vector2 motion;
	Vector2 targetMotion;
};

#endif
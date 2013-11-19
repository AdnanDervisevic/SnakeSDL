#ifndef _BODYPART_H_
#define _BODYPART_H_

#include "SDL/SDL.h"
#include "Vector2.h"
#include "GlobalVariables.h"
#include "SpriteBatch.h"
#include <memory>

class BodyPart
{
public:
	BodyPart();

	bool Initialize(Vector2 position, Vector2 motion, SDL_Surface*);
	void Update(float elapsedGameTime, BodyPart& leftNeighbor, BodyPart& rightNeighbor);
	void Draw(float elapsedGameTime, SpriteBatch* spriteBatch);

	SDL_Surface* Texture;

	Vector2 Position;
	Vector2 TargetPosition;

	Vector2 Motion;
	Vector2 TargetMotion;

	Rectangle BoundingBox;
};

#endif
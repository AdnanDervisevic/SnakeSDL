#ifndef _IDRAWABLEGAMECOMPONENT_H_
#define _IDRAWABLEGAMECOMPONENT_H_

#include "SpriteBatch.h"

class IDrawableGameComponent
{
public:
	virtual void HandleInput() = 0;
	virtual void Update(float elapsedGameTime) = 0;
	virtual void Draw(float elapsedGameTime, SpriteBatch* spriteBatch) = 0;
	virtual void Cleanup() = 0;
};

#endif
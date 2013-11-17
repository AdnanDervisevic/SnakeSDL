#include "BodyPart.h"
#include <math.h>

// Construct a new player at the position of 0, 0
BodyPart::BodyPart()
	: position(0, 0), motion(0, 0), targetPosition(-2, -2), targetMotion(-2, -2)
{ 
}

// Initialize the player.
bool BodyPart::Initialize(Vector2 position, Vector2 motion, Vector2* targetPosition, Vector2* targetMotion, SDL_Surface* texture)
{
	// If the texture is null return false.
	if (texture == NULL)
		return false;

	// Set the position and texture.
	this->position = position;
	this->motion = motion;
	this->texture = texture;

	return true;
}

// Updates the player.
void BodyPart::Update(float elapsedGameTime, BodyPart& neighbor)
{
	if (targetMotion.X != -2 && targetMotion.Y != -2)
	{
		if (targetMotion.Y == 1 || targetMotion.Y == -1)
		{
			if ( (motion.X == 1 && position.X >= targetPosition.X) || (motion.X == -1 && position.X <= targetPosition.X) )
			{
				this->motion = targetMotion;

				if (position.X != neighbor.position.X || position.Y != neighbor.position.X)
				{
					neighbor.targetPosition = this->position;
					neighbor.targetMotion = this->motion;
				}

				this->targetMotion.X = -2;
				this->targetMotion.Y = -2;
				this->targetPosition.X = -2;
				this->targetPosition.Y = -2;
			}
		}
		else if (targetMotion.X == 1 || targetMotion.X == -1)
		{
			if ( (motion.Y == 1 && position.Y >= targetPosition.Y) || (motion.Y == -1 && position.Y <= targetPosition.Y) )
			{
				this->motion = targetMotion;
				
				if (position.X != neighbor.position.X || position.Y != neighbor.position.X)
				{
					neighbor.targetPosition = this->position;
					neighbor.targetMotion = this->motion;
				}

				this->targetMotion.X = -2;
				this->targetMotion.Y = -2;
				this->targetPosition.X = -2;
				this->targetPosition.Y = -2;
			}
		}
	}

	if (motion.X >= -1 && motion.Y >= -1)
	{
		position.X += (this->motion.X * 120 * elapsedGameTime);
		position.Y += (this->motion.Y * 120 * elapsedGameTime);
	}

	// Prevent the player from going off-screen.
	if (position.X > 1280 - 50)
		position.X = -50;
}

// Draws the player.
void BodyPart::Draw(float elapsedGameTime, SpriteBatch* spriteBatch)
{
	// Draws the players sprite.
	spriteBatch->Draw(this->texture, this->position, Rectangle(0, 0, 20, 20));
}

// Cleans up all the variables.
void BodyPart::Cleanup()
{
	// Free up the texture surface.
	SDL_FreeSurface(this->texture);
}
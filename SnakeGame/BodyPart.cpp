#include "BodyPart.h"

// Construct a new player at the position of 0, 0
BodyPart::BodyPart()
	: position(0, 0), motion(0, 0), targetPosition(-2, -2), targetMotion(-2, -2)
{ 
}

// Initialize the player.
bool BodyPart::Initialize(Vector2 position, Vector2 motion, SDL_Surface* texture)
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
void BodyPart::Update(float elapsedGameTime, BodyPart& leftNeighbor, BodyPart& rightNeighbor)
{
	if (targetMotion.X != -2 && targetMotion.Y != -2)
	{
		if (targetMotion.Y == 1 || targetMotion.Y == -1)
		{
			if ( (motion.X == 1 && position.X >= targetPosition.X) || (motion.X == -1 && position.X <= targetPosition.X) )
			{
				this->motion = targetMotion;

				if (position.X != rightNeighbor.position.X || position.Y != rightNeighbor.position.X)
				{
					rightNeighbor.targetPosition = this->position;
					rightNeighbor.targetMotion = this->motion;
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
				
				if (position.X != rightNeighbor.position.X || position.Y != rightNeighbor.position.X)
				{
					rightNeighbor.targetPosition = this->position;
					rightNeighbor.targetMotion = this->motion;
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

	if (motion.X == leftNeighbor.motion.X && motion.Y == leftNeighbor.motion.Y)
	{
		if (position.X < leftNeighbor.position.X)
			position.X = leftNeighbor.position.X - 20;
		else if (position.X > leftNeighbor.position.X)
			position.X = leftNeighbor.position.X + 20;
		else if (position.Y < leftNeighbor.position.Y)
			position.Y = leftNeighbor.position.Y - 20;
		else if (position.Y > leftNeighbor.position.Y)
			position.Y = leftNeighbor.position.Y + 20;
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
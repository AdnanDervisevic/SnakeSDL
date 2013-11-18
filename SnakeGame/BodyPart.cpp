#include "BodyPart.h"

// Construct a new player at the position of 0, 0
BodyPart::BodyPart()
	: Position(0, 0), Motion(0, 0), TargetPosition(-2, -2), TargetMotion(-2, -2), BoundingBox(0, 0, 10, 10)
{ 
}

// Initialize the player.
bool BodyPart::Initialize(Vector2 position, Vector2 motion, SDL_Surface* texture)
{
	// If the texture is null return false.
	if (texture == NULL)
		return false;

	// Set the position and texture.
	this->Position = position;
	this->Motion = motion;
	this->Texture = texture;

	this->BoundingBox.x = static_cast<int>(position.X) + 5;
	this->BoundingBox.y = static_cast<int>(position.Y) + 5;

	return true;
}

// Updates the player.
void BodyPart::Update(float elapsedGameTime, BodyPart& leftNeighbor, BodyPart& rightNeighbor)
{
	if (TargetMotion.X != -2 && TargetMotion.Y != -2)
	{
		if (TargetMotion.Y == 1 || TargetMotion.Y == -1)
		{
			if ( (Motion.X == 1 && Position.X >= TargetPosition.X) || (Motion.X == -1 && Position.X <= TargetPosition.X) )
			{
				this->Position.X = TargetPosition.X;
				this->Motion = TargetMotion;

				if (Position.X != rightNeighbor.Position.X || Position.Y != rightNeighbor.Position.X)
				{
					rightNeighbor.TargetPosition = this->Position;
					rightNeighbor.TargetMotion = this->Motion;
				}

				this->TargetMotion.X = -2;
				this->TargetMotion.Y = -2;
				this->TargetPosition.X = -2;
				this->TargetPosition.Y = -2;
			}
		}
		else if (TargetMotion.X == 1 || TargetMotion.X == -1)
		{
			if ( (Motion.Y == 1 && Position.Y >= TargetPosition.Y) || (Motion.Y == -1 && Position.Y <= TargetPosition.Y) )
			{
				this->Position.Y = TargetPosition.Y;

				this->Motion = TargetMotion;
				
				if (Position.X != rightNeighbor.Position.X || Position.Y != rightNeighbor.Position.X)
				{
					rightNeighbor.TargetPosition = this->Position;
					rightNeighbor.TargetMotion = this->Motion;
				}

				this->TargetMotion.X = -2;
				this->TargetMotion.Y = -2;
				this->TargetPosition.X = -2;
				this->TargetPosition.Y = -2;
			}
		}
	}

	if (Motion.X >= -1 && Motion.Y >= -1)
	{
		Position.X += (this->Motion.X * 120 * elapsedGameTime);
		Position.Y += (this->Motion.Y * 120 * elapsedGameTime);
	}

	if (Motion.X == leftNeighbor.Motion.X && Motion.Y == leftNeighbor.Motion.Y)
	{
		if (Motion.X == 1 || Motion.X == -1)
		{
			if (Position.X < leftNeighbor.Position.X)
				Position.X = leftNeighbor.Position.X - 20;
			else if (Position.X > leftNeighbor.Position.X)
				Position.X = leftNeighbor.Position.X + 20;
			else if (Position.Y < leftNeighbor.Position.Y)
				Position.Y = leftNeighbor.Position.Y - 20;
			else if (Position.Y > leftNeighbor.Position.Y)
				Position.Y = leftNeighbor.Position.Y + 20;
		}
		else
		{
			if (Position.Y < leftNeighbor.Position.Y)
				Position.Y = leftNeighbor.Position.Y - 20;
			else if (Position.Y > leftNeighbor.Position.Y)
				Position.Y = leftNeighbor.Position.Y + 20;
			else if (Position.X < leftNeighbor.Position.X)
				Position.X = leftNeighbor.Position.X - 20;
			else if (Position.X > leftNeighbor.Position.X)
				Position.X = leftNeighbor.Position.X + 20;
		}
	}

	this->BoundingBox.x = static_cast<int>(Position.X) + 5;
	this->BoundingBox.y = static_cast<int>(Position.Y) + 5;
}

// Draws the player.
void BodyPart::Draw(float elapsedGameTime, SpriteBatch* spriteBatch)
{
	// Draws the players sprite.
	spriteBatch->Draw(this->Texture, this->Position, Rectangle(0, 0, 20, 20));
}

// Cleans up all the variables.
void BodyPart::Cleanup()
{
	// Free up the texture surface.
	SDL_FreeSurface(this->Texture);
}
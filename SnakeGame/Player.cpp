#include "Player.h"

// Construct a new player at the position of 0, 0
Player::Player() { }

// Initialize the player.
bool Player::Initialize(Vector2 position, SDL_Surface* headTexture, int headSize, SDL_Surface* bodyTexture, int bodySize, SDL_Surface* tailTexture, int tailSize)
{
	// If the texture is null return false.
	if (headTexture == NULL || bodyTexture == NULL || tailTexture == NULL)
		return false;

	// Set the position and texture.

	BodyPart head;
	if ((head.Initialize(position, Vector2(1, 0), NULL, NULL, headTexture)) == false)
		return false;

	BodyPart body;
	Vector2 bodyPosition(position.X - headSize, position.Y);
	if ((body.Initialize(bodyPosition, Vector2(1, 0), NULL, NULL, bodyTexture)) == false)
		return false;

	BodyPart tail;
	Vector2 tailPosition(bodyPosition.X - headSize, bodyPosition.Y);
	if ((tail.Initialize(tailPosition, Vector2(1, 0), NULL, NULL, tailTexture)) == false)
		return false;

	this->bodyParts.push_back(head);
	this->bodyParts.push_back(tail);

	std::vector<BodyPart>::iterator it = bodyParts.begin() + 1;

	this->bodyParts.insert(it, body);

	return true;
}

// Handles the input for the player.
void Player::HandleInput()
{
}

// Updates the player.
void Player::Update(float elapsedGameTime)
{
	// Updates all the body parts.
	for (int i = 0; i < bodyParts.size(); i++)
	{
		if (i + 1 >= bodyParts.size())
			bodyParts.at(i).Update(elapsedGameTime, bodyParts.at(i));
		else
			bodyParts.at(i).Update(elapsedGameTime, bodyParts.at(i + 1));
	}
}

// Draws the player.
void Player::Draw(float elapsedGameTime, SpriteBatch* spriteBatch)
{
	// Draws all the body parts.
	for(std::vector<BodyPart>::iterator bodyPart = bodyParts.begin(); bodyPart != bodyParts.end(); bodyPart++) 
		bodyPart->Draw(elapsedGameTime, spriteBatch);
}

// Cleans up all the variables.
void Player::Cleanup()
{
	// Free up the vector
	bodyParts.clear();
}

void Player::Turn(Direction direction)
{
	BodyPart& head = bodyParts.at(0);
	BodyPart& body = bodyParts.at(1);

	switch (direction)
	{
	case UP:
		if (head.motion.Y != 1 && head.motion.X == body.motion.X && head.motion.Y == body.motion.Y)
		{
			head.targetPosition = head.position;
			head.targetMotion = Vector2(0, -1);
		}
		break;
	case DOWN:
		if (head.motion.Y != -1 && head.motion.X == body.motion.X && head.motion.Y == body.motion.Y)
		{
			head.targetPosition = head.position;
			head.targetMotion = Vector2(0, 1);
		}
		break;
	case RIGHT:
		if (head.motion.X != -1 && head.motion.X == body.motion.X && head.motion.Y == body.motion.Y)
		{
			head.targetPosition = head.position;
			head.targetMotion = Vector2(1, 0);
		}
		break;
	case LEFT:
		if (head.motion.X != 1 && head.motion.X == body.motion.X && head.motion.Y == body.motion.Y)
		{
			head.targetPosition = head.position;
			head.targetMotion = Vector2(-1, 0);
		}
		break;
	}


	/*
	BodyPart& head = bodyParts.at(0);

	for (int i = 1; i < bodyParts.size(); i++)
	{
		BodyPart& firstPart = bodyParts.at(i - 1);
		BodyPart& secondPart = bodyParts.at(i);

		switch (direction)
		{
		case UP:
			if (firstPart.motion.X == secondPart.motion.X && 
				firstPart.motion.Y == secondPart.motion.Y && 
				head.motion.Y != 1)
			{
				head.motion = Vector2(0, -1);
				secondPart.targetPosition = firstPart.position;
				secondPart.targetMotion = firstPart.motion;
			}
			break;
		case DOWN:
			if (firstPart.motion.X == secondPart.motion.X && 
				firstPart.motion.Y == secondPart.motion.Y && 
				head.motion.Y != -1)
			{
				head.motion = Vector2(0, 1);
				secondPart.targetPosition = firstPart.position;
				secondPart.targetMotion = firstPart.motion;
			}
			break;
		case RIGHT:
			if (firstPart.motion.X == secondPart.motion.X && 
				firstPart.motion.Y == secondPart.motion.Y && 
				head.motion.X != -1)
			{
				head.motion = Vector2(1, 0);
				secondPart.targetPosition = firstPart.position;
				secondPart.targetMotion = firstPart.motion;
			}
			break;
		case LEFT:
			if (firstPart.motion.X == secondPart.motion.X && 
				firstPart.motion.Y == secondPart.motion.Y && 
				head.motion.X != 1)
			{
				head.motion = Vector2(-1, 0);
				secondPart.targetPosition = firstPart.position;
				secondPart.targetMotion = firstPart.motion;
			}
			break;
		}
	}
	*/
}
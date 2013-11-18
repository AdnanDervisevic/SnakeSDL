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
	this->bodyTexture = bodyTexture;

	BodyPart head;
	if ((head.Initialize(position, Vector2(1, 0), headTexture)) == false)
		return false;

	BodyPart tail;
	Vector2 tailPosition(position.X - bodySize, position.Y);
	if ((tail.Initialize(tailPosition, Vector2(1, 0), tailTexture)) == false)
		return false;

	this->bodyParts.push_back(head);
	this->bodyParts.push_back(tail);

	this->AddBodyPart();
	this->AddBodyPart();
	this->AddBodyPart();
	this->AddBodyPart();
	this->AddBodyPart();
	this->AddBodyPart();
	this->AddBodyPart();
	this->AddBodyPart();
	this->AddBodyPart();
	this->AddBodyPart();
	this->AddBodyPart();
	this->AddBodyPart();
	this->AddBodyPart();
	this->AddBodyPart();

	this->alive = true;

	return true;
}

// Handles the input for the player.
void Player::HandleInput()
{
}

int Player::Roll(int min, int max)
{
   return min + static_cast<int>( rand() / static_cast<double>(RAND_MAX) * (max - min) );
}

// Check if the players head collides with another player.
bool Player::Collides(const Player& player)
{
	BodyPart& head = this->bodyParts.at(0);

	for (int i = 0; i < player.bodyParts.size(); i++)
		if (head.BoundingBox.Intersects(player.bodyParts.at(i).BoundingBox))
			return true;

	return false;
}

// Updates the player.
void Player::Update(float elapsedGameTime)
{
	if (!this->alive)
		return;

	// Updates all the body parts.
	for (int i = 0; i < bodyParts.size(); i++)
	{
		if (i - 1 >= 0 && i + 1 < bodyParts.size())
		{
			bodyParts.at(i).Update(elapsedGameTime, bodyParts.at(i - 1), bodyParts.at(i + 1));
		}
		else if (i + 1 < bodyParts.size())
		{
			bodyParts.at(i).Update(elapsedGameTime, bodyParts.at(i), bodyParts.at(i + 1));
		}
		else if (i - 1 >= 0)
		{
			bodyParts.at(i).Update(elapsedGameTime, bodyParts.at(i - 1), bodyParts.at(i));
		}
		else
			bodyParts.at(i).Update(elapsedGameTime, bodyParts.at(i), bodyParts.at(i));
	}

	// Check if you're eating yourself = you die.
	BodyPart& head = this->bodyParts.at(0);

	for (std::vector<BodyPart>::iterator bodyPart = bodyParts.begin() + 1; bodyPart != bodyParts.end(); bodyPart++)
		if (head.BoundingBox.Intersects(bodyPart->BoundingBox))
			this->alive = false;
}

// Draws the player.
void Player::Draw(float elapsedGameTime, SpriteBatch* spriteBatch)
{
	if (!this->alive)
		return;

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

// Adds an extra body part.
void Player::AddBodyPart()
{
	BodyPart& lastAddedBody = this->bodyParts.at(this->bodyParts.size() - 2);
	BodyPart& tail = this->bodyParts.at(this->bodyParts.size() - 1);
		
	BodyPart body;
	Vector2 bodyPosition = lastAddedBody.Position;

	if (lastAddedBody.Motion.X == 1 && lastAddedBody.Motion.Y == 0)
	{
		bodyPosition.X -= 20;
		tail.Position.X -= 20;
	}
	else if (lastAddedBody.Motion.X == -1 && lastAddedBody.Motion.Y == 0)
	{
		bodyPosition.X += 20;
		tail.Position.X += 20;
	}
	else if (lastAddedBody.Motion.X == 0 && lastAddedBody.Motion.Y == 1)
	{
		bodyPosition.Y -= 20;
		tail.Position.Y -= 20;
	}
	else if (lastAddedBody.Motion.X == 0 && lastAddedBody.Motion.Y == -1)
	{
		bodyPosition.Y += 20;
		tail.Position.Y += 20;
	}

	if ((body.Initialize(bodyPosition, lastAddedBody.Motion, bodyTexture)) == false)
		return;

	std::vector<BodyPart>::iterator it = bodyParts.end() - 1;

	this->bodyParts.insert(it, body);
}

void Player::Turn(int direction)
{
	if (!this->alive)
		return;

	BodyPart& head = bodyParts.at(0);
	BodyPart& body = bodyParts.at(1);

	switch (direction)
	{
	case DIRECTION_UP:
		if (head.Motion.Y != 1 && head.Motion.X == body.Motion.X && head.Motion.Y == body.Motion.Y)
		{
			head.TargetPosition = head.Position;
			head.TargetMotion = Vector2(0, -1);
		}
		break;
	case DIRECTION_DOWN:
		if (head.Motion.Y != -1 && head.Motion.X == body.Motion.X && head.Motion.Y == body.Motion.Y)
		{
			head.TargetPosition = head.Position;
			head.TargetMotion = Vector2(0, 1);
		}
		break;
	case DIRECTION_RIGHT:
		if (head.Motion.X != -1 && head.Motion.X == body.Motion.X && head.Motion.Y == body.Motion.Y)
		{
			head.TargetPosition = head.Position;
			head.TargetMotion = Vector2(1, 0);
		}
		break;
	case DIRECTION_LEFT:
		if (head.Motion.X != 1 && head.Motion.X == body.Motion.X && head.Motion.Y == body.Motion.Y)
		{
			head.TargetPosition = head.Position;
			head.TargetMotion = Vector2(-1, 0);
		}
		break;
	}
}
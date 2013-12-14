#include "Player.h"
#include "SnakeGame.h"

// Construct a new player at the position of 0, 0
Player::Player() : Score(0) { }

// Initialize the player.
bool Player::Initialize(Vector2 position, SDL_Surface* headTexture, SDL_Surface* bodyTexture, SDL_Surface* tailTexture)
{
	// If the texture is null return false.
	if (headTexture == NULL || bodyTexture == NULL || tailTexture == NULL)
		return false;

	// Set the position and texture.
	this->bodyTexture = bodyTexture;
	this->headTexture = headTexture;
	this->tailTexture = tailTexture;

	BodyPart head;
	if ((head.Initialize(position, Vector2(1, 0), headTexture)) == false)
		return false;

	BodyPart tail;
	Vector2 tailPosition(position.X - BODYSIZE, position.Y);
	if ((tail.Initialize(tailPosition, Vector2(1, 0), tailTexture)) == false)
		return false;

	this->bodyParts.push_back(head);
	this->bodyParts.push_back(tail);

	this->alive = true;

	return true;
}

// Handles the input for the player.
void Player::HandleInput()
{
}

// Check if the players body collides with a rectangle.
bool Player::Collides(const Rectangle& rect)
{
	for (std::vector<BodyPart>::iterator bodyPart = bodyParts.begin(); bodyPart != bodyParts.end(); bodyPart++)
		if (bodyPart->BoundingBox.Intersects(rect))
			return true;
	return false;
}

// Check if the players head collides with another player.
bool Player::HeadCollides(const Player& player)
{
	BodyPart& head = this->bodyParts.at(0);

	for (int i = 0; i < player.bodyParts.size(); i++)
		if (head.BoundingBox.Intersects(player.bodyParts.at(i).BoundingBox))
			return true;

	return false;
}

// Check if the players head collides with a rectangle.
bool Player::HeadCollides(const Rectangle& rect)
{
	BodyPart& head = this->bodyParts.at(0);
	return head.BoundingBox.Intersects(rect);
}

// Check if the players body collides with a rectangle.
bool Player::BodyCollides(const Rectangle& rect)
{
	for (std::vector<BodyPart>::iterator bodyPart = bodyParts.begin() + 1; bodyPart != bodyParts.end(); bodyPart++)
		if (bodyPart->BoundingBox.Intersects(rect))
			return true;
	return false;
}

// Updates the player.
void Player::Update(float elapsedGameTime, Rectangle appleHitbox, Rectangle bulletHitbox, Player player)
{
	if (!this->alive)
		return;

	// Updates all the body parts.
	for (int i = 0; i < bodyParts.size(); i++)
	{
		if (i - 1 >= 0 && i + 1 < bodyParts.size())
			bodyParts.at(i).Update(elapsedGameTime, bodyParts.at(i - 1), bodyParts.at(i + 1));
		else if (i + 1 < bodyParts.size())
			bodyParts.at(i).Update(elapsedGameTime, bodyParts.at(i), bodyParts.at(i + 1));
		else if (i - 1 >= 0)
			bodyParts.at(i).Update(elapsedGameTime, bodyParts.at(i - 1), bodyParts.at(i));
		else
			bodyParts.at(i).Update(elapsedGameTime, bodyParts.at(i), bodyParts.at(i));
	}

	// Check if you're eating yourself = you die.
	BodyPart& head = this->bodyParts.at(0);
	for (std::vector<BodyPart>::iterator bodyPart = bodyParts.begin() + 2; bodyPart != bodyParts.end(); bodyPart++)
		if (head.BoundingBox.Intersects(bodyPart->BoundingBox))
		{
			Rectangle rect(0, 0, BODYSIZE * 2, BODYSIZE);
			{
				rect.x = SnakeGame::Roll(0, SCREEN_WIDTH - BODYSIZE * 2);
				rect.y = SnakeGame::Roll(0, SCREEN_HEIGHT - BODYSIZE);
			} while (rect.Intersects(appleHitbox) || rect.Intersects(bulletHitbox) || player.Collides(rect));
			Vector2 pos(rect.x, rect.y);

			Reset(pos, false);
			break;
		}
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
	SDL_FreeSurface(this->bodyTexture);
	SDL_FreeSurface(this->headTexture);
	SDL_FreeSurface(this->tailTexture);
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
		bodyPosition.X -= BODYSIZE;
		tail.Position.X -= BODYSIZE;
	}
	else if (lastAddedBody.Motion.X == -1 && lastAddedBody.Motion.Y == 0)
	{
		bodyPosition.X += BODYSIZE;
		tail.Position.X += BODYSIZE;
	}
	else if (lastAddedBody.Motion.X == 0 && lastAddedBody.Motion.Y == 1)
	{
		bodyPosition.Y -= BODYSIZE;
		tail.Position.Y -= BODYSIZE;
	}
	else if (lastAddedBody.Motion.X == 0 && lastAddedBody.Motion.Y == -1)
	{
		bodyPosition.Y += BODYSIZE;
		tail.Position.Y += BODYSIZE;
	}

	if ((body.Initialize(bodyPosition, lastAddedBody.Motion, bodyTexture)) == false)
		return;

	std::vector<BodyPart>::iterator it = bodyParts.end() - 1;

	this->bodyParts.insert(it, body);
}

void Player::RemoveBodyPart()
{
	if (this->bodyParts.size() > 2)
	{
		BodyPart& tail = this->bodyParts.at(this->bodyParts.size() - 1);
		BodyPart& remove = this->bodyParts.at(this->bodyParts.size() - 2);

		tail.Position = remove.Position;
		tail.Motion = remove.Motion;
		tail.TargetMotion = remove.TargetMotion;
		tail.TargetPosition = remove.TargetPosition;
		tail.BoundingBox = remove.BoundingBox;

		this->bodyParts.erase(this->bodyParts.end() - 1);
		this->Score -= REMOVEBODYPART_SCORE;
	}
}

void Player::Reset(Vector2 startPos, bool spawnUpper)
{
	if (this->bodyParts.size() > 2)
	{
		BodyPart& head = this->bodyParts.at(0);
		BodyPart& tail = this->bodyParts.at(this->bodyParts.size() - 1);
		BodyPart& body = this->bodyParts.at(1);

		tail.Position = body.Position;
		tail.Motion = head.Motion;
		tail.TargetMotion = head.TargetMotion;
		tail.TargetPosition = head.TargetPosition;
		tail.BoundingBox = body.BoundingBox;

		this->bodyParts.erase(this->bodyParts.begin() + 1, this->bodyParts.end() - 1);
	}
	this->Score = 0;

	Vector2 pos(BODYSIZE,0);

	if (startPos.X == 0 && startPos.Y == 0)
	{
		if (!spawnUpper)
			pos.Y = SCREEN_HEIGHT - BODYSIZE;
	}
	else
	{
		pos.X = startPos.X;
		pos.Y = startPos.Y;
	}

	for(std::vector<BodyPart>::iterator bodyPart = bodyParts.begin(); bodyPart != bodyParts.end(); bodyPart++)
	{
		bodyPart->Position = pos;
		bodyPart->BoundingBox.x = static_cast<int>(pos.X);
		bodyPart->BoundingBox.y = static_cast<int>(pos.Y);
		bodyPart->Motion.Y = 0;
		bodyPart->Motion.X = 1;
		pos.X -= BODYSIZE;
	}
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
#include "Player.h"

// Construct a new player at the position of 0, 0
Player::Player()
	: position(0, 0) { }

// Initialize the player.
bool Player::Initialize(Vector2 position, SDL_Surface* texture)
{
	// If the texture is null return false.
	if (texture == NULL)
		return false;

	// Set the position and texture.
	this->position = position;
	this->texture = texture;

	return true;
}

// Handles the input for the player.
void Player::HandleInput()
{
}

// Updates the player.
void Player::Update(float elapsedGameTime)
{
	// Moves the player in a speed of 120 pixels / second (?)
	position.X += elapsedGameTime * 120;

	// Prevent the player from going off-screen.
	if (position.X > 1280 - 50)
		position.X = -50;
}

// Draws the player.
void Player::Draw(float elapsedGameTime, SpriteBatch* spriteBatch)
{
	// Draws the players sprite.
	spriteBatch->Draw(this->texture, this->position, Rectangle(0, 0, 100, 10));
}

// Cleans up all the variables.
void Player::Cleanup()
{
	// Free up the texture surface.
	SDL_FreeSurface(this->texture);
}
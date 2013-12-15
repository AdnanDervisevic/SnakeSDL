#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "SDL/SDL.h"
#include "Vector2.h"
#include <vector>
#include "BodyPart.h"

class Player
{
public:
	Player();

	bool Initialize(Vector2 position, SDL_Surface* headTexture, SDL_Surface* bodyTexture, SDL_Surface* tailTexture);
	void HandleInput();
	void Update(float elapsedGameTime, Rectangle appleHitbox, Rectangle bulletHitbox, Rectangle mouseHitbox, Player player);
	void Draw(float elapsedGameTime, SpriteBatch* spriteBatch);
	void Cleanup();

	bool Collides(const Rectangle& player);
	bool HeadCollides(const Player& player);
	bool HeadCollides(const Rectangle& rect);
	bool BodyCollides(const Rectangle& rect);

	void Turn(int direction);
	void AddBodyPart();
	void RemoveBodyPart();
	void Reset(Vector2 startPos, bool spawnUpper);

	bool IsAlive() { return alive; }

	std::vector<BodyPart> bodyParts;
	int Score;

private:
	SDL_Surface* headTexture;
	SDL_Surface* bodyTexture;
	SDL_Surface* tailTexture;
	bool alive;

	float outsideTimer;
	bool outsideTimerEnabled;
};

#endif
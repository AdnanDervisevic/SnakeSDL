#ifndef _SNAKEGAME_H_
#define _SNAKEGAME_H_

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "GlobalVariables.h"
#include "Texture.h"
#include "SpriteBatch.h"
#include "Player.h"
#include "SpriteFont.h"
#include "SoundEffect.h"
#include "Music.h"
#include <cstdlib>
#include <string>
#include <time.h>

class SnakeGame
{
public:
	SnakeGame();

	int Run();
	bool Initialize();
	void HandleSDLInput(SDL_Event* event);
	void HandleInput();
	void Update(float elapsedGameTime);
	void Draw(float elapsedGameTime);
	void Cleanup();

	void Fire();

	static int Roll(int min, int max)
	{
	   return min + static_cast<int>( rand() / static_cast<double>(0x7fff) * (max - min) );
	}

	SoundEffect* chomp;

private:
	Uint32 currentTicks;
	Uint32 oldTicks;
	bool running;
	SDL_Surface* backbuffer;
	SpriteBatch* spriteBatch;

	TTF_Font* font;
	SoundEffect* proj;
	SoundEffect* coin;

	Player player1;
	Player player2;

	SDL_Surface* startScreen;
	SDL_Surface* winnerScreen;

	bool gameStarted;

	SDL_Surface* bulletTexture;
	Rectangle bulletSpawnHitbox;
	int bulletBelongsToPlayer;
	float bulletSpawnTimer;

	Vector2 bulletPosition;
	Vector2 bulletMotion;
	bool bulletFired;
	Rectangle bulletHitbox;

	SDL_Surface* appleTexture;
	Rectangle appleHitbox;
	float appleSpawnTimer;
	bool appleSpawned;

	SDL_Joystick *stick;

};

#endif
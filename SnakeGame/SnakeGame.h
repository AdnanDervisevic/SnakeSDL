#ifndef _SNAKEGAME_H_
#define _SNAKEGAME_H_

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "GlobalVariables.h"
#include "Texture.h"
#include "SpriteBatch.h"
#include "Player.h"
#include "SpriteFont.h"
#include <cstdlib>
#include <string>
#include <time.h>
#include <stdio.h>

class SnakeGame
{
public:
	SnakeGame();

	int Run();
	bool Initialize();
	void HandleSDLInput(SDL_Event* event);
	void HandleInput(float elapsedGameTime);
	void Update(float elapsedGameTime);
	void Draw(float elapsedGameTime);
	void Cleanup();

	void Fire();

	static int Roll(int min, int max)
	{
		return rand() % (max + 1) + min;
	}

private:
	Uint32 currentTicks;
	Uint32 oldTicks;
	bool running;
	SDL_Surface* backbuffer;
	SpriteBatch* spriteBatch;

	TTF_Font* font;

	Player player1;
	Player player2;

	SDL_Surface* startScreen;
	SDL_Surface* brownWinnerScreen;
	SDL_Surface* blueWinnerScreen;

	float buttonTimer;
	bool enableButtonTimer;
	int buttonReleases;

	bool gameStarted;

	SDL_Surface* bulletTexture;
	Rectangle bulletSpawnHitbox;
	int bulletBelongsToPlayer;
	float bulletSpawnTimer;

	SDL_Surface* mouseTexture;
	Rectangle mouseSpawnHitbox;
	float mouseSpawnTimer;
	float mouseNextSpawnTime;
	bool mouseSpawned;
	Vector2 mouseMotion;
	Vector2 mousePosition;

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
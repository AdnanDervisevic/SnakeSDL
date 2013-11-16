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

private:
	Uint32 currentTicks;
	Uint32 oldTicks;
	bool running;
	SDL_Surface* backbuffer;
	SpriteBatch* spriteBatch;

	TTF_Font* font;
	SoundEffect* proj;

	Player player;

};

#endif
#include "SnakeGame.h"

// Creates a new Snake object.
SnakeGame::SnakeGame()
{
	backbuffer = NULL;
	spriteBatch = NULL;
	running = true;
}

// This method is called to run the game.
int SnakeGame::Run()
{
	// Initialize the game.
	if (Initialize() == false)
		return -1;

	SDL_Event event;

	currentTicks = SDL_GetTicks();

	// Run until you close the window.
	while (running)
	{
		// Calculate the delta time since the last run.
		oldTicks = currentTicks;
		currentTicks = SDL_GetTicks();
		float dt = (currentTicks - oldTicks) / 1000.0f;
		
		// Handle the SDL Input
		while (SDL_PollEvent(&event))
			HandleSDLInput(&event);

		// Hand regular input.
		HandleInput();

		// Update and Draw the game.
		Update(dt);
		Draw(dt);
	}

	// Clean up the variables.
	Cleanup();

	return 0;
}

// Here we Initialize our game.
bool SnakeGame::Initialize()
{
	// Initialize the sdl.
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;

	// Create our display surface.
	if ((backbuffer = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL)
		return false;

	// Create the spritebatch.
	spriteBatch = new SpriteBatch(backbuffer);

	// Change our window title
	SDL_WM_SetCaption(WINDOW_TITLE, NULL);

	// Load the player texture.	
	if (player.Initialize(Vector2(0, 0), Texture::Load("Assets/playerTexture.bmp")) == false)
		return false;

	return true;
}

// Here we handle all the input coming from the SDL library.
void SnakeGame::HandleSDLInput(SDL_Event* event)
{
	// Check if we want to quit
	if (event->type == SDL_QUIT)
		running = false;
}

// Here we handle all the input not coming from the SDL.
void SnakeGame::HandleInput()
{
}

// Here we update our game.
void SnakeGame::Update(float elapsedGameTime)
{
	player.Update(elapsedGameTime);
}

// Here we draw our game.
void SnakeGame::Draw(float elapsedGameTime)
{
	// Clears the backbuffer.
	SDL_FillRect(backbuffer, NULL, SDL_MapRGB(backbuffer->format, 0, 0, 0));

	// Draws the player.
	player.Draw(elapsedGameTime, this->spriteBatch);

	// Shows the backbuffer.
	SDL_Flip(backbuffer);
}

// Here we clean up and releases our resources.
void SnakeGame::Cleanup()
{
	// Cleans up the players variables.
	player.Cleanup();

	// Clean up variables.
	delete spriteBatch;
	SDL_FreeSurface(backbuffer);
	SDL_Quit();
}

// The main entry point of the program.
int main(int argc, char* argv[])
{
	// Create the game and run it.
	SnakeGame snake;

	return snake.Run();
}
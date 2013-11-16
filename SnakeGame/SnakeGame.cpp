#include "SnakeGame.h"
#include <iostream>

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

	if (TTF_Init() == -1)
		return false;

	// Create the spritebatch.
	spriteBatch = new SpriteBatch(backbuffer);

	// Change our window title
	SDL_WM_SetCaption(WINDOW_TITLE, NULL);

	// Load the player texture.	
	if (player.Initialize(Vector2(0, 0), Texture::Load("Assets/playerTexture.bmp")) == false)
		return false;

	// Loads a font using the SpriteFont helper.
	if ((font = SpriteFont::Load("Assets/Lazy.ttf", 28)) == NULL)
		return false;

	// Open the mixer, this is needed to play any audio.
	if (Mix_OpenAudio(AUDIO_RATE, AUDIO_FORMAT, AUDIO_CHANNELS, AUDIO_BUFFERS))
		return false;

	// Load the sound effect.
	if ((proj = SoundEffect::Load("Assets/proj.wav")) == NULL)
		return false;

	// Load the background music, music is static because you can only play one song at the time.
	if ((Music::Load("Assets/background.ogg")) == false)
		return false;

	// Set the volume of the music to 10%
	Music::Volume(10);

	// Start the music and loop it.
	Music::Play(true);

	// Set the volume of the projectile effect.
	proj->Volume(30);

	// Start the sound effect and loop it.
	proj->Play(true);

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
	
	// Draw a test string using the loaded font.
	spriteBatch->DrawString("Testar min spritefont LOL", Vector2(100, 100), font, Color(255, 255, 255));

	// Shows the backbuffer.
	SDL_Flip(backbuffer);
}

// Here we clean up and releases our resources.
void SnakeGame::Cleanup()
{
	// Cleans up the players variables.
	player.Cleanup();

	// Close the font.
	TTF_CloseFont(font);
	font = NULL;

	// Dispose the projectile sound effect.
	proj->Dispose();
	proj = NULL;

	// Dispose the background music.
	Music::Dispose();

	// Clean up variables.
	Mix_CloseAudio(); // Close the audio
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
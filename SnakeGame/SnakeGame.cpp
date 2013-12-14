#include "SnakeGame.h"
#include "GPIO.h"
#include <iostream>
#include <cstring>

// Creates a new Snake object.
SnakeGame::SnakeGame() : 
	bulletSpawnHitbox(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 10, 20, 20), 
	bulletBelongsToPlayer(0), bulletSpawnTimer(0), bulletHitbox(0, 0, 20, 20), bulletMotion(0, 0), bulletPosition(0, 0),
	appleHitbox(0, 0, 20, 20), appleSpawnTimer(0), appleSpawned(false)
{
	backbuffer = NULL;
	spriteBatch = NULL;
	running = true;
	this->bulletFired = false;
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

		SDL_Delay(1);
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

	// Load player1.
	if (player1.Initialize(Vector2(200, 0), Texture::Load("Assets/playerTexture.png"), Texture::Load("Assets/playerTexture.png"), Texture::Load("Assets/playerTexture.png")) == false)
		return false;

	// Load player2
	if (player2.Initialize(Vector2(200, SCREEN_HEIGHT - BODYSIZE), Texture::Load("Assets/player2Texture.png"), Texture::Load("Assets/player2Texture.png"), Texture::Load("Assets/player2Texture.png")) == false)
		return false;

	if ((bulletTexture = Texture::Load("Assets/bullet.png")) == NULL)
		return false;

	if ((appleTexture = Texture::Load("Assets/apple.png")) == NULL)
		return false;

	if ((startScreen = Texture::Load("Assets/startScreen.png")) == NULL)
		return false;

	if ((winnerScreen = Texture::Load("Assets/winnerScreen.png")) == NULL)
		return false;

	// Loads a font using the SpriteFont helper.
	if ((font = SpriteFont::Load("Assets/DolceVita.ttf", 22)) == NULL)
		return false;

	// Open the mixer, this is needed to play any audio.
	if (Mix_OpenAudio(AUDIO_RATE, AUDIO_FORMAT, AUDIO_CHANNELS, AUDIO_BUFFERS))
		return false;

	// Load the sound effect.
	if ((proj = SoundEffect::Load("Assets/proj.wav")) == NULL)
		return false;

	if ((chomp = SoundEffect::Load("Assets/chomp.wav")) == NULL)
		return false;

	if ((coin = SoundEffect::Load("Assets/coin.wav")) == NULL)
		return false;

	// Load the background music, music is static because you can only play one song at the time.
	if ((Music::Load("Assets/background.ogg")) == false)
		return false;

	gameStarted = false;

	// Set the rand time to NULL for true randomness.
	srand(time(NULL));

	// Set the volume of the music to 10%
	Music::Volume(10);

	// Start the music and loop it.
	Music::Play(true);

	// Set the volume of the projectile effect.
	//proj->Volume(30);
	//coin->Volume(30);
	//chomp->Volume(30);

	if(SDL_NumJoysticks() != 0)
		stick = SDL_JoystickOpen(0);
	
	// Enabled GPIO pins
	if (GPIOExport(GPIO_BUTTON) == -1)
		return false;
	if (GPIOExport(GPIO_BUTTONUP) == -1)
		return false;
	if (GPIOExport(GPIO_BUTTONDOWN) == -1)
		return false;
	if (GPIOExport(GPIO_BUTTONRIGHT) == -1)
		return false;
	if (GPIOExport(GPIO_BUTTONLEFT) == -1)
		return false;
		
	int maxTries = 10;
	int pinValue = 0;
	int tries = 0;
	do
	{
		if (tries == maxTries)
			return false;

		pinValue = GPIODirection(GPIO_BUTTON, IN);

		if (pinValue == -1)
		{
			tries++;
			usleep(1000000);
		}

	} while (pinValue == -1);

	pinValue = 0;
	tries = 0;
	do
	{
		if (tries == maxTries)
			return false;

		pinValue = GPIODirection(GPIO_BUTTONUP, IN);

		if (pinValue == -1)
		{
			tries++;
			usleep(1000000);
		}

	} while (pinValue == -1);

	pinValue = 0;
	tries = 0;
	do
	{
		if (tries == maxTries)
			return false;

		pinValue = GPIODirection(GPIO_BUTTONDOWN, IN);

		if (pinValue == -1)
		{
			tries++;
			usleep(1000000);
		}

	} while (pinValue == -1);

	pinValue = 0;
	tries = 0;
	do
	{
		if (tries == maxTries)
			return false;

		pinValue = GPIODirection(GPIO_BUTTONRIGHT, IN);

		if (pinValue == -1)
		{
			tries++;
			usleep(1000000);
		}

	} while (pinValue == -1);

	pinValue = 0;
	tries = 0;
	do
	{
		if (tries == maxTries)
			return false;

		pinValue = GPIODirection(GPIO_BUTTONLEFT, IN);

		if (pinValue == -1)
		{
			tries++;
			usleep(1000000);
		}

	} while (pinValue == -1);
        
	return true;
}

// Here we handle all the input coming from the SDL library.
void SnakeGame::HandleSDLInput(SDL_Event* event)
{
	if (!gameStarted)
	{
		switch (event->type)
		{
		case SDL_QUIT:
			running = false;
			break;

		case SDL_KEYDOWN:
			if (event->key.keysym.sym == SDLK_ESCAPE)
				running = false;
			else if (event->key.keysym.sym == SDLK_KP_ENTER)
			{
				gameStarted = true;
				player1.Reset(Vector2(0, 0), true);
				player2.Reset(Vector2(0, 0), false);
				this->bulletBelongsToPlayer = 0;
				this->bulletSpawnTimer = 0;
				this->bulletFired = false;
				this->appleSpawned = false;
				this->appleSpawnTimer = 0;
			}
			break;
		}
	}
	else
	{
		// Check if we want to quit
		switch (event->type)
		{
		case SDL_QUIT:
			running = false;
			break;

			// Om inte joystick är kopplad -- debugging
		case SDL_KEYDOWN:
			if (event->key.keysym.sym == SDLK_w)
				player1.Turn(DIRECTION_UP);
			else if (event->key.keysym.sym == SDLK_s)
				player1.Turn(DIRECTION_DOWN);
			else if (event->key.keysym.sym == SDLK_d)
				player1.Turn(DIRECTION_RIGHT);
			else if (event->key.keysym.sym == SDLK_a)
				player1.Turn(DIRECTION_LEFT);

			else if (event->key.keysym.sym == SDLK_UP)
				player2.Turn(DIRECTION_UP);
			else if (event->key.keysym.sym == SDLK_DOWN)
				player2.Turn(DIRECTION_DOWN);
			else if (event->key.keysym.sym == SDLK_RIGHT)
				player2.Turn(DIRECTION_RIGHT);
			else if (event->key.keysym.sym == SDLK_LEFT)
				player2.Turn(DIRECTION_LEFT);

			else if (event->key.keysym.sym == SDLK_SPACE && this->bulletBelongsToPlayer > 0)
				Fire();
			break;

		case SDL_JOYHATMOTION:
			// Access the hat movement - For arcade stick connected via USB, one of the movement options
			if (stick != NULL)
			{
				if (SDL_JoystickGetHat(stick, 0) == 0x01)
					player1.Turn(DIRECTION_UP);
				else if (SDL_JoystickGetHat(stick, 0) == 0x04)
					player1.Turn(DIRECTION_DOWN);
				else if (SDL_JoystickGetHat(stick, 0) == 0x08 || SDL_JoystickGetHat(stick, 0) == (0x08 | 0x04) || SDL_JoystickGetHat(stick, 0) == (0x08 | 0x01))
					player1.Turn(DIRECTION_LEFT);
				else if (SDL_JoystickGetHat(stick, 0) == 0x02 || SDL_JoystickGetHat(stick, 0) == (0x02 | 0x04) || SDL_JoystickGetHat(stick, 0) == (0x02 | 0x01))
					player1.Turn(DIRECTION_RIGHT);
			}
			break;
		}
	}
}

// Here we handle all the input not coming from the SDL.
void SnakeGame::HandleInput()
{
	if (gameStarted)
	{
		int pinValue;


		if ((pinValue = GPIORead(GPIO_BUTTONDOWN)) == 0)
		{
			usleep(10000);
			if ((pinValue = GPIORead(GPIO_BUTTONDOWN)) == 0)
			{
				player1.Turn(DIRECTION_DOWN);
				//printf("Turn Down\n");
			}
		}

		if ((pinValue = GPIORead(GPIO_BUTTONRIGHT)) == 0)
		{
			usleep(10000);
			if ((pinValue = GPIORead(GPIO_BUTTONRIGHT)) == 0)
			{
				player1.Turn(DIRECTION_RIGHT);
				//printf("Turn Right\n");
			}
		}

		int pinValue2;

		//if ((pinValue = GPIORead(GPIO_BUTTON)) == 0 && this->bulletBelongsToPlayer > 0)
		//	Fire();

		//printf("%i", pinValue);
		/*
		if (GPIORead(GPIO_BUTTONUP) == 0)
			player1.Turn(DIRECTION_UP);
		else if ((pinValue = GPIORead(GPIO_BUTTONDOWN)) == 1)
			player1.Turn(DIRECTION_DOWN);
		else if ((pinValue2 = GPIORead(GPIO_BUTTONRIGHT)) == 1)
			player1.Turn(DIRECTION_RIGHT);
		else if (GPIORead(GPIO_BUTTONLEFT) == 0)
			player1.Turn(DIRECTION_LEFT);
*/
		//printf("DOWN: %i\n", pinValue);
		//printf("RIGHT: %i", pinValue2);
	}
}

void SnakeGame::Fire()
{
	proj->Play(false);
	BodyPart& head = (this->bulletBelongsToPlayer == 1) ? this->player1.bodyParts.at(0) : this->player2.bodyParts.at(0);

	if (head.Motion.X == 1 && head.Motion.Y == 0)
	{
		// Ormen rör sig till höger
		this->bulletHitbox.x = static_cast<int>(head.Position.X + 5);
		this->bulletHitbox.y = static_cast<int>(head.Position.Y);
	}
	else if (head.Motion.X == -1 && head.Motion.Y == 0)
	{
		// Ormen rör sig till vänster
		this->bulletHitbox.x = static_cast<int>(head.Position.X - 5);
		this->bulletHitbox.y = static_cast<int>(head.Position.Y);
	}
	else if (head.Motion.X == 0 && head.Motion.Y == 1)
	{
		// Ormen rör sig ner
		this->bulletHitbox.x = static_cast<int>(head.Position.X);
		this->bulletHitbox.y = static_cast<int>(head.Position.Y + 5);
	}
	else if (head.Motion.X == 0 && head.Motion.Y == -1)
	{
		// Ormen rör sig upp
		this->bulletHitbox.x = static_cast<int>(head.Position.X );
		this->bulletHitbox.y = static_cast<int>(head.Position.Y - 5);
	}
	this->bulletMotion = head.Motion;
	this->bulletPosition.X = this->bulletHitbox.x;
	this->bulletPosition.Y = this->bulletHitbox.y;
	this->bulletFired = true;

	// Start the sound effect and loop it.
}

// Here we update our game.
void SnakeGame::Update(float elapsedGameTime)
{
	if (gameStarted)
	{
		if (!bulletFired)
		{
			player1.Update(*chomp, elapsedGameTime, appleHitbox, bulletHitbox, player2);
			player2.Update(*chomp, elapsedGameTime, appleHitbox, bulletHitbox, player1);

			if (this->bulletBelongsToPlayer == 0)
			{
				if (this->player1.HeadCollides(this->bulletSpawnHitbox))
				{
					this->bulletBelongsToPlayer = 1;
					this->coin->Play(false);
				}
				else if (this->player2.HeadCollides(this->bulletSpawnHitbox))
				{
					this->bulletBelongsToPlayer = 2;
					printf("Player 2 picked up the bullet");
					this->coin->Play(false);
				}
			}

			bool player1EatsPlayer2 = player1.HeadCollides(player2);
			bool player2EatsPlayer1 = player2.HeadCollides(player1);

			if (player1EatsPlayer2 && player2EatsPlayer1)
			{
				player1.Reset(Vector2(0, 0), true);
				player2.Reset(Vector2(0, 0), false);
				chomp->Play(false);
			}
			else if (player1EatsPlayer2 && !player2EatsPlayer1)
			{
				Rectangle rect(0, 0, BODYSIZE * 2, BODYSIZE);
				do
				{
					rect.x = Roll(0, SCREEN_WIDTH - BODYSIZE * 2);
					rect.y = Roll(0, SCREEN_HEIGHT - BODYSIZE);
				} while (rect.Intersects(this->appleHitbox) || rect.Intersects(this->bulletHitbox) || player2.Collides(rect));
				Vector2 pos(rect.x, rect.y);
				player1.Reset(pos, false);
				chomp->Play(false);
			}
			else if (!player1EatsPlayer2 && player2EatsPlayer1)
			{
				Rectangle rect(0, 0, BODYSIZE * 2, BODYSIZE);
				do
				{
					rect.x = Roll(0, SCREEN_WIDTH - BODYSIZE * 2);
					rect.y = Roll(0, SCREEN_HEIGHT - BODYSIZE);
				} while (rect.Intersects(this->appleHitbox) || rect.Intersects(this->bulletHitbox) || player2.Collides(rect));
				Vector2 pos(rect.x, rect.y);
				player2.Reset(pos, false);
				chomp->Play(false);
			}

			if (this->appleSpawned && player1.HeadCollides(this->appleHitbox))
			{
				player1.Score += APPLE_SCORE;
				player1.AddBodyPart();
				this->appleSpawnTimer = 0;
				this->appleSpawned = false;
				this->coin->Play(false);
			}
			else if (this->appleSpawned && player2.HeadCollides(this->appleHitbox))
			{
				player2.Score += APPLE_SCORE;
				player2.AddBodyPart();
				this->appleSpawnTimer = 0;
				this->appleSpawned = false;
				printf("Player 2 picked up the apple");
				this->coin->Play(false);
			}

			printf("%f - %d", this->appleSpawnTimer, this->appleSpawned);
			this->appleSpawnTimer += elapsedGameTime;
			if (this->appleSpawnTimer >= 3 && !this->appleSpawned)
			{
				printf("Apple Spawn");
				this->appleSpawned = true;

				do
				{
					this->appleHitbox.x = SnakeGame::Roll(0, SCREEN_WIDTH - BODYSIZE);
					this->appleHitbox.y = SnakeGame::Roll(0, SCREEN_HEIGHT - BODYSIZE);
				} while (this->appleHitbox.Intersects(this->bulletHitbox) || player1.Collides(this->appleHitbox) || player2.Collides(this->appleHitbox));
			}
		}
		else
		{
			printf("Fire!");
			this->bulletPosition.X += (this->bulletMotion.X * 120 * elapsedGameTime);
			this->bulletPosition.Y += (this->bulletMotion.Y * 120 * elapsedGameTime);

			this->bulletHitbox.x = static_cast<int>(this->bulletPosition.X);
			this->bulletHitbox.y = static_cast<int>(this->bulletPosition.Y);

			if (this->bulletPosition.X > SCREEN_WIDTH || this->bulletPosition.X < 0 || this->bulletPosition.Y > SCREEN_HEIGHT || this->bulletPosition.Y < 0)
			{
				this->bulletFired = false;
				this->bulletBelongsToPlayer = 0;
				this->bulletSpawnTimer = 0;
			}
			else
			{
				if (this->bulletBelongsToPlayer == 1)
				{
					if (this->player2.HeadCollides(this->bulletHitbox))
					{
						// Headshot :!
						Rectangle rect(0, 0, BODYSIZE * 2, BODYSIZE);
						do
						{
							rect.x = Roll(0, SCREEN_WIDTH - BODYSIZE * 2);
							rect.y = Roll(0, SCREEN_HEIGHT - BODYSIZE);
						} while (rect.Intersects(this->appleHitbox) || rect.Intersects(this->bulletHitbox) || player2.Collides(rect));
						Vector2 pos(rect.x, rect.y);

						this->player2.Reset(pos, false);
						this->bulletFired = false;
						this->bulletBelongsToPlayer = 0;
						this->bulletSpawnTimer = 0;
						chomp->Play(false);
					}
					else if (this->player2.BodyCollides(this->bulletHitbox))
					{
						// Bodyshot: !
						this->player2.RemoveBodyPart();
						this->bulletFired = false;
						this->bulletBelongsToPlayer = 0;
						this->bulletSpawnTimer = 0;
						chomp->Play(false);
					}
				}
				else if (this->bulletBelongsToPlayer == 2)
				{
					if (this->player1.HeadCollides(this->bulletHitbox))
					{
						// Headshot :!
						Rectangle rect(0, 0, BODYSIZE * 2, BODYSIZE);
						do
						{
							rect.x = Roll(0, SCREEN_WIDTH - BODYSIZE * 2);
							rect.y = Roll(0, SCREEN_HEIGHT - BODYSIZE);
						} while (rect.Intersects(this->appleHitbox) || rect.Intersects(this->bulletHitbox) || player2.Collides(rect));
						Vector2 pos(rect.x, rect.y);

						this->player1.Reset(pos, false);
						this->bulletFired = false;
						this->bulletBelongsToPlayer = 0;
						this->bulletSpawnTimer = 0;
						chomp->Play(false);
					}
					else if (this->player1.BodyCollides(this->bulletHitbox))
					{
						// Bodyshot: !
						this->player1.RemoveBodyPart();
						this->bulletFired = false;
						this->bulletBelongsToPlayer = 0;
						this->bulletSpawnTimer = 0;
						chomp->Play(false);
					}
				}
			}
		}

		if (player1.Score >= WINNER_SCORE || player2.Score >= WINNER_SCORE)
			gameStarted = false;
	}
}

// Here we draw our game.
void SnakeGame::Draw(float elapsedGameTime)
{
	// Clears the backbuffer.
	SDL_FillRect(backbuffer, NULL, SDL_MapRGB(backbuffer->format, 0, 0, 0));

	if (!gameStarted)
	{
		if (player1.Score > 0 || player2.Score > 0)
		{
			if (player1.Score > player2.Score)
			{
				//std::string message = "Score: " + std::to_string(this->player1.Score) + " over " + std::to_string(this->player2.Score);

				// Player 1 wins:!
				this->spriteBatch->Draw(this->winnerScreen, Vector2(0, 0));
				this->spriteBatch->DrawString("Player 1 Wins", Vector2(200, 150), this->font, Color(255, 255, 255));
				//this->spriteBatch->DrawString(message.c_str(), Vector2(200, 180), this->font, Color(255, 255, 255));
			}
			else if (player1.Score < player2.Score)
			{
				// Player 2 Wins
				//std::string message = "Score: " + std::to_string(this->player2.Score) + " over " + std::to_string(this->player1.Score);

				this->spriteBatch->Draw(this->winnerScreen, Vector2(0, 0));
				this->spriteBatch->DrawString("Player 2 Wins", Vector2(200, 150), this->font, Color(255, 255, 255));
				//this->spriteBatch->DrawString(message.c_str(), Vector2(200, 180), this->font, Color(255, 255, 255));
			}
		}
		else
		{
			// Else draw the start screen.
			this->spriteBatch->Draw(this->startScreen, Vector2(0, 0));
		}
	}
	else
	{
		this->bulletSpawnTimer += elapsedGameTime;
		if (this->bulletSpawnTimer >= 4 && this->bulletBelongsToPlayer == 0)
		{
			this->spriteBatch->Draw(this->bulletTexture, Vector2(this->bulletSpawnHitbox.x, this->bulletSpawnHitbox.y));
		}

		if (this->appleSpawned)
			this->spriteBatch->Draw(this->appleTexture, Vector2(this->appleHitbox.x, this->appleHitbox.y));

		if (this->bulletFired)
		{
			this->spriteBatch->Draw(this->bulletTexture, this->bulletPosition);
		}

		// Draws the players.
		player1.Draw(elapsedGameTime, this->spriteBatch);
		player2.Draw(elapsedGameTime, this->spriteBatch);
	
		char brownScore[32];
		sprintf(brownScore, "%d", this->player1.Score);

		char brownScoreString[124] = "Brown Score: ";

		strcat(brownScoreString, brownScore);

		char blueScore[32];
		sprintf(blueScore, "%d", this->player2.Score);

		char blueScoreString[124] = "Blue Score: ";
		strcat(blueScoreString, blueScore);
		
		this->spriteBatch->DrawString(brownScoreString, Vector2(2, 2), this->font, Color(255, 255, 255));
		this->spriteBatch->DrawString(blueScoreString, Vector2(2, SCREEN_HEIGHT - 22), this->font, Color(255, 255, 255));
	}

	// Shows the backbuffer.
	SDL_Flip(backbuffer);
}

// Here we clean up and releases our resources.
void SnakeGame::Cleanup()
{
	
	GPIOUnexport(GPIO_BUTTON);
	GPIOUnexport(GPIO_BUTTONUP);
	GPIOUnexport(GPIO_BUTTONDOWN);
	GPIOUnexport(GPIO_BUTTONRIGHT);
	GPIOUnexport(GPIO_BUTTONLEFT);
	
	// Cleans up the players variables.
	player1.Cleanup();
	player2.Cleanup();

	SDL_FreeSurface(this->startScreen);
	SDL_FreeSurface(this->winnerScreen);
	
	SDL_FreeSurface(this->bulletTexture);
	SDL_FreeSurface(this->appleTexture);

	// Close the font.
	TTF_CloseFont(font);
	font = NULL;

	// Dispose the projectile sound effect.
	proj->Dispose();
	delete proj;

	chomp->Dispose();
	delete chomp;

	coin->Dispose();
	delete coin;

	// Dispose the background music.
	Music::Dispose();

	//CLOSE JOYSTICK
	if(stick == NULL)
		SDL_JoystickClose(stick);

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

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

//prototypes of functions
SDL_Surface *load_image( std::string filename );
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );


//screen attributes
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int SCREEN_BPP = 32;

// the surfaces that will be used
SDL_Surface *message = NULL;
SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;

int main( int argc, char* args[] )
{

	//all of sdl init
	if (SDL_Init ( SDL_INIT_EVERYTHING ) == -1)
	{
		return 1;
	}

	//set up the screen
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

	if(screen == NULL)
	{
		return 1;
	}

	SDL_WM_SetCaption( "Test Appen", NULL);

	//load images
	message = load_image("hello.png");
	background = load_image("background.png");

	//apply bg
	apply_surface( 0, 0, background, screen);
	apply_surface( 320, 0, background, screen);
	apply_surface( 0, 240, background, screen);
	apply_surface( 320, 240, background, screen);
	
	apply_surface(180,140,message,screen);

	if(SDL_Flip(screen) == -1)
	{
		return -1;
	}

	SDL_Delay(2000);

	SDL_FreeSurface(message);
	SDL_FreeSurface(background);

	SDL_Quit();

	return 0;
}

SDL_Surface *load_image( std::string filename ) 
{
	//temp storage for loaded image
	SDL_Surface* loadedImage = NULL;

	// the optmized image that will be used
	SDL_Surface* optimizedImage = NULL;

	//loadImage
	loadedImage = IMG_Load(filename.c_str());

	if(loadedImage != NULL)
	{
		//create optimized image
		optimizedImage = SDL_DisplayFormat(loadedImage);

		//freee old image
		SDL_FreeSurface(loadedImage);
	}

	return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
	//temporary rectangle to hold offsets
	SDL_Rect offset;

	//give offsets to rect
	offset.x = x;
	offset.y = y;

	//blit the surface
	SDL_BlitSurface(source, NULL, destination, &offset);
}
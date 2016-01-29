/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#ifdef __linux
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
#elif _WIN32
    #include <SDL.h>
    #include <SDL_image.h>
#endif
#include <stdio.h>
#include <stdbool.h>
#include "LTexture.h"
#include "Particle.h"
#include "Dot.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void myClose();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture* gDotTexture;
LTexture* gRedTexture;
LTexture* gGreenTexture;
LTexture* gBlueTexture;
LTexture* gShimmerTexture;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load dot texture
	gDotTexture = LTexture_create();
	if( !LTexture_loadFromFile(gDotTexture, gRenderer, "dot.bmp" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

	//Load red texture
	gRedTexture = LTexture_create();
	if( !LTexture_loadFromFile(gRedTexture, gRenderer, "red.bmp" ) )
	{
		printf( "Failed to load red texture!\n" );
		success = false;
	}

	//Load green texture
	gGreenTexture = LTexture_create();
	if( !LTexture_loadFromFile(gGreenTexture, gRenderer, "green.bmp" ) )
	{
		printf( "Failed to load green texture!\n" );
		success = false;
	}

	//Load blue texture
	gBlueTexture = LTexture_create();
	if( !LTexture_loadFromFile(gBlueTexture, gRenderer, "blue.bmp" ) )
	{
		printf( "Failed to load blue texture!\n" );
		success = false;
	}

	//Load shimmer texture
	gShimmerTexture = LTexture_create();
	if( !LTexture_loadFromFile(gShimmerTexture, gRenderer, "shimmer.bmp" ) )
	{
		printf( "Failed to load shimmer texture!\n" );
		success = false;
	}

	//Set texture transparency
	LTexture_setAlpha(gRedTexture, 192 );
	LTexture_setAlpha(gGreenTexture, 192 );
	LTexture_setAlpha(gBlueTexture, 192 );
	LTexture_setAlpha(gShimmerTexture, 192 );

	return success;
}

void myClose()
{
	//Free loaded images
	LTexture_destroy(gDotTexture);
	LTexture_destroy(gRedTexture);
	LTexture_destroy(gGreenTexture);
	LTexture_destroy(gBlueTexture);
	LTexture_destroy(gShimmerTexture);

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;
			LTexture* colours[] = {gGreenTexture, gRedTexture, gBlueTexture};

			//The dot that will be moving around on the screen
			Dot dot = Dot_create(gDotTexture, SCREEN_WIDTH, SCREEN_HEIGHT, colours);

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//Handle input for the dot
					Dot_handleEvent(&dot, &e );
				}

				//Move the dot
				Dot_move(&dot);

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render objects
				Dot_render(&dot, gRenderer, colours, gShimmerTexture);

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	myClose();

	return 0;
}

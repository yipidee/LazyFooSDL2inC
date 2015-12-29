/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "LTexture.h"

//Colour definitions in RGBA list format for SDL renderer
#define COLOUR_BLACK 0xFF, 0xFF, 0xFF, 0xFF

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture* gPressTexture;
LTexture* gUpTexture;
LTexture* gDownTexture;
LTexture* gLeftTexture;
LTexture* gRightTexture;

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
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, COLOUR_BLACK );

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

	//Load press texture
	gPressTexture = LTexture_create();
	if( !LTexture_loadFromFile(gPressTexture, gRenderer, "press.png" ) )
	{
		printf( "Failed to load press texture!\n" );
		success = false;
	}

	//Load up texture
	gUpTexture = LTexture_create();
	if( !LTexture_loadFromFile(gUpTexture, gRenderer, "up.png" ) )
	{
		printf( "Failed to load up texture!\n" );
		success = false;
	}

	//Load down texture
	gDownTexture = LTexture_create();
	if( !LTexture_loadFromFile(gDownTexture, gRenderer, "down.png" ) )
	{
		printf( "Failed to load down texture!\n" );
		success = false;
	}

	//Load left texture
	gLeftTexture = LTexture_create();
	if( !LTexture_loadFromFile(gLeftTexture, gRenderer, "left.png" ) )
	{
		printf( "Failed to load left texture!\n" );
		success = false;
	}

	//Load right texture
	gRightTexture = LTexture_create();
	if( !LTexture_loadFromFile(gRightTexture, gRenderer, "right.png" ) )
	{
		printf( "Failed to load right texture!\n" );
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded images
	LTexture_destroy(gPressTexture);
	LTexture_destroy(gUpTexture);
	LTexture_destroy(gDownTexture);
	LTexture_destroy(gLeftTexture);
	LTexture_destroy(gRightTexture);

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

			//Current rendered texture
			LTexture* currentTexture;

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
				}

				//Set texture based on current keystate
				const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
				if( currentKeyStates[ SDL_SCANCODE_UP ] )
				{
					currentTexture = gUpTexture;
				}
				else if( currentKeyStates[ SDL_SCANCODE_DOWN ] )
				{
					currentTexture = gDownTexture;
				}
				else if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
				{
					currentTexture = gLeftTexture;
				}
				else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
				{
					currentTexture = gRightTexture;
				}
				else
				{
					currentTexture = gPressTexture;
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, COLOUR_BLACK );
				SDL_RenderClear( gRenderer );

				//Render current texture
				LTexture_render( currentTexture, gRenderer, 0, 0, NULL, 0, NULL, 0 );

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

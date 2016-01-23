/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, strings, and string streams
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "LTexture.h"
#include "LWindow.h"

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

//Our custom window
LWindow* gWindow;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture* gSceneTexture;


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
		gWindow = LWindow_create();
		if( !LWindow_init(gWindow, SCREEN_WIDTH, SCREEN_HEIGHT))
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = LWindow_createRenderer(gWindow);
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

	//Load scene texture
	gSceneTexture = LTexture_create();
	if( !LTexture_loadFromFile(gSceneTexture, gRenderer, "window.png" ) )
	{
		printf( "Failed to load window texture!\n" );
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded images
	LTexture_destroy(gSceneTexture);

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	LWindow_free(gWindow);

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

					//Handle window events
					LWindow_handleEvent(gWindow, gRenderer, &e );
				}

				//Only draw when not minimized
				if( !LWindow_isMinimized(gWindow) )
				{
					//Clear screen
					SDL_SetRenderDrawColor( gRenderer, COLOUR_BLACK );
					SDL_RenderClear( gRenderer );

					//Render text textures
					LTexture_render(gSceneTexture, gRenderer, ( LWindow_getWidth(gWindow) - LTexture_getWidth(gSceneTexture) ) / 2, ( LWindow_getHeight(gWindow) - LTexture_getHeight(gSceneTexture) ) / 2, NULL, 0, NULL, 0 );

					//Update screen
					SDL_RenderPresent( gRenderer );
				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

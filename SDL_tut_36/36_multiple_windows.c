/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, standard IO, strings, and string streams
#ifdef __linux
#include <SDL2/SDL.h>
#elif _WIN32
#include <SDL.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "LTexture.h"
#include "LWindow.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Total windows
#define TOTAL_WINDOWS  3

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void myClose();

//Our custom windows
LWindow* gWindows[ TOTAL_WINDOWS ];

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
		gWindows[ 0 ] = LWindow_create();
		if( !LWindow_init(gWindows[ 0 ], SCREEN_WIDTH, SCREEN_HEIGHT) )
		{
			printf( "Window 0 could not be created!\n" );
			success = false;
		}
	}

	return success;
}

void myClose()
{
	//Destroy windows
	for( int i = 0; i < TOTAL_WINDOWS; ++i )
	{
		LWindow_free(gWindows[ i ]);
	}

	//Quit SDL subsystems
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
		//Initialize the rest of the windows
		for( int i = 1; i < TOTAL_WINDOWS; ++i )
		{
			gWindows[ i ] = LWindow_create();
			LWindow_init(gWindows[ i ], SCREEN_WIDTH, SCREEN_HEIGHT);
		}

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
				for( int i = 0; i < TOTAL_WINDOWS; ++i )
				{
					LWindow_handleEvent(gWindows[ i ], &e );
				}

				//Pull up window
				if( e.type == SDL_KEYDOWN )
				{
					switch( e.key.keysym.sym )
					{
						case SDLK_1:
						LWindow_focus(gWindows[ 0 ]);
						break;

						case SDLK_2:
						LWindow_focus(gWindows[ 1 ]);
						break;

						case SDLK_3:
						LWindow_focus(gWindows[ 2 ]);
						break;
					}
				}
			}

			//Update all windows
			for( int i = 0; i < TOTAL_WINDOWS; ++i )
			{
				LWindow_render(gWindows[ i ]);
			}

			//Check all windows
			bool allWindowsClosed = true;
			for( int i = 0; i < TOTAL_WINDOWS; ++i )
			{
				if( LWindow_isShown(gWindows[ i ]) )
				{
					allWindowsClosed = false;
					break;
				}
			}

			//Application closed all windows
			if( allWindowsClosed )
			{
				quit = true;
			}
		}
	}

	//Free resources and close SDL
	myClose();

	return 0;
}

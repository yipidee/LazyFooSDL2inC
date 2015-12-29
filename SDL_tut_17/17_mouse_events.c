/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "LTexture.h"
#include "LButton.h"

//Colour definitions in RGBA list format for SDL renderer
#define COLOUR_BLACK 0xFF, 0xFF, 0xFF, 0xFF

//define for no of buttons
#define TOTAL_BUTTONS 4

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Button constants
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;

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

//Mouse button sprites
SDL_Rect gSpriteClips[ BUTTON_SPRITE_TOTAL ];
LTexture* gButtonSpriteSheetTexture;

//Buttons objects
LButton* gButtons[ TOTAL_BUTTONS ];

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

	//Load sprites
	gButtonSpriteSheetTexture = LTexture_create();
	if( !LTexture_loadFromFile(gButtonSpriteSheetTexture, gRenderer, "button.png" ) )
	{
		printf( "Failed to load button sprite texture!\n" );
		success = false;
	}
	else
	{
		//Set sprites
		int i;
		for( i = 0; i < BUTTON_SPRITE_TOTAL; ++i )
		{
			gSpriteClips[ i ].x = 0;
			gSpriteClips[ i ].y = i * 200;
			gSpriteClips[ i ].w = BUTTON_WIDTH;
			gSpriteClips[ i ].h = BUTTON_HEIGHT;
		}

		//Set buttons in corners
		gButtons[ 0 ] = LButton_create(gButtonSpriteSheetTexture, &gSpriteClips, BUTTON_WIDTH, BUTTON_HEIGHT);
		LButton_setPosition(gButtons[0], 0, 0);
		gButtons[ 1 ] = LButton_create(gButtonSpriteSheetTexture, &gSpriteClips, BUTTON_WIDTH, BUTTON_HEIGHT);
		LButton_setPosition(gButtons[1], SCREEN_WIDTH - BUTTON_WIDTH, 0);
		gButtons[ 2 ] = LButton_create(gButtonSpriteSheetTexture, &gSpriteClips, BUTTON_WIDTH, BUTTON_HEIGHT);
		LButton_setPosition(gButtons[2], 0, SCREEN_HEIGHT - BUTTON_HEIGHT);
		gButtons[ 3 ] = LButton_create(gButtonSpriteSheetTexture, &gSpriteClips, BUTTON_WIDTH, BUTTON_HEIGHT);
		LButton_setPosition(gButtons[3], SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT);
	}

	return success;
}

void close()
{
	//Free loaded images
	LTexture_destroy(gButtonSpriteSheetTexture);

	//Free memory associated with buttons
	int i;
	for(i=0; i<TOTAL_BUTTONS; ++i)
    {
        LButton_destroy(gButtons[i] );
	}

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

					//Handle button events
					int i;
					for( i = 0; i < TOTAL_BUTTONS; ++i )
					{
						LButton_handleEvent(gButtons[ i ], &e );
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, COLOUR_BLACK );
				SDL_RenderClear( gRenderer );

				//Render buttons
				int i;
				for( i = 0; i < TOTAL_BUTTONS; ++i )
				{
					LButton_render(gButtons[ i ], gRenderer);
				}

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

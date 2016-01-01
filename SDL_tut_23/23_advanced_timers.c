/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, SDL_ttf, standard IO, strings, and string streams
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include "LTexture.h"
#include "LTimer.h"

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

//Globally used font
TTF_Font* gFont = NULL;

//Scene textures
LTexture* gTimeTextTexture;
LTexture* gPausePromptTexture;
LTexture* gStartPromptTexture;

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

				 //Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
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

	//Open the font
	gFont = TTF_OpenFont( "lazy.ttf", 28 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Set text color as black
		SDL_Color textColor = { 0, 0, 0, 255 };

		//Load stop prompt texture
		gStartPromptTexture = LTexture_create();
		if( !LTexture_loadFromRenderedText(gStartPromptTexture, gRenderer, "Press S to Start or Stop the Timer", gFont, textColor ) )
		{
			printf( "Unable to render start/stop prompt texture!\n" );
			success = false;
		}

		//Load pause prompt texture
		gPausePromptTexture = LTexture_create();
		if( !LTexture_loadFromRenderedText(gPausePromptTexture, gRenderer, "Press P to Pause or Unpause the Timer", gFont, textColor ) )
		{
			printf( "Unable to render pause/unpause prompt texture!\n" );
			success = false;
		}
	}

	return success;
}

void close()
{
	//Free loaded images
	LTexture_destroy(gTimeTextTexture);
	LTexture_destroy(gStartPromptTexture);
	LTexture_destroy(gPausePromptTexture);

	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
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

			//Set text color as black
			SDL_Color textColor = { 0, 0, 0, 255 };

			//The application timer
			LTimer* timer = LTimer_create();

			//In memory text stream --> CHANGED FOR C
			//std::stringstream timeText;
			int buffSize = 45;
			char buff[buffSize];

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
					//Reset start time on return keypress
					else if( e.type == SDL_KEYDOWN )
					{
						//Start/stop
						if( e.key.keysym.sym == SDLK_s )
						{
							if( LTimer_isStarted(timer) )
							{
								LTimer_stop(timer);
							}
							else
							{
								LTimer_start(timer);
							}
						}
						//Pause/unpause
						else if( e.key.keysym.sym == SDLK_p )
						{
							if( LTimer_isPaused(timer) )
							{
								LTimer_unpause(timer);
							}
							else
							{
								LTimer_pause(timer);
							}
						}
					}
				}

				//Set text to be rendered
				//timeText.str( "" );
				//timeText << "Seconds since start time " << ( timer.getTicks() / 1000.f ) ;
				if(snprintf(buff, buffSize, "Seconds since start time %.3f",  LTimer_getTicks(timer) / 1000.f )> buffSize)
                {
                    printf("Oh dear, we have a problem with buffer size\n");
                }

				//Render text
				gTimeTextTexture = LTexture_create();
				if( !LTexture_loadFromRenderedText(gTimeTextTexture, gRenderer, buff, gFont, textColor ) )
				{
					printf( "Unable to render time texture!\n" );
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, COLOUR_BLACK );
				SDL_RenderClear( gRenderer );

				//Render textures
				LTexture_render(gStartPromptTexture, gRenderer, ( SCREEN_WIDTH - LTexture_getWidth(gStartPromptTexture) ) / 2, 0 , NULL, 0, NULL, 0);
				LTexture_render(gPausePromptTexture, gRenderer, ( SCREEN_WIDTH - LTexture_getWidth(gPausePromptTexture) ) / 2, LTexture_getHeight(gStartPromptTexture), NULL, 0, NULL, 0 );
				LTexture_render(gTimeTextTexture, gRenderer, ( SCREEN_WIDTH - LTexture_getWidth(gTimeTextTexture) ) / 2, ( SCREEN_HEIGHT - LTexture_getHeight(gTimeTextTexture) ) / 2, NULL, 0, NULL, 0 );

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
			LTimer_destroy(timer);
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

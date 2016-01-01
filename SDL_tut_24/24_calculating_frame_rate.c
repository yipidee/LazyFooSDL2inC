/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, SDL_ttf, standard IO, strings, and string streams
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
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
LTexture* gFPSTextTexture;

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

	return success;
}

void close()
{
	//Free loaded images
	LTexture_destroy(gFPSTextTexture);

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

			//The frames per second timer
			LTimer* fpsTimer = LTimer_create();

			//In memory text stream
			//std::stringstream timeText;
			int buffSize = 45;
			char buff[buffSize];

			//Start counting frames per second
			int countedFrames = 0;
			LTimer_start(fpsTimer);

			//initialise gFPSTextTexture
			gFPSTextTexture = LTexture_create();

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

				//Calculate and correct fps
				float avgFPS = countedFrames / ( LTimer_getTicks(fpsTimer) / 1000.f );
				if( avgFPS > 2000000 )
				{
					avgFPS = 0;
				}

				//Set text to be rendered
				//timeText.str( "" );
				//timeText << "Average Frames Per Second " << avgFPS;
                if(snprintf(buff, buffSize, "Average Frames Per Second %.f", avgFPS )> buffSize)
                {
                    printf("Oh dear, we have a problem with buffer size\n");
                }
                else
                {
                    //Render text
                    if( !LTexture_loadFromRenderedText(gFPSTextTexture, gRenderer, buff, gFont, textColor ) )
                    {
                        printf( "Unable to render FPS texture!\n" );
                    }

                    //Clear screen
                    SDL_SetRenderDrawColor( gRenderer, COLOUR_BLACK );
                    SDL_RenderClear( gRenderer );

                    //Render textures
                    LTexture_render(gFPSTextTexture, gRenderer, ( SCREEN_WIDTH - LTexture_getWidth(gFPSTextTexture) ) / 2, ( SCREEN_HEIGHT - LTexture_getHeight(gFPSTextTexture) ) / 2, NULL, 0, NULL, 0 );
                }
				//Update screen
				SDL_RenderPresent( gRenderer );
				++countedFrames;
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

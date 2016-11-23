/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, math, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "LTexture.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void cleanup();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture* gArrowTexture;

//Game Controller 1 handler
SDL_Joystick* gGameController = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) < 0 )
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

		//Check for joysticks
		if( SDL_NumJoysticks() < 1 )
		{
			printf( "Warning: No joysticks connected!\n" );
		}
		else
		{
			//Load joystick
			gGameController = SDL_JoystickOpen( 0 );
			if( gGameController == NULL )
			{
				printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
			}
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

	//Load arrow texture
	gArrowTexture = LTexture_create();
	if( !LTexture_loadFromFile(gArrowTexture, gRenderer, "arrow.png" ) )
	{
		printf( "Failed to load arrow texture!\n" );
		success = false;
	}

	return success;
}

void cleanup()
{
	//Free loaded images
	LTexture_destroy(gArrowTexture);

	//Close game controller
	SDL_JoystickClose( gGameController );
	gGameController = NULL;

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

			//Normalized direction
			int xDir = 0;
			int yDir = 0;

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
					else if( e.type == SDL_JOYAXISMOTION )
					{
						//Motion on controller 0
						if( e.jaxis.which == 0 )
						{
							//X axis motion
							if( e.jaxis.axis == 0 )
							{
								//Left of dead zone
								if( e.jaxis.value < -JOYSTICK_DEAD_ZONE )
								{
									xDir = -1;
								}
								//Right of dead zone
								else if( e.jaxis.value > JOYSTICK_DEAD_ZONE )
								{
									xDir =  1;
								}
								else
								{
									xDir = 0;
								}
							}
							//Y axis motion
							else if( e.jaxis.axis == 1 )
							{
								//Below of dead zone
								if( e.jaxis.value < -JOYSTICK_DEAD_ZONE )
								{
									yDir = -1;
								}
								//Above of dead zone
								else if( e.jaxis.value > JOYSTICK_DEAD_ZONE )
								{
									yDir =  1;
								}
								else
								{
									yDir = 0;
								}
							}
						}
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Calculate angle
				double joystickAngle = atan2( (double)yDir, (double)xDir ) * ( 180.0 / M_PI );

				//Correct angle
				if( xDir == 0 && yDir == 0 )
				{
					joystickAngle = 0;
				}

				//Render joystick 8 way angle
				LTexture_render(gArrowTexture,
                     gRenderer,
                      ( SCREEN_WIDTH - LTexture_getWidth(gArrowTexture)) / 2,
                          ( SCREEN_HEIGHT - LTexture_getHeight(gArrowTexture) ) / 2,
                     NULL, joystickAngle, NULL, 0 );

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	cleanup();

	return 0;
}

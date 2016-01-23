/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, SDL_ttf, standard IO, strings, and string streams
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include "LTexture.h"

//Colour definitions in RGBA list format for SDL renderer
#define COLOUR_BLACK 0xFF, 0xFF, 0xFF, 0xFF

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Number of data integers
#define TOTAL_DATA  10

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
TTF_Font *gFont = NULL;

//Scene textures
LTexture* gPromptTextTexture;
LTexture* gDataTextures[ TOTAL_DATA ];

//Data points
Sint32 gData[ TOTAL_DATA ];

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
	//Text rendering color
	SDL_Color textColor = { 0, 0, 0, 0xFF };
	SDL_Color highlightColor = { 0xFF, 0, 0, 0xFF };

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
		//Render the prompt
		gPromptTextTexture = LTexture_create();
		if( !LTexture_loadFromRenderedText(gPromptTextTexture, gRenderer, "Enter Data:", gFont, textColor ) )
		{
			printf( "Failed to render prompt text!\n" );
			success = false;
		}
	}

	//Open file for reading in binary
	SDL_RWops* file = SDL_RWFromFile( "nums.bin", "r+b" );

	//File does not exist
	if( file == NULL )
	{
		printf( "Warning: Unable to open file! SDL Error: %s\n", SDL_GetError() );

		//Create file for writing
		file = SDL_RWFromFile( "nums.bin", "w+b" );
		if( file != NULL )
		{
			printf( "New file created!\n" );

			//Initialize data
			int i;
			for( i = 0; i < TOTAL_DATA; ++i )
			{
				gData[ i ] = 0;
				SDL_RWwrite( file, &gData[ i ], sizeof(Sint32), 1 );
			}

			//Close file handler
			SDL_RWclose( file );
		}
		else
		{
			printf( "Error: Unable to create file! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
	}
	//File exists
	else
	{
		//Load data
		printf( "Reading file...!\n" );
		int i;
		for( i = 0; i < TOTAL_DATA; ++i )
		{
			SDL_RWread( file, &gData[ i ], sizeof(Sint32), 1 );
		}

		//Close file handler
		SDL_RWclose( file );
	}

	//Initialize data textures
	int buffSize = 5;
	char dataText[buffSize];
	snprintf(dataText, buffSize, "%ld", gData[0]);
	gDataTextures[0] = LTexture_create();
	LTexture_loadFromRenderedText(gDataTextures[ 0 ], gRenderer, dataText, gFont, highlightColor );
	int i;
	for( i = 1; i < TOTAL_DATA; ++i )
	{
	    snprintf(dataText, buffSize, "%ld", gData[i]);
	    gDataTextures[i] = LTexture_create();
		LTexture_loadFromRenderedText(gDataTextures[ i ], gRenderer, dataText , gFont, textColor );
	}

	return success;
}

void close()
{
	//Open data for writing
	SDL_RWops* file = SDL_RWFromFile( "nums.bin", "w+b" );
	if( file != NULL )
	{
		//Save data
		int i;
		for( i = 0; i < TOTAL_DATA; ++i )
		{
			SDL_RWwrite( file, &gData[ i ], sizeof(Sint32), 1 );
		}

		//Close file handler
		SDL_RWclose( file );
	}
	else
	{
		printf( "Error: Unable to save file! %s\n", SDL_GetError() );
	}

	//Free loaded images
	LTexture_destroy(gPromptTextTexture);
	int i;
	for( i = 0; i < TOTAL_DATA; ++i )
	{
		LTexture_destroy(gDataTextures[ i ]);
	}

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

			//Text rendering color
			SDL_Color textColor = { 0, 0, 0, 0xFF };
			SDL_Color highlightColor = { 0xFF, 0, 0, 0xFF };

			//Current input point
			int currentData = 0;

			//text buffer
			int buffSize = 5;
			char dataText[buffSize];

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
					else if( e.type == SDL_KEYDOWN )
					{
						switch( e.key.keysym.sym )
						{
							//Previous data entry
							case SDLK_UP:
							//Rerender previous entry input point
							snprintf(dataText, buffSize, "%ld", gData[currentData]);
							LTexture_loadFromRenderedText(gDataTextures[ currentData ], gRenderer, dataText , gFont, textColor );
							--currentData;
							if( currentData < 0 )
							{
								currentData = TOTAL_DATA - 1;
							}

							//Rerender current entry input point
							snprintf(dataText, buffSize, "%ld", gData[currentData]);
							LTexture_loadFromRenderedText(gDataTextures[ currentData ], gRenderer, dataText , gFont, highlightColor );
							break;

							//Next data entry
							case SDLK_DOWN:
							//Rerender previous entry input point
							snprintf(dataText, buffSize, "%ld", gData[currentData]);
							LTexture_loadFromRenderedText(gDataTextures[ currentData ], gRenderer, dataText , gFont, textColor );
							++currentData;
							if( currentData == TOTAL_DATA )
							{
								currentData = 0;
							}

							//Rerender current entry input point
							snprintf(dataText, buffSize, "%ld", gData[currentData]);
							LTexture_loadFromRenderedText(gDataTextures[ currentData ], gRenderer, dataText , gFont, highlightColor );
							break;

							//Decrement input point
							case SDLK_LEFT:
							--gData[ currentData ];
							snprintf(dataText, buffSize, "%ld", gData[currentData]);
							LTexture_loadFromRenderedText(gDataTextures[ currentData ], gRenderer, dataText , gFont, highlightColor );
							break;

							//Increment input point
							case SDLK_RIGHT:
							++gData[ currentData ];
							snprintf(dataText, buffSize, "%ld", gData[currentData]);
							LTexture_loadFromRenderedText(gDataTextures[ currentData ], gRenderer, dataText , gFont, highlightColor );
							break;
						}
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, COLOUR_BLACK );
				SDL_RenderClear( gRenderer );

				//Render text textures
				LTexture_render(gPromptTextTexture, gRenderer, ( SCREEN_WIDTH - LTexture_getWidth(gPromptTextTexture) ) / 2, 0, NULL, 0, NULL, 0 );
				int i;
				for( i = 0; i < TOTAL_DATA; ++i )
				{
					LTexture_render(gDataTextures[ i ], gRenderer, ( SCREEN_WIDTH - LTexture_getWidth(gDataTextures[ i ]) ) / 2, LTexture_getHeight(gPromptTextTexture) + LTexture_getHeight(gDataTextures[ 0 ]) * i, NULL, 0, NULL, 0 );
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

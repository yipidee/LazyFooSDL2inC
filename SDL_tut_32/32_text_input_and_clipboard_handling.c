/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, SDL_ttf, standard IO, strings, and string streams
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string.h>
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

//Globally used font
TTF_Font *gFont = NULL;

//Scene textures
LTexture* gPromptTextTexture;
LTexture* gInputTextTexture;

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
		//Render the prompt
		gPromptTextTexture = LTexture_create();
		SDL_Color textColor = { 0, 0, 0, 0xFF };
		if( !LTexture_loadFromRenderedText(gPromptTextTexture, gRenderer, "Enter Text:", gFont, textColor ) )
		{
			printf( "Failed to render prompt text!\n" );
			success = false;
		}
	}

	return success;
}

void close()
{
	//Free loaded images
	LTexture_destroy(gPromptTextTexture);
	LTexture_destroy(gInputTextTexture);

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
			SDL_Color textColor = { 0, 0, 0, 0xFF };

			//The current input text.
			//std::string inputText = "Some Text";
			int buffSize = 45;
			char inputText[buffSize];
			snprintf(inputText, buffSize, "Some Text");

			gInputTextTexture = LTexture_create();
			LTexture_loadFromRenderedText(gInputTextTexture, gRenderer, inputText, gFont, textColor );

			//Enable text input
			SDL_StartTextInput();

			//While application is running
			while( !quit )
			{
				//The rerender text flag
				bool renderText = false;

				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					//Special key input
					else if( e.type == SDL_KEYDOWN )
					{
						//Handle backspace
						if( e.key.keysym.sym == SDLK_BACKSPACE && strlen(inputText) > 0 )
						{
							//lop off character
							int length = strlen(inputText);
							strcpy(&inputText[length-1], "\0");
							renderText = true;
						}
						//Handle copy
						else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
						{
							SDL_SetClipboardText( inputText );
						}
						//Handle paste
						else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
						{
							if(strlen(SDL_GetClipboardText()) < buffSize)
                            {
                                strcpy(inputText, SDL_GetClipboardText());
                                renderText = true;
                            }
						}
					}
					//Special text input event
					else if( e.type == SDL_TEXTINPUT )
					{
						//Not copy or pasting
						if( !( ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' ) && ( e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) && SDL_GetModState() & KMOD_CTRL ) )
						{
							//Append character
							if(strlen(inputText) < buffSize)
                            {
                                strcat(inputText, e.text.text);
                                renderText = true;
                            }
                            else
                            {
                                printf("string buff full\n");
                            }
						}
					}
				}

				//Rerender text if needed
				if( renderText )
				{
					//Text is not empty
					if( strncmp(inputText, "", buffSize ))
					{
						//Render new text
						LTexture_loadFromRenderedText(gInputTextTexture, gRenderer, &inputText, gFont, textColor );
					}
					//Text is empty
					else
					{
						//Render space texture
						LTexture_loadFromRenderedText(gInputTextTexture, gRenderer, " ", gFont, textColor );
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, COLOUR_BLACK );
				SDL_RenderClear( gRenderer );

				//Render text textures
				LTexture_render(gPromptTextTexture, gRenderer, ( SCREEN_WIDTH - LTexture_getWidth(gPromptTextTexture)) / 2, 0, NULL, 0, NULL, 0  );
				LTexture_render(gInputTextTexture, gRenderer,  ( SCREEN_WIDTH - LTexture_getWidth(gInputTextTexture)) / 2, LTexture_getHeight(gPromptTextTexture), NULL, 0, NULL, 0 );

				//Update screen
				SDL_RenderPresent( gRenderer );
			}

			//Disable text input
			SDL_StopTextInput();
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

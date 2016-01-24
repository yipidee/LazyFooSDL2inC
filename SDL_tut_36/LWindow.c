#include <stdlib.h>
#include <stdio.h>
#include "LWindow.h"

//initialiser
LWindow* LWindow_create()
{
	//Initialize non-existant window
	LWindow* win = malloc(sizeof(LWindow));

	win->mWindow = NULL;
	win->mRenderer = NULL;
	win->mMouseFocus = false;
	win->mKeyboardFocus = false;
	win->mFullScreen = false;
	win->mMinimized = false;
	win->mShown = false;
	win->mWindowID = -1;
	win->mWidth = 0;
	win->mHeight = 0;

	return win;
}

//Creates window
bool LWindow_init(LWindow* win, int screenW, int screenH)
{
	//Create window
	win->mWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenW, screenH, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
	if( win->mWindow != NULL )
	{
		win->mMouseFocus = true;
		win->mKeyboardFocus = true;
		win->mWidth = screenW;
		win->mHeight = screenH;

		//Create renderer for window
		win->mRenderer = SDL_CreateRenderer( win->mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
		if( win->mRenderer == NULL )
		{
			printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
			SDL_DestroyWindow( win->mWindow );
			win->mWindow = NULL;
		}
		else
		{
			//Initialize renderer color
			SDL_SetRenderDrawColor( win->mRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

			//Grab window identifier
			win->mWindowID = SDL_GetWindowID( win->mWindow );

			//Flag as opened
			win->mShown = true;
		}
	}
	else
	{
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
	}

	return win->mWindow != NULL && win->mRenderer != NULL;
}

//Handles window events
void LWindow_handleEvent( LWindow* win, SDL_Event* event )
{
	//If an event was detected for this window
	SDL_Event e = *event;
	if( e.type == SDL_WINDOWEVENT && e.window.windowID == win->mWindowID )
	{
		//Caption update flag
		bool updateCaption = false;

		switch( e.window.event )
		{
			//Window appeared
			case SDL_WINDOWEVENT_SHOWN:
			win->mShown = true;
			break;

			//Window disappeared
			case SDL_WINDOWEVENT_HIDDEN:
			win->mShown = false;
			break;

			//Get new dimensions and repaint
			case SDL_WINDOWEVENT_SIZE_CHANGED:
			win->mWidth = e.window.data1;
			win->mHeight = e.window.data2;
			SDL_RenderPresent( win->mRenderer );
			break;

			//Repaint on expose
			case SDL_WINDOWEVENT_EXPOSED:
			SDL_RenderPresent( win->mRenderer );
			break;

			//Mouse enter
			case SDL_WINDOWEVENT_ENTER:
			win->mMouseFocus = true;
			updateCaption = true;
			break;

			//Mouse exit
			case SDL_WINDOWEVENT_LEAVE:
			win->mMouseFocus = false;
			updateCaption = true;
			break;

			//Keyboard focus gained
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			win->mKeyboardFocus = true;
			updateCaption = true;
			break;

			//Keyboard focus lost
			case SDL_WINDOWEVENT_FOCUS_LOST:
			win->mKeyboardFocus = false;
			updateCaption = true;
			break;

			//Window minimized
			case SDL_WINDOWEVENT_MINIMIZED:
            win->mMinimized = true;
            break;

			//Window maxized
			case SDL_WINDOWEVENT_MAXIMIZED:
			win->mMinimized = false;
            break;

			//Window restored
			case SDL_WINDOWEVENT_RESTORED:
			win->mMinimized = false;
            break;

			//Hide on close
			case SDL_WINDOWEVENT_CLOSE:
			SDL_HideWindow( win->mWindow );
			break;
		}

		//Update window caption with new data
		if( updateCaption )
		{
			int capLen = 100;
			char caption[capLen];
			snprintf(caption, capLen, "SDL Tutorial - ID: %i, MouseFocus: %s, KeyboardFocus: %s", win->mWindowID, win->mMouseFocus ? "On" : "Off", win->mKeyboardFocus ? "On" : "Off" );
			SDL_SetWindowTitle( win->mWindow, caption );
		}
	}
}

//Focuses on window
void LWindow_focus(LWindow* win)
{
	//Restore window if needed
	if( !win->mShown )
	{
		SDL_ShowWindow( win->mWindow );
	}

	//Move window forward
	SDL_RaiseWindow( win->mWindow );
}

//Shows windows contents
void LWindow_render(LWindow* win)
{
	if( !win->mMinimized )
	{
		//Clear screen
		SDL_SetRenderDrawColor( win->mRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( win->mRenderer );

		//Update screen
		SDL_RenderPresent( win->mRenderer );
	}
}

//Deallocates internals
void LWindow_free(LWindow* win)
{
	if( win->mWindow != NULL )
	{
		SDL_DestroyWindow( win->mWindow );
	}

	win->mMouseFocus = false;
	win->mKeyboardFocus = false;
	win->mWidth = 0;
	win->mHeight = 0;
}

//Window dimensions
int LWindow_getWidth(LWindow* win)
{
    return win->mWidth;
}

int LWindow_getHeight(LWindow* win)
{
    return win->mHeight;
}

//Window focii
bool LWindow_hasMouseFocus(LWindow* win)
{
    return win->mMouseFocus;
}

bool LWindow_hasKeyboardFocus(LWindow* win)
{
    return win->mKeyboardFocus;
}

bool LWindow_isMinimized(LWindow* win)
{
    return win->mMinimized;
}

bool LWindow_isShown(LWindow* win)
{
    return win->mShown;
}

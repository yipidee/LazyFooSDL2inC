#include <stdlib.h>
#include <stdio.h>
#include "LWindow.h"

//initialiser
LWindow* LWindow_create()
{
	//Initialize non-existant window
	LWindow* win = malloc(sizeof(LWindow));

	win->mWindow = NULL;
	win->mMouseFocus = false;
	win->mKeyboardFocus = false;
	win->mFullScreen = false;
	win->mMinimized = false;
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
	}

	return win->mWindow != NULL;
}

//Creates renderer from internal window
SDL_Renderer* LWindow_createRenderer(LWindow* win)
{
	return SDL_CreateRenderer( win->mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
}

//Handles window events
void LWindow_handleEvent( LWindow* win, SDL_Renderer* renderer, SDL_Event* event )
{
    SDL_Event e = *event;
	//Window event occured
	if( e.type == SDL_WINDOWEVENT )
	{
		//Caption update flag
		bool updateCaption = false;

		switch( e.window.event )
		{
			//Get new dimensions and repaint on window size change
			case SDL_WINDOWEVENT_SIZE_CHANGED:
			win->mWidth = e.window.data1;
			win->mHeight = e.window.data2;
			SDL_RenderPresent( renderer );
			break;

			//Repaint on exposure
			case SDL_WINDOWEVENT_EXPOSED:
			SDL_RenderPresent( renderer );
			break;

			//Mouse entered window
			case SDL_WINDOWEVENT_ENTER:
			win->mMouseFocus = true;
			updateCaption = true;
			break;

			//Mouse left window
			case SDL_WINDOWEVENT_LEAVE:
			win->mMouseFocus = false;
			updateCaption = true;
			break;

			//Window has keyboard focus
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			win->mKeyboardFocus = true;
			updateCaption = true;
			break;

			//Window lost keyboard focus
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
		}

		//Update window caption with new data
		if( updateCaption )
		{
			int capLen = 50;
			char caption[capLen];

			snprintf(caption, capLen, "SDL Tutorial - MouseFocus: %s, KeyboardFocus: %s, ", win->mMouseFocus ? "On" : "Off", win->mKeyboardFocus ? "On" : "Off");
			SDL_SetWindowTitle( win->mWindow, caption );
		}
	}
	//Enter exit full screen on return key
	else if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN )
	{
		if( win->mFullScreen )
		{
			SDL_SetWindowFullscreen( win->mWindow, SDL_FALSE );
			win->mFullScreen = false;
		}
		else
		{
			SDL_SetWindowFullscreen( win->mWindow, SDL_TRUE );
			win->mFullScreen = true;
			win->mMinimized = false;
		}
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
	free(win);
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

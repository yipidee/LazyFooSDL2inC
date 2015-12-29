#include <stdbool.h>
#include <stdlib.h>
#include "LButton.h"

//Initializes internal variables
LButton* LButton_create(LTexture* spriteSheetTexture, SDL_Rect* clipRects, int W, int H)
{
    LButton* b = malloc(sizeof(LButton));
	b->mPosition.x = 0;
	b->mPosition.y = 0;
	b->gButtonSpriteSheetTexture = spriteSheetTexture;
	b->gSpriteClips = clipRects;
	b->bWidth = W;
	b->bHeight = H;

	b->mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
	return b;
}

//Sets top left position
void LButton_setPosition(LButton* b, int x, int y )
{
    b->mPosition.x = x;
    b->mPosition.y = y;
}

//Handles mouse event
void LButton_handleEvent( LButton* b, SDL_Event* e )
{
	//If mouse event happened
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if( x < b->mPosition.x )
		{
			inside = false;
		}
		//Mouse is right of the button
		else if( x > b->mPosition.x + b->bWidth )
		{
			inside = false;
		}
		//Mouse above the button
		else if( y < b->mPosition.y )
		{
			inside = false;
		}
		//Mouse below the button
		else if( y > b->mPosition.y + b->bHeight )
		{
			inside = false;
		}

		//Mouse is outside button
		if( !inside )
		{
			b->mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch( e->type )
			{
				case SDL_MOUSEMOTION:
				b->mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;

				case SDL_MOUSEBUTTONDOWN:
				b->mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;

				case SDL_MOUSEBUTTONUP:
				b->mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				break;
			}
		}
	}
}

//Shows button sprite
void LButton_render(LButton* b, SDL_Renderer* renderer)
{
	//Show current button sprite
	LTexture_render( b->gButtonSpriteSheetTexture, renderer, b->mPosition.x, b->mPosition.y, &b->gSpriteClips[ b->mCurrentSprite ], 0, NULL, 0 );
}

//Free memory associated with button
void LButton_destroy(LButton* b)
{
    if(b != NULL)free(b);
}

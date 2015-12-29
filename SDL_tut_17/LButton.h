// A C implementation of Lazy Foo's LButton C++ class
#ifndef LBUTTON_H
#define LBUTTON_H

#include <SDL.h>
#include "LTexture.h"

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

typedef struct
{
    //Top left position
    SDL_Point mPosition;
    //Currently used global sprite
    enum LButtonSprite mCurrentSprite;
    //LTexture holding sprite sheet
    LTexture* gButtonSpriteSheetTexture;
    //Rect array for clipping sprite sheet
    SDL_Rect* gSpriteClips;
    //Button width and height
    int bWidth, bHeight;
} LButton;

//Initializes internal variables
LButton* LButton_create(LTexture* spriteSheetTexture, SDL_Rect* clipRects, int W, int H);

//Sets top left position
void LButton_setPosition( LButton* b, int x, int y );

//Handles mouse event
void LButton_handleEvent( LButton* b, SDL_Event* e );

//Shows button sprite
void LButton_render(LButton* b, SDL_Renderer* renderer);

//Free memory associated with button
void LButton_destroy(LButton* b);
#endif  //LButton_H

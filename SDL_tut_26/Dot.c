#include <stdlib.h>
#include "Dot.h"

//The dimensions of the dot
static const int DOT_WIDTH = 20;
static const int DOT_HEIGHT = 20;

//Maximum axis velocity of the dot
static const int DOT_VEL = 10;

//Initializes the variables
Dot Dot_create(const LTexture* lt, const int screenW, const int screenH)
{
    //Initialize the offsets
    Dot d;

    d.mPosX = 0;
    d.mPosY = 0;

    //Initialize the velocity
    d.mVelX = 0;
    d.mVelY = 0;

    d.screenH=screenH;
    d.screenW=screenW;

    d.mDotTexture = lt;

    return d;
}

//Takes key presses and adjusts the dot's velocity
void Dot_handleEvent(Dot* dot, SDL_Event* ep )
{
    //If a key was pressed
    SDL_Event e = *ep;
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: dot->mVelY -= DOT_VEL; break;
            case SDLK_DOWN: dot->mVelY += DOT_VEL; break;
            case SDLK_LEFT: dot->mVelX -= DOT_VEL; break;
            case SDLK_RIGHT: dot->mVelX += DOT_VEL; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: dot->mVelY += DOT_VEL; break;
            case SDLK_DOWN: dot->mVelY -= DOT_VEL; break;
            case SDLK_LEFT: dot->mVelX += DOT_VEL; break;
            case SDLK_RIGHT: dot->mVelX -= DOT_VEL; break;
        }
    }
}

//Moves the dot
void Dot_move(Dot* dot)
{
    //Move the dot left or right
    dot->mPosX += dot->mVelX;

    //If the dot went too far to the left or right
    if( ( dot->mPosX < 0 ) || ( dot->mPosX + DOT_WIDTH > dot->screenW ) )
    {
        //Move back
        dot->mPosX -= dot->mVelX;
    }

    //Move the dot up or down
    dot->mPosY += dot->mVelY;

    //If the dot went too far up or down
    if( ( dot->mPosY < 0 ) || ( dot->mPosY + DOT_HEIGHT > dot->screenH ) )
    {
        //Move back
        dot->mPosY -= dot->mVelY;
    }
}

//Shows the dot on the screen
void Dot_render(const Dot dot, const SDL_Renderer* renderer)
{
    LTexture_render(dot.mDotTexture, renderer, dot.mPosX, dot.mPosY, NULL, 0, NULL, 0 );
}

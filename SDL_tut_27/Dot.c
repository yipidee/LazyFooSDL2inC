#include <stdlib.h>
#include "Dot.h"

//The dimensions of the dot
static const int DOT_WIDTH = 20;
static const int DOT_HEIGHT = 20;

//Maximum axis velocity of the dot
static const int DOT_VEL = 10;

//local function declarations
//Box collision detector
bool checkCollision( SDL_Rect a, SDL_Rect b );

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

    //Collision rect
    d.mCollider.w = DOT_WIDTH;
    d.mCollider.h = DOT_HEIGHT;

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
void Dot_move(Dot* dot, SDL_Rect wall)
{
    //Move the dot left or right
    dot->mPosX += dot->mVelX;
    dot->mCollider.x=dot->mPosX;

    //If the dot went too far to the left or right
    if( ( dot->mPosX < 0 ) || ( dot->mPosX + DOT_WIDTH > dot->screenW ) || checkCollision( dot->mCollider, wall ))
    {
        //Move back
        dot->mPosX -= dot->mVelX;
        dot->mCollider.x=dot->mPosX;
    }

    //Move the dot up or down
    dot->mPosY += dot->mVelY;
    dot->mCollider.y=dot->mPosY;

    //If the dot collided or went too far up or down
    if( ( ( dot->mPosY < 0 ) || ( dot->mPosY + DOT_HEIGHT > dot->screenH ) || checkCollision( dot->mCollider, wall ) ) )
    {
        //Move back
        dot->mPosY -= dot->mVelY;
		dot->mCollider.y = dot->mPosY;
    }
}

//Shows the dot on the screen
void Dot_render(const Dot dot, const SDL_Renderer* renderer)
{
    LTexture_render(dot.mDotTexture, renderer, dot.mPosX, dot.mPosY, NULL, 0, NULL, 0 );
}

bool checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

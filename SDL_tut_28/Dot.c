#include <stdlib.h>
#include "Dot.h"

//The dimensions of the dot
static const int DOT_WIDTH = 20;
static const int DOT_HEIGHT = 20;

//Maximum axis velocity of the dot
static const int DOT_VEL = 1;

//local function declarations
//Box collision detector
bool _checkCollision( const Dot a, const Dot b );
//Shift colliders to match dot position
void _shiftColliders(Dot* dot);

//Initializes the variables
Dot Dot_create(LTexture* lt, int screenW, int screenH)
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

    //set rects pointer to NULL
    d.mColliders = NULL;

    return d;
}

//Initialise dot's collider rectangles
void Dot_createColliders(Dot* dot, int nColliders, SDL_Rect* rects)
{
    if(dot->mColliders == NULL)
    {
        dot->nColliders = nColliders;
        dot->mColliders = malloc(sizeof(Dot) * nColliders);

        int i;
        for(i=0; i<nColliders; ++i)
        {
            *(dot->mColliders + i) = *(rects + i);
        }
        //set rect positions
        _shiftColliders(dot);
    }
}

void Dot_clearColliders(Dot* dot)
{
    if(dot->mColliders != NULL)
    {
        free(dot->mColliders);
    }
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
void Dot_move(Dot* moveDot, const Dot otherDot)
{
    //Move the dot left or right
    moveDot->mPosX += moveDot->mVelX;
    _shiftColliders(moveDot);

    //If the dot went too far to the left or right
    if( ( moveDot->mPosX < 0 ) || ( moveDot->mPosX + DOT_WIDTH > moveDot->screenW ) || _checkCollision( *moveDot, otherDot ))
    {
        //Move back
        moveDot->mPosX -= moveDot->mVelX;
        _shiftColliders(moveDot);
    }

    //Move the dot up or down
    moveDot->mPosY += moveDot->mVelY;
    _shiftColliders(moveDot);

    //If the dot collided or went too far up or down
    if( ( ( moveDot->mPosY < 0 ) || ( moveDot->mPosY + DOT_HEIGHT > moveDot->screenH ) || _checkCollision( *moveDot, otherDot ) ) )
    {
        //Move back
        moveDot->mPosY -= moveDot->mVelY;
		_shiftColliders(moveDot);
    }
}

//Shows the dot on the screen
void Dot_render( Dot dot, SDL_Renderer* renderer)
{
    LTexture_render(dot.mDotTexture, renderer, dot.mPosX, dot.mPosY, NULL, 0, NULL, 0 );
}

bool _checkCollision( const Dot a, const Dot b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Go through the A boxes
    SDL_Rect box;
    int Abox;
    for( Abox = 0; Abox < a.nColliders; Abox++ )
    {
        //Calculate the sides of rect A
        box = *(a.mColliders + Abox);
        leftA = box.x;
        rightA = leftA + box.w;
        topA = box.y;
        bottomA = topA + box.h;

        //Go through the B boxes
        int Bbox;
        for( Bbox = 0; Bbox < b.nColliders; Bbox++ )
        {
            //Calculate the sides of rect B
            box = *(b.mColliders + Bbox);
            leftB = box.x;
            rightB = leftB + box.w;
            topB = box.y;
            bottomB = topB + box.h;

            //If no sides from A are outside of B
            if( ( ( bottomA <= topB ) || ( topA >= bottomB ) || ( rightA <= leftB ) || ( leftA >= rightB ) ) == false )
            {
                //A collision is detected
                return true;
            }
        }
    }

    //If none of the sides from A are outside B
    return false;
}

void _shiftColliders(Dot* dot)
{
    //The row offset
    int r = 0;

    //Go through the dot's collision boxes
    SDL_Rect* rect;
    int c = 0;
    for( c = 0; c < dot->nColliders; ++c )
    {
        //Center the collision box
        rect = dot->mColliders + c;
        rect->x = dot->mPosX + ( DOT_WIDTH - rect->w ) / 2;

        //Set the collision box at its row offset
        rect->y = dot->mPosY + r;

        //Move the row offset down the height of the collision box
        r += rect->h;
    }
}

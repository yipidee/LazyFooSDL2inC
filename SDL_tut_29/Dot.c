#include <stdlib.h>
#include "Dot.h"

//The dimensions of the dot
const int DOT_WIDTH = 20;
const int DOT_HEIGHT = 20;

//Maximum axis velocity of the dot
static const int DOT_VEL = 1;

//local function declarations
//Box collision detector
bool _checkCollisionCC( const Circle a, const Circle b );
bool _checkCollisionCS( const Circle a, const SDL_Rect b );
//Shift colliders to match dot position
void _shiftColliders(Dot* dot);
//Calculates distance squared between two points
double _distanceSquared( int x1, int y1, int x2, int y2 );

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

    //set collision detection
    d.mCollider.r = DOT_WIDTH / 2;
    _shiftColliders(&d);

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
void Dot_move(Dot* dot, const SDL_Rect* square, const Circle* circle)
{
    //Move the dot left or right
    dot->mPosX += dot->mVelX;
	_shiftColliders(dot);

    //If the dot collided or went too far to the left or right
	if( ( dot->mPosX - dot->mCollider.r < 0 ) || ( dot->mPosX + dot->mCollider.r > dot->screenW ) || _checkCollisionCS( dot->mCollider, *square ) || _checkCollisionCC( dot->mCollider, *circle ) )
    {
        //Move back
        dot->mPosX -= dot->mVelX;
		_shiftColliders(dot);
    }

    //Move the dot up or down
    dot->mPosY += dot->mVelY;
	_shiftColliders(dot);

    //If the dot collided or went too far up or down
    if( ( dot->mPosY - dot->mCollider.r < 0 ) || ( dot->mPosY + dot->mCollider.r > dot->screenH ) || _checkCollisionCS( dot->mCollider, *square ) || _checkCollisionCC( dot->mCollider, *circle ) )
    {
        //Move back
        dot->mPosY -= dot->mVelY;
		_shiftColliders(dot);
    }
}

//Shows the dot on the screen
void Dot_render( Dot dot, SDL_Renderer* renderer)
{
    LTexture_render(dot.mDotTexture, renderer, dot.mPosX, dot.mPosY, NULL, 0, NULL, 0 );
}

bool _checkCollisionCC( const Circle a, const Circle b )
{
    //Calculate total radius squared
	int totalRadiusSquared = a.r + b.r;
	totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

    //If the distance between the centers of the circles is less than the sum of their radii
    if( _distanceSquared( a.x, a.y, b.x, b.y ) < totalRadiusSquared )
    {
        //The circles have collided
        return true;
    }

    //If not
    return false;
}

bool _checkCollisionCS( const Circle a, const SDL_Rect b )
{
    //Closest point on collision box
    int cX, cY;

    //Find closest x offset
    if( a.x < b.x )
    {
        cX = b.x;
    }
    else if( a.x > b.x + b.w )
    {
        cX = b.x + b.w;
    }
    else
    {
        cX = a.x;
    }

    //Find closest y offset
    if( a.y < b.y )
    {
        cY = b.y;
    }
    else if( a.y > b.y + b.h )
    {
        cY = b.y + b.h;
    }
    else
    {
        cY = a.y;
    }

    //If the closest point is inside the circle
    if( _distanceSquared( a.x, a.y, cX, cY ) < a.r * a.r )
    {
        //This box and the circle have collided
        return true;
    }

    //If the shapes have not collided
    return false;
}

void _shiftColliders(Dot* dot)
{
    //Align collider to center of dot
	dot->mCollider.x = dot->mPosX + DOT_WIDTH / 2;
	dot->mCollider.y = dot->mPosY + DOT_HEIGHT / 2;
}

double _distanceSquared( int x1, int y1, int x2, int y2 )
{
    int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	return deltaX*deltaX + deltaY*deltaY;
}

//Reset collider for whatever reason
void updateCollider(Dot* dot)
{
    _shiftColliders(dot);
}

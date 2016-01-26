#include <stdlib.h>
#include "Dot.h"

//The dimensions of the dot
const int DOT_WIDTH = 20;
const int DOT_HEIGHT = 20;

//Maximum axis velocity of the dot
#define DOT_VEL  10


//Initializes the variables
Dot Dot_create(LTexture* lt, LTexture** colours)
{
    Dot d;
    //Initialize the offsets
    d.mPosX = 0;
    d.mPosY = 0;

    //Initialize the velocity
    d.mVelX = 0;
    d.mVelY = 0;

    d.mDotTexture = lt;

    //Initialize particles
    int i;
    for( i = 0; i < TOTAL_PARTICLES; ++i )
    {
        d.particles[ i ] = Particle_create( d.mPosX, d.mPosY, colours );
    }

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
void Dot_render( Dot dot, SDL_Renderer* renderer, LTexture** colours, LTexture* gShimmerTexture)
{
    //Show the dot
	LTexture_render(dot.mDotTexture, renderer, dot.mPosX, dot.mPosY, NULL, 0, NULL, 0 );

	//Show particles on top of dot
	Dot_renderParticles(dot, renderer, colours, gShimmerTexture);
}

void Dot_renderParticles(Dot dot, SDL_Renderer* renderer, LTexture** colours, LTexture* gShimmerTexture)
{
	//Go through particles
	int i;
    for( i = 0; i < TOTAL_PARTICLES; ++i )
    {
        //Delete and replace dead particles
        if( Particle_isDead(dot.particles[ i ]) )
        {
            free(dot.particles[ i ]);
            dot.particles[ i ] = Particle_create( dot.mPosX, dot.mPosY, colours );
        }
    }

    //Show particles
    for( i = 0; i < TOTAL_PARTICLES; ++i )
    {
        Particle_render(dot.particles[ i ], renderer, gShimmerTexture);
    }
}

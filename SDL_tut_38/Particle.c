#include <stdlib.h>
#include "Particle.h"

//Initialize position and animation
Particle Particle_create( int x, int y, LTexture** colours )
{
    Particle p;

    //Set offsets
    p.mPosX = x - 5 + ( rand() % 25 );
    p.mPosY = y - 5 + ( rand() % 25 );

    //Initialize animation
    p.mFrame = rand() % 5;

    //Set type
    p.mTexture = colours[rand() % 3];

    return p;
}

//Shows the particle
void Particle_render(Particle* p, SDL_Renderer* renderer, LTexture* gShimmerTexture)
{
    //Show image
	LTexture_render(p->mTexture, renderer, p->mPosX, p->mPosY, NULL, 0, NULL, 0 );

    //Show shimmer
    if( p->mFrame % 2 == 0 )
    {
		LTexture_render(gShimmerTexture, renderer,  p->mPosX, p->mPosY, NULL, 0, NULL, 0 );
    }

    //Animate
    p->mFrame += 1;
}

//Checks if particle is dead
bool Particle_isDead(Particle p)
{
    return p.mFrame > 10;
}

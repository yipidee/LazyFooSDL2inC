#include <stdlib.h>
#include "Particle.h"

//Initialize position and animation
Particle* Particle_create( int x, int y, LTexture** colours )
{
    Particle* p = malloc(sizeof(Particle));
    //Set offsets
    p->mPosX = x - 5 + ( rand() % 25 );
    p->mPosY = y - 5 + ( rand() % 25 );

    //Initialize animation
    p->mFrame = rand() % 5;

    //Set type
    switch( rand() % 3 )
    {
        case 0: p->mTexture = colours[0]; break;
        case 1: p->mTexture = colours[1]; break;
        case 2: p->mTexture = colours[2]; break;
    }
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
    p->mFrame++;
}

//Checks if particle is dead
bool Particle_isDead(Particle* p)
{
    return p->mFrame > 10;
}


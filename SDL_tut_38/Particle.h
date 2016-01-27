#ifndef PARTICLE_H
#define PARTICLE_H

#ifdef __linux
    #include <SDL2/SDL.h>
#elif _WIN32
    #include <SDL.h>
#endif
#include "LTexture.h"

typedef struct Particle
{
    //Offsets
    int mPosX, mPosY;

    //Current frame of animation
    int mFrame;

    //Type of particle
    LTexture *mTexture;

} Particle;

//Initialize position and animation
Particle Particle_create( int x, int y, LTexture** colours );

//Shows the particle
void Particle_render(Particle p, SDL_Renderer* renderer, LTexture* gShimmerTexture);

//Checks if particle is dead
bool Particle_isDead(Particle p);

#endif // PARTICLE_H

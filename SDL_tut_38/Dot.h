#ifndef DOT_H
#define DOT_H

#ifdef __linux
    #include <SDL2/SDL.h>
#elif _WIN32
    #include <SDL.h>
#endif
#include "LTexture.h"
#include "Particle.h"

extern const int DOT_WIDTH;
extern const int DOT_HEIGHT;
#define TOTAL_PARTICLES 20

typedef struct Dot
{
    //The X and Y offsets of the dot
    int mPosX, mPosY;
    //The velocity of the dot
    int mVelX, mVelY;
    //Screen width, height
    int screenW, screenH;
    //Texture
    LTexture* mDotTexture;
    //The particles
    Particle* particles[ TOTAL_PARTICLES ];
} Dot;

//Initializes the variables
Dot Dot_create(LTexture* lt, LTexture** colours);

//Takes key presses and adjusts the dot's velocity
void Dot_handleEvent(Dot* dot, SDL_Event* ep );

//Moves the dot
void Dot_move(Dot* dot );

//Shows the dot on the screen
void Dot_render(Dot dot, SDL_Renderer* renderer, LTexture** colours, LTexture* gShimmerTexture);

//Shows the particles
void Dot_renderParticles(Dot dot, SDL_Renderer* renderer, LTexture** colours, LTexture* gShimmerTexture);

#endif // DOT_H

#include <SDL.h>
#include "LTexture.h"

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
} Dot;

//Initializes the variables
Dot* Dot_create(LTexture* lt, int screenW, int screenH);
void Dot_destroy(Dot* dot);

//Takes key presses and adjusts the dot's velocity
void Dot_handleEvent(Dot* dot, SDL_Event* ep );

//Moves the dot
void Dot_move(Dot* dot);

//Shows the dot on the screen
void Dot_render(Dot* dot, SDL_Renderer* renderer);

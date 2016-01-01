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
    //Dot's collision box
    SDL_Rect mCollider;
} Dot;

//Initializes the variables
Dot Dot_create(const LTexture* lt, const int screenW, const int screenH);

//Takes key presses and adjusts the dot's velocity
void Dot_handleEvent(Dot* dot, SDL_Event* ep );

//Moves the dot
void Dot_move(Dot* dot, SDL_Rect wall);

//Shows the dot on the screen
void Dot_render(const Dot dot, const SDL_Renderer* renderer);

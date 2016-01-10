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
    //number of collider Rects
    int nColliders;
    //Dot's collision box
    SDL_Rect* mColliders;

} Dot;

//Initializes the variables
Dot Dot_create(LTexture* lt, int screenW, int screenH);

//Initialise dot's collider rectangles
void Dot_createColliders(Dot* dot, int nColliders, SDL_Rect* rects);
void Dot_clearColliders(Dot* dot);

//Takes key presses and adjusts the dot's velocity
void Dot_handleEvent(Dot* dot, SDL_Event* ep );

//Moves the dot
void Dot_move(Dot* moveDot, const Dot otherDot );

//Shows the dot on the screen
void Dot_render(Dot dot, SDL_Renderer* renderer);

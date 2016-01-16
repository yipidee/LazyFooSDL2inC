#include <SDL.h>
#include "LTexture.h"

extern const int DOT_WIDTH;
extern const int DOT_HEIGHT;

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
Dot Dot_create(LTexture* lt, int screenW, int screenH);

//Takes key presses and adjusts the dot's velocity
void Dot_handleEvent(Dot* dot, SDL_Event* ep );

//Moves the dot
void Dot_move(Dot* moveDot );

//Shows the dot on the screen
void Dot_render(Dot dot, SDL_Renderer* renderer);

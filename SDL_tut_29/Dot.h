#include <SDL.h>
#include "LTexture.h"

extern const int DOT_WIDTH;
extern const int DOT_HEIGHT;

//A circle stucture
typedef struct Circle
{
	int x, y;
	int r;
} Circle;

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
    Circle mCollider;

} Dot;

//Initializes the variables
Dot Dot_create(LTexture* lt, int screenW, int screenH);

//Takes key presses and adjusts the dot's velocity
void Dot_handleEvent(Dot* dot, SDL_Event* ep );

//Moves the dot
void Dot_move(Dot* moveDot, const SDL_Rect* square, const Circle* circle );

//Shows the dot on the screen
void Dot_render(Dot dot, SDL_Renderer* renderer);

//Reset collider for whatever reason
void updateCollider(Dot* dot);

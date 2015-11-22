// A C implementation of Lazy Foo's LTexture C++ class

#ifndef LTEXTURE_H
#define LTEXTURE_H

#include <SDL.h>
#include <stdbool.h>

// A structure to hold the LTEXTURE variables
typedef struct LTexture
{
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
} LTexture;


LTexture LTexture_init();

void LTexture_destroy(LTexture* lt);

bool LTexture_loadFromFile(LTexture *lt, SDL_Renderer* renderer, char* path );

void LTexture_render( SDL_Renderer* renderer, LTexture* lt, int x, int y );

int LTexture_getWidth(LTexture *lt);

int LTexture_getHeight(LTexture *lt);

#endif // LTEXTURE_H

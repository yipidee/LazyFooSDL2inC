// A C implementation of Lazy Foo's LTexture C++ class

#ifndef LTEXTURE_H
#define LTEXTURE_H

#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL_image.h>

// A structure to hold the LTEXTURE variables
typedef struct LTexture
{
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
} LTexture;

// Functions that operate on LTextures
LTexture LTexture_create();

void LTexture_destroy(LTexture* lt);

bool LTexture_loadFromFile(LTexture *lt, SDL_Renderer* renderer, char* path );

void LTexture_render( LTexture* lt, SDL_Renderer* renderer, int x, int y, SDL_Rect* clip );

int LTexture_getWidth(LTexture *lt);

int LTexture_getHeight(LTexture *lt);

#endif // LTEXTURE_H

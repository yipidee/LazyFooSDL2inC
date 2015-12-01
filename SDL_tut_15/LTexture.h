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

// Creates and returns an initialised LTexture struct
LTexture* LTexture_create();

//Resets structure to NULL state
void LTexture_reset(LTexture* lt);

//Destroys Texture and frees memory
void LTexture_destroy(LTexture* lt);

//Populates struct members from specified image asset
bool LTexture_loadFromFile(LTexture *lt, SDL_Renderer* renderer, char* path );

//Performs colour modulation on texture
void LTexture_setColor( LTexture* lt, Uint8 red, Uint8 green, Uint8 blue );

//renders texture using specified renderer to stated position
void LTexture_render( LTexture* lt,             //ref to LTexure
                      SDL_Renderer* renderer,   //ref to renderer
                      int x,                    //x pos
                      int y,                    //y pos
                      SDL_Rect* clip,           //clip to render
                      double angle,             //angle of rotation
                      SDL_Point* center,        //point of rotation
                      SDL_RendererFlip flip     //render flip
                      );

//sets texture blend mode
void LTexture_setBlendMode( LTexture* lt, SDL_BlendMode blending );

//sets texture alpha value
void LTexture_setAlpha( LTexture* lt, Uint8 alpha );

//returns width of texture
int LTexture_getWidth(LTexture *lt);

//returns height of texture
int LTexture_getHeight(LTexture *lt);

#endif // LTEXTURE_H

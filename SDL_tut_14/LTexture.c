#include "LTexture.h"

LTexture LTexture_create()
{
    //create and initialize an LTexture struct
    LTexture lt;
    lt.mTexture = NULL;
	lt.mWidth = 0;
	lt.mHeight = 0;

	return lt;
}

void LTexture_destroy(LTexture* lt)
{
    if( lt->mTexture != NULL )
	{
		SDL_DestroyTexture( lt->mTexture );
		lt->mTexture = NULL;
		lt->mWidth = 0;
		lt->mHeight = 0;
	}
}

bool LTexture_loadFromFile(LTexture *lt, SDL_Renderer* renderer, char* path )
{
	//Get rid of preexisting texture
	LTexture_destroy(lt);

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			lt->mWidth = loadedSurface->w;
			lt->mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	lt->mTexture = newTexture;
	return lt->mTexture != NULL;
}

void LTexture_setColor( LTexture* lt, Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture
	SDL_SetTextureColorMod( lt->mTexture, red, green, blue );
}

void LTexture_render( LTexture* lt, SDL_Renderer* renderer, int x, int y, SDL_Rect* clip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, lt->mWidth, lt->mHeight };

	if(clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

	SDL_RenderCopy( renderer, lt->mTexture, clip, &renderQuad );
}

void LTexture_setBlendMode( LTexture* lt, SDL_BlendMode blending )
{
    SDL_SetTextureBlendMode( lt->mTexture, blending );
}

void LTexture_setAlpha( LTexture* lt, Uint8 alpha )
{
    SDL_SetTextureAlphaMod( lt->mTexture, alpha );
}

int LTexture_getWidth(LTexture *lt)
{
    return lt->mWidth;
}

int LTexture_getHeight(LTexture *lt)
{
    return lt->mHeight;
}

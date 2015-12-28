#include <stdlib.h>
#include "LTexture.h"

LTexture* LTexture_create()
{
    //create LTexture in RAM
    LTexture* lt = malloc(sizeof(LTexture));

    lt->mTexture = NULL;
	lt->mWidth = 0;
	lt->mHeight = 0;

	return lt;
}

void LTexture_reset(LTexture* lt)
{
    if( lt->mTexture != NULL )
	{
		SDL_DestroyTexture( lt->mTexture );
		lt->mTexture = NULL;
		lt->mWidth = 0;
		lt->mHeight = 0;
	}
}

void LTexture_destroy(LTexture* lt)
{
    LTexture_reset(lt);
    free(lt);
}

bool LTexture_loadFromFile(LTexture *lt, SDL_Renderer* renderer, char* path )
{
	//Get rid of preexisting texture
	LTexture_reset(lt);

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

bool LTexture_loadFromRenderedText( LTexture *lt, SDL_Renderer* renderer, char *textureText, TTF_Font *font, SDL_Color textColor )
{
	//Get rid of preexisting texture
	LTexture_reset(lt);

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped( font, textureText, textColor, 200 );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        lt->mTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
		if( lt->mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			lt->mWidth = textSurface->w;
			lt->mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}

	//Return success
	return lt->mTexture != NULL;
}

void LTexture_setColor( LTexture* lt, Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture
	SDL_SetTextureColorMod( lt->mTexture, red, green, blue );
}

void LTexture_render( LTexture* lt, SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, lt->mWidth, lt->mHeight };

	if(clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

	SDL_RenderCopyEx( renderer, lt->mTexture, clip, &renderQuad, angle, center, flip );
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

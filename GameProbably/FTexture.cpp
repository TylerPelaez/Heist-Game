#include "Ftexture.h"

FTexture::FTexture()
{
	m_Texture = NULL;
	m_Width = 0;
	m_Height = 0;
}

FTexture::~FTexture()
{
	free();
}

void FTexture::free()
{
	if ( m_Texture != NULL )
	{
		SDL_DestroyTexture( m_Texture );
		m_Texture = NULL;
		m_Height = 0;
		m_Width = 0;
	}
}

bool FTexture::loadFromFile( std::string path )
{
	free();

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if ( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0xFF, 0xFF, 0xFF ) );

		//Create texture from surface pixels 
		newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if ( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			m_Width = loadedSurface->w;
			m_Height = loadedSurface->h;
		}

		SDL_FreeSurface( loadedSurface );
	}

	m_Texture = newTexture;
	return m_Texture != NULL;
}

void FTexture::render( SDL_Rect* clip, SDL_Rect renderQuad, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	SDL_RenderCopyEx( gRenderer, m_Texture, clip, &renderQuad, angle, center, flip );
}

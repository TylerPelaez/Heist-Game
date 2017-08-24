#pragma once
#pragma once

#include "Globals.h"
#include <string>

class FTexture
{
	SDL_Texture* m_Texture;

	int m_Width;
	int m_Height;

	void free();


public:
	// Ctor
	FTexture();
	~FTexture();

	bool loadFromFile( std::string path );

	void render( SDL_Rect* clip, SDL_Rect renderQuad, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

	int getWidth() const { return m_Width; }
	int getHeight() const { return m_Height; }

};
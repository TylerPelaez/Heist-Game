#pragma once
#include "Globals.h"

class Collider
{
public:	
	SDL_Rect boundingBox;

	Collider()
	{
		boundingBox = { 0,0,0,0 };
	}

	bool checkCollision( const SDL_Rect &other ) const;
};
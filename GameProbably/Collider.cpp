#include "Collider.h"

bool Collider::checkCollision(const SDL_Rect &other) const
{

	if ( boundingBox.y + boundingBox.h <= other.y )
	{
		return false;
	}

	if ( boundingBox.y >= other.y + other.h )
	{
		return false;
	}

	if ( boundingBox.x + boundingBox.w <= other.x )
	{
		return false;
	}

	if ( boundingBox.x >= other.x + other.w )
	{
		return false;
	}

	return true;

}
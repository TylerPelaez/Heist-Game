#include "Entity.h"
#include <math.h>

void Entity::render()
{
	float camX = camera.x;
	float camY = camera.y;

	if ( sprite.getHeight() > 0 )
	{
		SDL_Rect renderQuad = { pos.X() - camX, pos.Y() - camY, sprite.getWidth(), sprite.getHeight() };
		sprite.render( NULL, renderQuad );
	}

	for ( std::list<Bullet>::iterator itr = bulletList.begin(); itr != bulletList.end(); ++itr )
	{
		SDL_Rect bulletRenderQuad = { (int)( itr->pos.X() - camX ), (int)( itr->pos.Y() - camY ), itr->sprite.getWidth(), itr->sprite.getHeight() };
		itr->sprite.render( NULL, bulletRenderQuad );
	}
}

void Entity::updateBulletList( float dt )
{
	for ( std::list<Bullet>::iterator itr = bulletList.begin(); itr != bulletList.end(); )
	{
		itr->update( dt );
		itr->deathTimer += dt;
		if ( itr->deathTimer >= itr->range || itr->willDestroy )
		{
			itr = bulletList.erase( itr );
		}
		else
		{
			itr++;
		}
	}
}

float Entity::getRadiansToTarget( Vector2 targetCenter )
{
	float relativeX = ( targetCenter.X() - (pos.X() + ( sprite.getWidth() / 2 ))); // Can definitely be simplified but the compiler's probably doing it so who cares
	float relativeY = ( (pos.Y() + ( sprite.getHeight() / 2 )) - targetCenter.Y());

	float tan = 0.0f;

	// All this extra work is for the edge cases - cases where values are 0 (right angles), as well as changing the angle based on the quadrant of the triangle

	bool rightAngle = false;
	if ( relativeX != 0 )
		tan = abs( relativeY ) / abs( relativeX );
	else
	{
		rightAngle = true;
	}

	float angle = 0.0f;
	if ( rightAngle )
	{
		if ( relativeY >= 0 )
			angle = 90.0f;
		else
			angle = 270.0f;
	}
	else
	{
		if ( tan == 0 )
		{
			if ( relativeX < 0 )
				angle = 180.0f;
			else
				angle = 0.0f;

			return  ( angle * ( (float)M_PI / 180.0f ) );
		}
		else
			angle = atan( tan ) * ( (float)180 / (float)M_PI );
	}

	if ( relativeX != 0.0f )
	{
		if ( relativeX < 0.0f && relativeY > 0.0f )
		{
			angle = 180.0f - angle;
		}
		else if ( relativeX < 0.0f && relativeY < 0.0f )
		{
			angle += 180.0f;
		}
		else if ( relativeX > 0.0f && relativeY < 0.0f )
		{
			angle = 360.0f - angle;
		}
	}


	float ret = ( angle * ( (float)M_PI / 180.0f ) );
	return ret;
}

void Entity::fireAt( Vector2 target )
{
	cooldownTimer = maxCooldown;

	Vector2 center( pos.X() + ( (float)sprite.getWidth() / 2.0f ), pos.Y() + ( (float)sprite.getHeight() / 2.0f ) );
	Bullet bullet( center, 0.0f, bulletDamage, team, bulletVelocity, this, 5.0f );
	bulletList.push_back( bullet );

	bulletList.back().setTexture( bulletSpritePath );

	Vector2 offset( ( (float)bulletList.back().sprite.getWidth() / 2.0f ), ( (float)bulletList.back().sprite.getHeight() / 2.0f ) );
	bulletList.back().pos = Vector2( bulletList.back().pos.X() - offset.X(), bulletList.back().pos.Y() - offset.Y() );
	
	bulletList.back().rads = getRadiansToTarget( target );
}
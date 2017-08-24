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
		itr->render();
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
	Bullet bullet( center, 0.0f, bulletDamage, team, bulletVelocity, this, map, 5.0f );
	bulletList.push_back( bullet );

	bulletList.back().setSprite( bulletSpritePath );

	Vector2 offset( ( (float)bulletList.back().getSprite()->getWidth() / 2.0f ), ( (float)bulletList.back().getSprite()->getHeight() / 2.0f ) );
	bulletList.back().setPos(Vector2( bulletList.back().getPos().X() - offset.X(), bulletList.back().getPos().Y() - offset.Y() ));
	
	bulletList.back().rads = getRadiansToTarget( target );
}

bool Entity::setSprite(std::string path)
{
	bool ret = sprite.loadFromFile(path);
	collider.boundingBox = { (int)pos.X(), (int)pos.Y(), sprite.getWidth(), sprite.getHeight() };
	return ret;
}

const FTexture* Entity::getSprite()
{
	return &sprite;
}

void Entity::setPos(Vector2 newPos)
{
	pos.setXY(newPos.X(), newPos.Y());
	collider.boundingBox.x = pos.X();
	collider.boundingBox.y = pos.Y();
}

Vector2 Entity::getPos()
{
	return pos;
}

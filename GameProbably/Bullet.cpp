#include "bullet.h"
#include "Entity.h"
#include "PolygonMap.h"
#include <cmath>
#include <iostream>

Bullet::Bullet( Vector2 _pos, float _rads, float _damage, int _team, float _velocity, Entity* _parent, PolygonMap* _map, float _range )
	: pos( _pos )
	, rads( _rads )
	, damage( _damage )
	, team( _team )
	, velocity( _velocity )
	, parent(_parent)
	, range(_range)
	, deathTimer(0.0f)
	, map(_map)
{}

void Bullet::setTexture( std::string path )
{
	sprite.loadFromFile( path );
	collider.boundingBox = { (int) pos.X(), (int)pos.Y(), sprite.getWidth(), sprite.getHeight() };
}


void Bullet::update( float dt )
{
	pos.X( pos.X() + ((float) cos( rads ) * velocity * dt) );
	pos.Y( pos.Y() - ((float) sin( rads ) * velocity * dt) );

	Vector2 xCheck(pos.X() + sprite.getWidth(), pos.Y());
	Vector2 yCheck(pos.X(), pos.Y() + sprite.getHeight());

	if (pos.X() < 0 || xCheck.X() > LEVEL_WIDTH || pos.Y() < 0 || yCheck.Y() > LEVEL_HEIGHT)
	{
		willDestroy = true;
		return;
	}
	if (map->polygons.size() > 1)
	{
		bool movedX = false;
		bool movedY = false;

		for (unsigned int i = 1; i < map->polygons.size(); i++)
		{
			float polyLeft = map->polygons[i].vertices[0].X();
			float polyRight = map->polygons[i].vertices[2].X();
			float polyTop = map->polygons[i].vertices[0].Y();
			float polyBottom = map->polygons[i].vertices[2].Y();

			SDL_Rect polyRect = { polyLeft, polyTop, polyRight - polyLeft, polyBottom - polyTop };

			if (collider.checkCollision(polyRect))
			{
				willDestroy = true;
				return;
			}
		}
	}


	if ( team == enemies )
	{
		SDL_Rect playerRect = { parent->pos.X(), parent->pos.Y(), parent->sprite.getWidth(), parent->sprite.getHeight() };
		collider.checkCollision(playerRect);
	}
	else if (team == players )
	{
		return;
	}
}
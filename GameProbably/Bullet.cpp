#include "bullet.h"
#include "Entity.h"
#include "PolygonMap.h"
#include <cmath>
#include <iostream>

Bullet::Bullet(Vector2 _pos, float _rads, float _damage, int _team, float _velocity, Entity* _parent, PolygonMap* _map, float _range)
	: pos(_pos)
	, rads(_rads)
	, damage(_damage)
	, team(_team)
	, velocity(_velocity)
	, parent(_parent)
	, range(_range)
	, deathTimer(0.0f)
	, map(_map)
{}

bool Bullet::setSprite(std::string path)
{
	bool ret = sprite.loadFromFile(path);
	collider.boundingBox = { (int)pos.X(), (int)pos.Y(), sprite.getWidth(), sprite.getHeight() };
	return ret;
}

const FTexture* Bullet::getSprite()
{
	return &sprite;
}


void Bullet::update(float dt)
{
	setPos(Vector2(pos.X() + ((float)cos(rads) * velocity * dt), pos.Y() - ((float)sin(rads) * velocity * dt)));
	
	if (checkWillDestroy())
	{
		return;
	}

	if (team == enemies)
	{
		SDL_Rect playerRect = { parent->getPos().X(), parent->getPos().Y(), parent->getSprite()->getWidth(), parent->getSprite()->getHeight() };
		collider.checkCollision(playerRect);
	}
	else if (team == players)
	{
		return; //TODO
	}
}

void Bullet::render()
{
	float camX = camera.x;
	float camY = camera.y;

	SDL_Rect renderQuad = { pos.X() - camX, pos.Y() - camY, sprite.getWidth(), sprite.getHeight() };
	sprite.render(NULL, renderQuad);
}

void Bullet::setPos(Vector2 newPos)
{
	pos.setXY(newPos.X(), newPos.Y());
	collider.boundingBox.x = pos.X();
	collider.boundingBox.y = pos.Y();
}

const Vector2 Bullet::getPos()
{
	return pos;
}

bool Bullet::checkWillDestroy()
{
	Vector2 xCheck(pos.X() + sprite.getWidth(), pos.Y());
	Vector2 yCheck(pos.X(), pos.Y() + sprite.getHeight());

	if (pos.X() < 0 || xCheck.X() > LEVEL_WIDTH || pos.Y() < 0 || yCheck.Y() > LEVEL_HEIGHT)
	{
		willDestroy = true;
		return true;
	}
	if (map->polygons.size() > 1)
	{
		for (unsigned int i = 1; i < map->polygons.size(); i++)
		{
			float polyLeft = map->polygons[i].vertices[0].X();
			float polyRight = map->polygons[i].vertices[2].X();
			float polyTop = map->polygons[i].vertices[0].Y();
			float polyBottom = map->polygons[i].vertices[2].Y();
			printf("left: %f right: %ftop: %fbottom:%f\n", polyLeft, polyRight, polyTop, polyBottom);
			printf("posX: %f posY: %f\n", pos.X(), pos.Y());

			SDL_Rect polyRect = { polyLeft, polyTop, polyRight - polyLeft, polyBottom - polyTop };

			if (collider.checkCollision(polyRect))
			{
				willDestroy = true;
				return true;
			}
		}
	}
	return false;
}

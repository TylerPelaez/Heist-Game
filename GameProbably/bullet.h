#pragma once
#include "FTexture.h"
#include "Vector2.h"
#include "Collider.h"
#include <string>

class Entity;
class PolygonMap;

class Bullet
{
public:

	Collider collider;
	float rads;
	float damage;
	int team;
	float velocity;
	Entity* parent;
	float range;
	float deathTimer;
	PolygonMap * map;
	bool willDestroy = false;

	bool setSprite( std::string path );
	const FTexture* getSprite();

	Bullet( Vector2 _pos, float _rads, float _damage, int _team, float _velocity, Entity* parent, PolygonMap* _map, float _range = 1.0f);

	void update( float dt );
	void render();

	void setPos(Vector2 newPos);
	const Vector2 getPos();

protected:
	FTexture sprite;
	
	Vector2 pos;

private:
	bool checkWillDestroy();
};
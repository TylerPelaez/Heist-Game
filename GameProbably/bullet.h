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

	FTexture sprite;
	Vector2 pos;
	float rads;
	float damage;
	int team;
	Collider collider;
	float velocity;
	Entity* parent;
	float range;
	float deathTimer;
	PolygonMap * map;
	bool willDestroy = false;

	void setTexture( std::string path );

	Bullet( Vector2 _pos, float _rads, float _damage, int _team, float _velocity, Entity* parent, PolygonMap* _map, float _range = 1.0f);

	void update( float dt );

};
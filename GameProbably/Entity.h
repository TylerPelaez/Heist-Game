#pragma once
#include "Vector2.h"
#include "Globals.h"
#include "PolygonMap.h"
#include "Collider.h"
#include "bullet.h"
#include <list>

class Entity
{
public:
	Vector2 pos;
	Vector2 velocity;
	FTexture sprite;
	int team;

	PolygonMap *map;
	
	Collider collider;
	float health;


	float maxCooldown;
	float cooldownTimer = 0.0f;
	float bulletDamage = 5.0f;
	float bulletVelocity;
	
	std::list<Bullet> bulletList;
	std::string bulletSpritePath;
	


	Entity()
		: pos( 0, 0 )
		, velocity( 0, 0 )
		, maxCooldown( 0.5f )
		, bulletVelocity( 700.0f )
		, health( 100.0f )
	{}

	virtual void render( );

	virtual void update( float dt )
	{
		updateBulletList( dt );
		move();
	}

protected:
	virtual void updateBulletList( float dt );

	virtual void fireAt( Vector2 target );

	float getRadiansToTarget( Vector2 target );

	virtual void move()
	{
		return;
	}
};
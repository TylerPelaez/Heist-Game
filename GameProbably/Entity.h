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
	
	Vector2 velocity;
	int team;

	PolygonMap *map;
	
	Collider collider;
	float health;


	float maxCooldown;
	float cooldownTimer = 0.0f;
	float bulletDamage = 5.0f;
	float bulletVelocity;

	float maxWalkSpeed = 5.0f;
	float acceleration = 2.0f;
	
	std::list<Bullet> bulletList;
	std::string bulletSpritePath;
	
	


	Entity(PolygonMap* _map = NULL)
		: pos( 0, 0 )
		, velocity( 0, 0 )
		, maxCooldown( 0.5f )
		, bulletVelocity( 700.0f )
		, health( 100.0f )
		, sprite()
		, map(_map)
	{}

	virtual void render( );

	virtual void update( float dt )
	{
		updateBulletList( dt );
		move();
	}

	bool setSprite(std::string path);
	const FTexture* getSprite();

	void setPos(Vector2 newPos);
	Vector2 getPos();

protected:

	Vector2 pos;
	FTexture sprite;

	virtual void updateBulletList( float dt );

	virtual void fireAt( Vector2 target );

	float getRadiansToTarget( Vector2 target );

	virtual void move()
	{
		return;
	}
	
};
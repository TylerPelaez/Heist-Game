#include "Enemy.h"



Enemy::Enemy(PolygonMap* _map)
	: Entity(_map)
{
	team = TEAM::ENEMIES;
	walking = false;
	maxWalkSpeed = maxWalkSpeed / 2.0f;
}

void Enemy::move()
{
	if (currentwalknode == -1 || map == nullptr)
	{
		return;
	}

	float tempwalktox = map->walkgraph.nodes[currentwalknode].pos.X();
	float tempwalktoy = map->walkgraph.nodes[currentwalknode].pos.Y();

	Vector2 b = Vector2( getPos().X(), getPos().Y() );
	Vector2 a = Vector2( tempwalktox, tempwalktoy );

	Vector2 c = Vector2::Subtract( a, b );

	if ( c.length() >= maxWalkSpeed )
	{
		c.normalize();
		c.X( c.X() * maxWalkSpeed );
		c.Y( c.Y() * maxWalkSpeed);
	}

	b = Vector2::Add( b, c );

	setPos( b );

	if ( walkpath.size() > 0 )
	{
		if ( tempwalktox == getPos().X() && tempwalktoy == getPos().Y() )
		{
			currentwalknode = walkpath[0];
			walkpath.erase( walkpath.begin() );
		}
	}
	if ( walktox == getPos().X() && walktoy == getPos().Y() )
	{
		walking = false;
	}
}
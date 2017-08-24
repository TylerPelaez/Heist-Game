#pragma once
#include "Vector2.h"
#include "FTexture.h"
#include "PolygonMap.h"
#include "Collider.h"
#include "bullet.h"
#include "Entity.h"
#include <list>
#include <string>

class Player : public Entity
{
public:
	FTexture mouseSprite;

	bool walking = false;
	float maxWalkSpeed = 5.0f;
	float acceleration = 2.0f;



	Player()
	{
		team = TEAM::players;
	}

	void render() override;

	void update( float dt ) override;

private:
	void move() override;
};


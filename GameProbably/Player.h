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
	bool walking = false;


	Player()
	{
		team = TEAM::PLAYERS;
	}

	void render() override;

	void update( float dt ) override;

	bool setMouseSprite(std::string mouseSpritePath);
	const FTexture* getMouseSprite();


private:
	FTexture mouseSprite;
	void move() override;
};


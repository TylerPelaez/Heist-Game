#pragma once
#ifndef _PATHFINDINGHANDLER_H
#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "PolygonMap.h"

class PathfindingHandler
{
public:
	Player* player;
	std::vector<Enemy*> enemies;

	std::vector<int> walkpath;
	PolygonMap polygonMap;
	int playerIndex;


	bool showlines = true;

	FTexture backgroundSprite;

	PathfindingHandler( Player* p);

	void init()
	{
		initializeWalkableArea(1);
	}

	void update( float dt, float camX, float camY );
	
	void render();

	void updatePlayerNode();
	void updateEnemyNodes();
	void assignPaths();
	void calculateEnemyPaths();

	

private:
	void initializeWalkableArea( int p );

};
#endif
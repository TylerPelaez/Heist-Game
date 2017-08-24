#pragma once
#ifndef _PATHFINDINGHANDLER_H
#include <vector>
#include "Player.h"
#include "PolygonMap.h"

class PathfindingHandler
{
public:
	std::vector<int> walkpath;
	PolygonMap polygonMap;

	bool showlines = true;

	Player* player;
	int currentwalknode = 0;
	float walktox;
	float walktoy;

	FTexture backgroundSprite;

	PathfindingHandler( Player* p );


	void init()
	{
		initializeWalkableArea(1);
	}

	void update( float dt, float camX, float camY );
	
	void render();

	

private:
	void walkEnemies();


	void initializeWalkableArea( int p );

};
#endif
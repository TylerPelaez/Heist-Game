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

	bool updateNeeded = true;

	std::vector<int> walkpath;
	PolygonMap polygonMap;
	int playerIndex;

	float pathResetMax = 0.0f; // Cached pathfinding. increasing will save path for longer. 0 is computing path every frame.
	float pathResetTimer = -1.0f; // begin at -1 to ensure path is calculated on first frame.


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
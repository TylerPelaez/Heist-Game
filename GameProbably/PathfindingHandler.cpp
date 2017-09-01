#include "PathfindingHandler.h"

PathfindingHandler::PathfindingHandler( Player* p)
	: polygonMap( SCREEN_WIDTH, SCREEN_HEIGHT, this )
{
	player = p;
	enemies = std::vector<Enemy*>();
}

void PathfindingHandler::update( float dt, float camX, float camY )
{
	pathResetTimer -= dt;
	if (pathResetTimer <= 0 || updateNeeded)
	{
		pathResetTimer = pathResetMax;
		updateNeeded = false;
		calculateEnemyPaths();
	}
		
	for (auto enemy : enemies)
	{
		enemy->update(dt);
		if (!enemy->walking)
			updateNeeded = true;
	}
}

void PathfindingHandler::updatePlayerNode()
{
	Vector2 playerLocation = Vector2(player->getPos());
	GraphNode startNode = GraphNode(Vector2(playerLocation.X(), playerLocation.Y()));

	playerIndex = polygonMap.walkgraph.nodes.size();
	polygonMap.walkgraph.addNode(startNode);
	//printf("%d\n", polygonMap.walkgraph.nodes.size());
	
	for (unsigned int c_index = 0; c_index < polygonMap.walkgraph.nodes.size() - 1; c_index++) // Now we add edges in LOS from Player
	{
		Vector2 c = Vector2(polygonMap.walkgraph.nodes[c_index].pos.X(), polygonMap.walkgraph.nodes[c_index].pos.Y());
		if (polygonMap.InLineOfSight(playerLocation, c))
		{
			polygonMap.walkgraph.addEdge(GraphEdge(playerIndex, c_index, polygonMap.Distance(playerLocation, c)));
			polygonMap.walkgraph.addEdge(GraphEdge(c_index, playerIndex, polygonMap.Distance(playerLocation, c)));
		}
	}

}

void PathfindingHandler::updateEnemyNodes()
{
	for (auto& enemy : enemies)
	{
		Vector2 enemyLocation = Vector2(enemy->getPos());
		GraphNode enemyNode = GraphNode(Vector2(enemyLocation.X(), enemyLocation.Y()));

		int enemyIndex = polygonMap.walkgraph.nodes.size();
		polygonMap.walkgraph.addNode(enemyNode);

		for (unsigned int c_index = 0; c_index < polygonMap.walkgraph.nodes.size() - 1; c_index++) // Now we add edges in LOS from Enemies
		{
			Vector2 c = Vector2(polygonMap.walkgraph.nodes[c_index].pos.X(), polygonMap.walkgraph.nodes[c_index].pos.Y());
			if (polygonMap.InLineOfSight(enemyLocation, c))
			{
				polygonMap.walkgraph.addEdge(GraphEdge(enemyIndex, c_index, polygonMap.Distance(enemyLocation, c)));
				polygonMap.walkgraph.addEdge(GraphEdge(c_index, enemyIndex, polygonMap.Distance(enemyLocation, c)));
			}
		}

	}
}

void PathfindingHandler::assignPaths()
{
	DijkstraAlgorithm dijkstra = DijkstraAlgorithm(polygonMap.walkgraph, playerIndex);

	for (unsigned int i = 0; i < enemies.size(); ++i)
	{
		Enemy* enemy = enemies[i];
		enemy->walkpath = dijkstra.getPath(playerIndex, polygonMap.mainwalkgraph.nodes.size() + 1 + i);  //necessarily the correct enemy for the location
		enemy->walkpath.erase(enemy->walkpath.begin());
		enemy->currentwalknode = enemy->walkpath[0];
		enemy->walking = true;
		
	}
}

void PathfindingHandler::calculateEnemyPaths()
{
	polygonMap.walkgraph = polygonMap.mainwalkgraph.clone();

	updatePlayerNode();
	updateEnemyNodes();
	
	assignPaths();
}

void PathfindingHandler::render()
{
	SDL_Rect renderQuad = {-camera.x, -camera.y, backgroundSprite.getWidth(),  backgroundSprite.getHeight() };
	backgroundSprite.render( NULL, renderQuad );
	polygonMap.render(camera.x, camera.y);

	polygonMap.walkgraph.render( 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, camera.x, camera.y );

	for (unsigned int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->render();
	}
}


void PathfindingHandler::initializeWalkableArea( int p )
{
	player->setPos( Vector2(1000.0f, 1000.0f) );

	enemies.push_back(new Enemy(&polygonMap));

	for (unsigned int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->setPos(Vector2(2000.0f, 1500.0f));
	}

	int polyId = 0;
	polygonMap = PolygonMap( SCREEN_WIDTH, SCREEN_HEIGHT, this );
	polygonMap.polygons.push_back (Polygon());
	if ( p == 1 )
	{
		polygonMap.polygons[polyId].addPoint( 0, 0 );
		polygonMap.polygons[polyId].addPoint( 3839, 0 );
		polygonMap.polygons[polyId].addPoint( 3839, 2159 );
		polygonMap.polygons[polyId].addPoint( 0, 2159 );
		polyId++;

		polygonMap.polygons.push_back( Polygon() );
		polygonMap.polygons[polyId].addPoint( 1400, 1400 );
		polygonMap.polygons[polyId].addPoint( 1600, 1400 );
		polygonMap.polygons[polyId].addPoint( 1600, 1600 );
		polygonMap.polygons[polyId].addPoint( 1400, 1600 );
	}

	polygonMap.CreateGraph();
}

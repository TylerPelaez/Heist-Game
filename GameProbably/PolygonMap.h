#pragma once
#include "Polygon.h"
#include "Graph.h"
#include "DijkstraAlgorithm.h"

class Enemy;
class PathfindingHandler;

class PolygonMap
{
public:
	std::vector<Vector2> vertices_concave;
	std::vector<class Polygon> polygons;
	PathfindingHandler* pfh;
	

	Graph mainwalkgraph;
	Graph walkgraph;

	float targetx;
	float targety;

	std::vector<int> calculatedPath;

	PolygonMap( float w, float h, PathfindingHandler* _pfh);
	PolygonMap( const PolygonMap &other );

	void CreateGraph();

	void render(float camX, float camY);

	float Distance( Vector2 v1, Vector2 v2 );

	bool LineSegmentsCross( Vector2 a, Vector2 b, Vector2 c, Vector2 d );

	bool IsVertexConcave( std::vector<Vector2> vertices, int vertex );

	bool InLineOfSight( Vector2 start, Vector2 end );

	void updatePlayerNode();
};
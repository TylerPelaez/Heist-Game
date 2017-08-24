#pragma once
#include "Polygon.h"
#include "Graph.h"
#include "AstarAlgorithm.h"


class PolygonMap
{
public:
	std::vector<Vector2> vertices_concave;
	std::vector<class Polygon> polygons;

	Graph mainwalkgraph;
	Graph walkgraph;

	float targetx;
	float targety;

	int startNodeIndex;
	int endNodeIndex;

	std::vector<int> calculatedPath;

	PolygonMap( float w, float h );
	PolygonMap( const PolygonMap &other );

	void CreateGraph();

	std::vector<int> calculatePath( Vector2 from, Vector2 to );

	void render(float camX, float camY);

	

private:
	float Distance( Vector2 v1, Vector2 v2 );

	bool LineSegmentsCross( Vector2 a, Vector2 b, Vector2 c, Vector2 d );

	bool IsVertexConcave( std::vector<Vector2> vertices, int vertex );

	bool InLineOfSight( Vector2 start, Vector2 end );
};
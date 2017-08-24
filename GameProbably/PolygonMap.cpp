#include "PolygonMap.h"

PolygonMap::PolygonMap( float w, float h )
{
	mainwalkgraph = Graph();
	calculatedPath = std::vector<int>();
	startNodeIndex = 0;
	endNodeIndex = 0;
	vertices_concave = std::vector<Vector2>();
	polygons = std::vector<class Polygon>();
}



void PolygonMap::CreateGraph()
{

	mainwalkgraph = Graph();
	bool first = true;
	vertices_concave = std::vector<Vector2>();
	for ( auto& polygon : polygons )
	{
		if (polygon.vertices.size() > 2 )
		{
			for ( unsigned int i = 0; i < polygon.vertices.size(); i++ )
			{
				if ( IsVertexConcave( polygon.vertices, i ) == first )
				{
					int index = vertices_concave.size();
					vertices_concave.push_back( polygon.vertices[i] );
					mainwalkgraph.addNode( GraphNode( Vector2( polygon.vertices[i].X(), polygon.vertices[i].Y() ) ) );
				}
			}
		}
		first = false;
	}
	for ( unsigned int c1_index = 0; c1_index < vertices_concave.size(); c1_index++ )
	{
		for ( unsigned int c2_index = 0; c2_index < vertices_concave.size(); c2_index++ )
		{
			Vector2 c1 = vertices_concave[c1_index];
			Vector2 c2 = vertices_concave[c2_index];
			if ( InLineOfSight( c1, c2 ) )
			{
				mainwalkgraph.addEdge( GraphEdge( c1_index, c2_index, Distance( c1, c2 ) ) );
			}
		}
	}
}

std::vector<int> PolygonMap::calculatePath( Vector2 from, Vector2 to )
{
	walkgraph = mainwalkgraph.clone();

	float mindistanceFrom = 100000;
	float mindistanceTo = 100000;

	startNodeIndex = walkgraph.nodes.size();
	if ( !polygons[0].pointInside( from ) )
	{
		from = polygons[0].getClosestPointONEdge( from );
	}
	if ( !polygons[0].pointInside( to ) )
	{
		to = polygons[0].getClosestPointONEdge( to );
	}


	if ( polygons.size() > 1 )
	{
		for ( unsigned int i = 1; i < polygons.size(); i++ )
		{
			if ( polygons[i].pointInside( to ) )
			{
				to = polygons[i].getClosestPointONEdge( to );
				break;
			}
		}
	}

	targetx = to.X();
	targety = to.Y();

	GraphNode startNode = GraphNode( Vector2( from.X(), from.Y() ) );
	Vector2 startNodeVector = Vector2( startNode.pos.X(), startNode.pos.Y() );

	walkgraph.addNode( startNode );
	for ( unsigned int c_index = 0; c_index < vertices_concave.size(); c_index++ )
	{
		Vector2 c = vertices_concave[c_index];
		if ( InLineOfSight( startNodeVector, c ) )
		{
			walkgraph.addEdge( GraphEdge( startNodeIndex, c_index, Distance( startNodeVector, c ) ) );
		}
	}


	endNodeIndex = walkgraph.nodes.size();

	GraphNode endNode = GraphNode( Vector2( to.X(), to.Y() ) );
	Vector2 endNodeVector = Vector2( endNode.pos.X(), endNode.pos.Y() );

	walkgraph.addNode( endNode );
	for ( unsigned int c_index = 0; c_index < vertices_concave.size(); c_index++ )
	{
		Vector2 c = vertices_concave[c_index];
		if ( InLineOfSight( endNodeVector, c ) )
		{
			walkgraph.addEdge( GraphEdge( c_index, endNodeIndex, Distance( endNodeVector, c ) ) );
		}
	}
	if ( InLineOfSight( startNodeVector, endNodeVector ) )
	{
		walkgraph.addEdge( GraphEdge( startNodeIndex, endNodeIndex, Distance( startNodeVector, endNodeVector ) ) );
	}

	AstarAlgorithm astar = AstarAlgorithm( walkgraph, startNodeIndex, endNodeIndex );
	calculatedPath = astar.getPath();

	return calculatedPath;

}

bool PolygonMap::IsVertexConcave( std::vector<Vector2> vertices, int vertex )
{
	Vector2 current = vertices[vertex];
	Vector2 next = vertices[( vertex + 1 ) % vertices.size()];
	Vector2 previous = vertices[vertex == 0 ? vertices.size() - 1 : vertex - 1];

	Vector2 left = Vector2::Subtract( current, previous );
	Vector2 right = Vector2::Subtract( next, current );

	float cross = ( left.X() * right.Y() ) - ( left.Y()  * right.X() );

	return cross < 0.0f;
}

bool PolygonMap::InLineOfSight( Vector2 start, Vector2 end )
{
	float epsilon = 0.5f;

	if ( !polygons[0].pointInside( start ) || !polygons[0].pointInside( end ) )
		return false;

	if ( Vector2::Subtract( start, end ).length() < epsilon )
		return true;

	bool inSight = true;

	for ( auto& polygon : polygons )
	{
		for ( unsigned int i = 0; i < polygon.vertices.size(); i++ )
		{
			Vector2 v1 = polygon.vertices[i];
			Vector2 v2 = polygon.vertices[( i + 1 ) % polygon.vertices.size()];
			if ( LineSegmentsCross( start, end, v1, v2 ) )
			{
				if ( polygon.distanceToSegment( start.X(), start.Y(), v1.X(), v1.Y(), v2.X(), v2.Y() ) > 0.5 && polygon.distanceToSegment( end.X(), end.Y(), v1.X(), v1.Y(), v2.X(), v2.Y() ) > 0.5 )
					return false;
			}
		}
	}


	Vector2 v = Vector2::Add( start, end );
	Vector2 v2 = Vector2( v.X() / 2, v.Y() / 2 );
	bool inside = polygons[0].pointInside( v2 );
	for ( unsigned int i = 1; i < polygons.size(); i++ )
	{
		if ( polygons[i].pointInside( v2, false ) )
			inside = false;
	}
	return inside;
}


float PolygonMap::Distance( Vector2 v1, Vector2 v2 )
{
	float dx = v1.X() - v2.X();
	float dy = v1.Y() - v2.Y();
	return sqrt( dx * dx + dy * dy );
}

bool PolygonMap::LineSegmentsCross( Vector2 a, Vector2 b, Vector2 c, Vector2 d )
{
	float denominator = ( ( b.X() - a.X() ) * ( d.Y() - c.Y() ) ) - ( ( b.Y() - a.Y() ) * ( d.X() - c.X() ) );
	if ( denominator == 0 )
		return false;

	float numerator1 = ( ( a.Y() - c.Y() ) * ( d.X() - c.X() ) ) - ( ( a.X() - c.X() ) * ( d.Y() - c.Y() ) );

	float numerator2 = ( ( a.Y() - c.Y() ) * ( b.X() - a.X() ) ) - ( ( a.X() - c.X() ) * ( b.Y() - a.Y() ) );


	if ( numerator1 == 0 || numerator2 == 0 )
		return false;
	
	float r = numerator1 / denominator;
	float s = numerator2 / denominator;

	return ( r > 0 && r < 1 ) && ( s > 0 && s < 1 );
}

void PolygonMap::render(float camX, float camY)
{
	for ( unsigned int i = 0; i < polygons.size(); i++ )
	{
		for ( unsigned int j = 0; j < polygons[i].vertices.size(); j++ )
		{
			SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
			SDL_Rect fillRect = { (polygons[i].vertices[j].X() - 4) - camX, (polygons[i].vertices[j].Y() - 4) - camY, 8, 8 };
			SDL_RenderFillRect( gRenderer, &fillRect );
			SDL_RenderDrawLine( gRenderer, polygons[i].vertices[j].X() - camX, polygons[i].vertices[j].Y() - camY, polygons[i].vertices[(j + 1) % polygons[i].vertices.size()].X() - camX, polygons[i].vertices[(j + 1) % polygons[i].vertices.size()].Y() - camY );
		}
	}
}

PolygonMap::PolygonMap( const PolygonMap &other )
{
	vertices_concave = std::vector<Vector2>(other.vertices_concave);
	polygons = std::vector<Polygon>(other.polygons);
}
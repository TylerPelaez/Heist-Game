#pragma once
#include <vector>
#include <cmath>
#include "Vector2.h"

class Polygon
{
public:
	std::vector<Vector2> vertices;

	Polygon()
	{
		vertices = std::vector<Vector2>();
	}

	Vector2 addPoint( float x, float y )
	{
		Vector2 v = Vector2( x, y );
		vertices.push_back( v );
		return v;
	}

	float distanceSquared( float vx, float vy, float wx, float wy )
	{
		return ( vx - wx )*( vx - wx ) + ( vy - wy )*( vy - wy );
	}

	float distanceToSegmentSquared( float px, float py, float vx, float vy, float wx, float wy )
	{
		float l2 = distanceSquared( vx, vy, wx, wy );
		if ( l2 == 0 )
			return distanceSquared( px, py, vx, vy );

		float t = ( (( px - vx ) * ( wx - vx )) + (( py - vy ) * ( wy - vy )) ) / l2;

		if ( t < 0 )
			return distanceSquared( px, py, vx, vy );
		if ( t > 1 )
			return distanceSquared( px, py, wx, wy );
		return distanceSquared( px, py, vx + (t * ( wx - vx )), vy +( t * ( wy - vy )) );
	}

	float distanceToSegment( float px, float py, float vx, float vy, float wx, float wy )
	{
		return sqrt( distanceToSegmentSquared( px, py, vx, vy, wx, wy ) );
	}


	// pointInside and getClosestPointOnEdge TBD

	bool pointInside( Vector2 _point, bool toleranceOnOutside = true )
	{
		Vector2 point = _point;

		float epsilon = 0.5f;

		bool inside = false;

		if ( vertices.size() < 3 )
			return false;
		Vector2 oldPoint = vertices[vertices.size() - 1];
		float oldSqDist = distanceSquared( oldPoint.X(), oldPoint.Y(), point.X(), point.Y() );

		for ( unsigned int i = 0; i < vertices.size(); i++ )
		{
			Vector2 newPoint = vertices[i];
			float newSqDist = distanceSquared( newPoint.X(), newPoint.Y(), point.X(), point.Y() );

			if ( ( oldSqDist + newSqDist + 2.0 *sqrt( oldSqDist * newSqDist ) - distanceSquared( newPoint.X(), newPoint.Y() , oldPoint.X(), oldPoint.Y())) < epsilon)
				return toleranceOnOutside;

			Vector2 left;
			Vector2 right;

			if ( newPoint.X() > oldPoint.X() )
			{
				left = oldPoint;
				right = newPoint;
			}
			else
			{
				left = newPoint;
				right = oldPoint;
			}

			if ( left.X()  < point.X() && point.X() <= right.X() && ( point.Y() - left.Y() ) * ( right.X() - left.X() ) < ( right.Y() - left.Y() ) * ( point.X() - left.X() ))
				inside = !inside;

			oldPoint = newPoint;
			oldSqDist = newSqDist;
		}

		return inside;
	}

	Vector2 getClosestPointONEdge( Vector2 p3 )
	{
		float tx = p3.X();
		float ty = p3.Y();
		int vi1 = -1;
		int vi2 = -1;

		float mindist = 100000;

		for ( unsigned int i = 0; i < vertices.size(); i++ )
		{
			float dist = distanceToSegment( tx, ty, vertices[i].X(), vertices[i].Y(), vertices[( i + 1 ) % vertices.size()].X(), vertices[( i + 1 ) % vertices.size()].Y() );
			if ( dist < mindist )
			{
				mindist = dist;
				vi1 = i;
				vi2 = ( i + 1 ) % vertices.size();
			}
		}

		Vector2 p1 = vertices[vi1];
		Vector2 p2 = vertices[vi2];

		float x1 = p1.X();
		float y1 = p1.Y();
		float x2 = p2.X();
		float y2 = p2.Y();
		float x3 = p3.X();
		float y3 = p3.Y();

		float u = ( ( ( x3 - x1 ) * ( x2 - x1 ) ) + ( ( y3 - y1 ) * ( y2 - y1 ) ) ) / ( ( ( x2 - x1 ) * ( x2 - x1 ) ) + ( ( y2 - y1 ) * ( y2 - y1 ) ) );

		float xu = x1 + u * ( x2 - x1 );
		float yu = y1 + u * ( y2 - y1 );

		Vector2 linevector;
		if ( u < 0 )
			linevector = Vector2( x1, y1 );
		else if ( u > 1 )
			linevector = Vector2( x2, y2 );
		else
			linevector = Vector2( xu, yu );

		return linevector;
	}
};
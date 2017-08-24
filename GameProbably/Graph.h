#pragma once
#ifndef __GRAPH_H
#include "GraphNode.h"
#include "GraphEdge.h"
#include <vector>

class Graph
{
public:
	
	std::vector<GraphNode> nodes;
	
	std::vector<std::vector<GraphEdge> > edges;
	
	int nextIndex = 0;

	
	Graph()
	{
		nodes = std::vector<GraphNode>();
		edges = std::vector<std::vector<GraphEdge> >();
	}


	Graph clone() const
	{
		Graph g = Graph();

		for ( std::vector<GraphNode>::const_iterator itr = nodes.begin(); itr != nodes.end(); itr++ )
		{
			g.nodes.push_back( (*itr).clone() );
		}
		for ( unsigned int i = 0; i < edges.size(); ++i )
		{
			g.edges.push_back( std::vector<GraphEdge>() );
			for ( std::vector<GraphEdge>::const_iterator itr = edges[i].begin(); itr != edges[i].end(); itr++ )
			{
				g.edges[i].push_back( (*itr).clone() );
			}
		}

		g.nextIndex = nextIndex;

		return g;
	}

	GraphEdge getEdge( int from, int to )
	{
		std::vector<GraphEdge> from_edges = edges[from];
		for ( unsigned int i = 0; i < from_edges.size(); i++ )
		{
			if ( i <= from_edges.size() - 1 )
			{
				if ( from_edges[i].to == to )
					return from_edges[i];
			}
			else
			{
				continue;
			}
		}

		GraphEdge e(-1, -1);
		return e;
	}

	int addNode( GraphNode node )
	{
		nodes.push_back( node );
		edges.push_back( std::vector<GraphEdge>() );
		nextIndex++;
		return 0;
	}

	bool validIndex( int idx )
	{
		return ( idx >= 0 && idx <= nextIndex );
	}

	void addEdge( GraphEdge edge )
	{
		if ( validIndex( edge.to ) && validIndex( edge.from ) )
		{
			if ( getEdge( edge.from, edge.to ).to == -1 )
			{
				edges[edge.from].push_back( edge );
			}
		}
	}

	void render( int r1, int g1, int b1, int r2, int g2, int b2, float camX, float camY )
	{
		for ( unsigned int i = 0; i < nodes.size(); i++ )
		{
			nodes[i].render( r1, g1, b1, camX, camY );	
		}
		SDL_SetRenderDrawColor( gRenderer, r2, g2, b2, 0xFF );

		for ( unsigned int i = 0; i < edges.size(); i++ )
		{
			for ( unsigned int j = 0; j < edges[i].size(); j++ )
			{
				Vector2 p0 = Vector2( nodes[edges[i][j].from].pos.X() - camX, nodes[edges[i][j].from].pos.Y() - camY );
				Vector2 p1 = Vector2( nodes[edges[i][j].to].pos.X() - camX, nodes[edges[i][j].to].pos.Y() - camY );
				SDL_RenderDrawLine( gRenderer, p0.X(), p0.Y(), p1.X(), p1.Y() );
			}
		}
	}
};
#endif
#pragma once
#ifndef _ASTARALOGORITHM_H
#include "Graph.h"
#include "IndexedProrityQueue.h"
#include <algorithm>
#include <cmath>

class DijkstraAlgorithm
{
public:
	Graph graph;

	std::vector<GraphEdge> SPT;
	std::vector<float> cost2Node;
	std::vector<GraphEdge> SF;

	 int source;

	 DijkstraAlgorithm( Graph aGraph, int aSource )
	{
		graph = aGraph;
		source = aSource;

		SPT = std::vector<GraphEdge>();
		cost2Node = std::vector<float>();
		SF = std::vector<GraphEdge>();


		for ( unsigned int i = 0; i < graph.nodes.size(); i++ )
		{
			cost2Node.push_back( 0 );
			GraphEdge e(-1, -1);
			SPT.push_back( e );
			SF.push_back( e );
		}

		search();
	}

	 static Vector2 normalizeVector( Vector2 v )
	{
		float length = v.length();

		return Vector2( v.X() / length, v.Y() / length );
	}

private:
	void search()
	{
		IndexedProrityQueue pq( cost2Node );
		pq.insert( source );
		while ( !pq.isEmpty() )
		{
			int NCN = pq.pop();
			SPT[NCN] = SF[NCN];

			std::vector<GraphEdge> edges = graph.edges[NCN];

			for ( unsigned int i = 0; i < edges.size(); i++ )
			{
				float nCost = cost2Node[NCN] + edges[i].cost;
				if ( SF[edges[i].to].to == -1)
				{
					cost2Node[edges[i].to] = nCost;
					pq.insert( edges[i].to );
					SF[edges[i].to] = edges[i];
				}
				else if ( ( nCost < cost2Node[edges[i].to] ) && ( SPT[edges[i].to].to == -1 ) )
				{
					cost2Node[edges[i].to] = nCost;
					pq.reorderUp();
					SF[edges[i].to] = edges[i];
				}
			}
		}
	}
public:
	 std::vector<int> getPath(int source, int target)
	{
		std::vector<int> path = std::vector<int>();

		if ( ( target < 0 ) )
			return path;
		int nd = target;
		path.push_back( nd );
		while ( ( nd != source ) && ( SPT[nd].to != -1 ) )
		{
			nd = SPT[nd].from;
			path.push_back( nd );
		}
		return path; 
	}
};
#endif
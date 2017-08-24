#pragma once
#ifndef _ASTARALOGORITHM_H
#include "Graph.h"
#include "IndexedProrityQueue.h"
#include <algorithm>
#include <cmath>

class AstarAlgorithm
{
public:
	Graph graph;

	std::vector<GraphEdge> SPT;

	std::vector<float> G_Cost;
	std::vector<float> F_Cost;

	std::vector<GraphEdge> SF;

	 int source;
	 int target;

	 AstarAlgorithm( Graph aGraph, int aSource, int aTarget )
	{
		graph = aGraph;
		source = aSource;
		target = aTarget;

		SPT = std::vector<GraphEdge>();
		G_Cost = std::vector<float>();
		F_Cost = std::vector<float>();
		SF = std::vector<GraphEdge>();


		for ( unsigned int i = 0; i < graph.nodes.size(); i++ )
		{
			G_Cost.push_back( 0 );
			F_Cost.push_back( 0 );
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
		IndexedProrityQueue pq( F_Cost );
		pq.insert( source );
		while ( !pq.isEmpty() )
		{
			int NCN = pq.pop();
			SPT[NCN] = SF[NCN];
			if ( NCN == target )
				return;

			std::vector<GraphEdge> edges = graph.edges[NCN];

			for ( unsigned int i = 0; i < edges.size(); i++ )
			{
				float Hcost = Vector2::Subtract( graph.nodes[edges[i].to].pos, graph.nodes[target].pos ).length();
				float Gcost = G_Cost[NCN] + edges[i].cost;
				if ( SF[edges[i].to].to == -1)
				{
					F_Cost[edges[i].to] = Gcost + Hcost;
					G_Cost[edges[i].to] = Gcost;
					pq.insert( edges[i].to );
					SF[edges[i].to] = edges[i];
				}
				else if ( ( Gcost < G_Cost[edges[i].to] ) && ( SPT[edges[i].to].to == -1 ) )
				{
					F_Cost[edges[i].to] = Gcost + Hcost;
					G_Cost[edges[i].to] = Gcost;
					pq.reorderUp();
					SF[edges[i].to] = edges[i];
				}
			}
		}
	}
public:
	 std::vector<int> getPath()
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

		std::reverse( path.begin(), path.end() );
		return path;
	}
};
#endif
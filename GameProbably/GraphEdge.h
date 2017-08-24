#pragma once

class GraphEdge
{
public:
	int from;
	int to;
	float cost;

	GraphEdge()
	{
		from = -1;
		to = -1;
		cost = -1.0f;
	}

	GraphEdge( int aFrom, int aTo, float aCost = 1.0f )
	{
		from = aFrom;
		to = aTo;
		cost = aCost;
	}

	GraphEdge clone() const
	{
		return GraphEdge( from, to, cost );
	}

	/*void render( int r, int g, int b, Vector2 _from, Vector2 _to )
	{
		SDL_SetRenderDrawColor( gRenderer, r, g, b, 0xFF );
		SDL_RenderDrawLine( gRenderer, _from.X(), _from.Y(), _to.X(), _to.Y() );
	}*/
};
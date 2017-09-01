#pragma once
#ifndef _GRAPHNODE_H
#include "Vector2.h"
#include "FTexture.h"

class GraphNode
{
public:
	Vector2 pos;

	GraphNode()
		: pos(0, 0)
	{}

	GraphNode(Vector2 aPos)
	{
		pos = aPos;
	}

	GraphNode clone() const
	{
		GraphNode ret = GraphNode(Vector2(pos));
		return ret;
	}

	void render(int r, int g, int b, float camX, float camY)
	{
		SDL_SetRenderDrawColor(gRenderer, r, g, b, 0xFF);

		SDL_Rect fillRect = { (int)((pos.X() - 4) - camX), (int)((pos.Y() - 4) - camY), 8, 8 };
		SDL_RenderFillRect(gRenderer, &fillRect);
	}
};
#endif
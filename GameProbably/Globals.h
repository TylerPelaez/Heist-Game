#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "FTexture.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern int LEVEL_WIDTH;
extern int LEVEL_HEIGHT;

extern SDL_Window* gWindow;

extern SDL_Renderer* gRenderer;

enum TEAM
{
	players,
	enemies
};

extern SDL_Rect camera;
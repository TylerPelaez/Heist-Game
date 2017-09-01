#include <Windows.h>
#include <stdio.h>
#include <string>
#include "Globals.h"
#include "PolygonMap.h"
#include "PathfindingHandler.h"
#include <iostream>

const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;

int LEVEL_WIDTH = 3840;
int LEVEL_HEIGHT = 2160;

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

Player* player = NULL;

PathfindingHandler* pfh = NULL;

SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if ( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "No Hamsters Here", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if ( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if ( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if ( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
		// TEST CODE
		player = new Player();
		pfh = new PathfindingHandler(player);
		pfh->init();
		player->map = &pfh->polygonMap;
	}

	return success;
}

bool loadMedia()
{
	bool success = true;
	success = player->setSprite( "assets/playerDot.png" );
	if (!success)
		return success;
	success = pfh->backgroundSprite.loadFromFile( "assets/wallpaperjpg.png" );
	if (!success)
		return success;
	success = player->setMouseSprite( "assets/reticle.png" );
	if (!success)
		return success;
	player->bulletSpritePath = "assets/bullet.png";
	pfh->enemies[0]->setSprite("assets/enemyDot.png");

	return success;
}

void close()
{
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	delete player;
	delete pfh;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void render()
{
	//Clear screen

	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );

	pfh->render();
	player->render();

	//Update screen
	SDL_RenderPresent( gRenderer );
}

void update( float dt )
{	
	player->update( dt );

	camera.x = ( player->getPos().X() + player->getSprite()->getWidth() / 2 ) - SCREEN_WIDTH  / 2;
	camera.y = ( player->getPos().Y() + player->getSprite()->getHeight() / 2 ) - SCREEN_HEIGHT / 2;

	if ( camera.x < 0 ) 
	{ 
		camera.x = 0; 
	}
	if ( camera.y < 0 ) 
	{ 
		camera.y = 0; 
	} 
	if ( camera.x > LEVEL_WIDTH - camera.w ) 
	{ 
		camera.x = LEVEL_WIDTH - camera.w; 
	} 
	if ( camera.y > LEVEL_HEIGHT - camera.h ) 
	{ 
		camera.y = LEVEL_HEIGHT - camera.h; 
	}

	pfh->update( dt, camera.x, camera.y );
	for (auto enemy : pfh->enemies)
	{
		enemy->update(dt);
	}

}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if ( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if ( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			float t = 0.0;
			const float dt = 1.0 / 60.0;  // delta time for 60 FPS

			float currentTime = (((float)SDL_GetTicks()) / 1000.0f);
			float accumulator = 0.0;

			int frame_count = 0;

			SDL_ShowCursor( SDL_DISABLE );

			//While application is running
			while ( !quit )
			{
				//Handle events on queue
				while ( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if ( e.type == SDL_QUIT )
					{
						quit = true;
					}
					else if ( e.type == SDL_MOUSEBUTTONDOWN)
					{
						/*if ( e.button.button == SDL_BUTTON_LEFT )
						{
							if ( !player->walking && pfh->walkpath.size() > 0 )
							{
								player->walking = true;
								pfh->currentwalknode = pfh->walkpath[0];
								pfh->walkpath.erase( pfh->walkpath.begin() );
								pfh->walktox = pfh->polygonMap.targetx;
								pfh->walktoy = pfh->polygonMap.targety;
							}
						}*/
					}
				}
				
				float newTime = ( ( (float)SDL_GetTicks() ) / 1000.0f );
				float frameTime = newTime - currentTime;

				currentTime = newTime;

				accumulator += frameTime;

				while ( accumulator >= dt )
				{
					//std::cout << "Frames: " << frame_count << "Accumulator:" << accumulator << std::endl;

					update( dt );

					accumulator -= dt;
					t += dt;
				}




				render();

				frame_count += 1;
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
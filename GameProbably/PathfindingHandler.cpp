#include "PathfindingHandler.h"

PathfindingHandler::PathfindingHandler( Player* p )
	: polygonMap( SCREEN_WIDTH, SCREEN_HEIGHT )
{
	player = p;
}

void PathfindingHandler::update( float dt, float camX, float camY )
{
	/*if ( player->walking )
	{
		walkPlayer();
	}
	else
	{
		int x, y;
		SDL_GetMouseState( &x, &y );
		x += camX;
		y += camY;
		walkpath = polygonMap.calculatePath( Vector2( player->pos.X(), player->pos.Y() ), Vector2( x, y ) );
	}*/
}

void PathfindingHandler::render()
{
	SDL_Rect renderQuad = {-camera.x, -camera.y, backgroundSprite.getWidth(),  backgroundSprite.getHeight() };
	backgroundSprite.render( NULL, renderQuad );
	polygonMap.render(camera.x, camera.y);

	polygonMap.walkgraph.render( 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, camera.x, camera.y );
}


//void PathfindingHandler::walkPlayer()
//{
//	float tempwalktox = polygonMap.walkgraph.nodes[currentwalknode].pos.X();
//	float tempwalktoy = polygonMap.walkgraph.nodes[currentwalknode].pos.Y();
//
//	Vector2 b = Vector2( player->pos.X(), player->pos.Y() );
//	Vector2 a = Vector2( tempwalktox, tempwalktoy );
//
//	Vector2 c = Vector2::Subtract( a, b );
//
//	if ( c.length() >= player->walkspeed )
//	{
//		c.normalize();
//		c.X( c.X() * player->walkspeed );
//		c.Y( c.Y() * player->walkspeed );
//	}
//
//	b = Vector2::Add( b, c );
//
//	player->pos.X( b.X() );
//	player->pos.Y( b.Y() );
//
//	if ( walkpath.size() > 0 )
//	{
//		if ( tempwalktox == player->pos.X() && tempwalktoy == player->pos.Y() )
//		{
//			currentwalknode = walkpath[0];
//			walkpath.erase( walkpath.begin() );
//		}
//	}
//	if ( walktox == player->pos.X() && walktoy == player->pos.Y() )
//	{
//		player->walking = false;
//	}
//
//}

void PathfindingHandler::initializeWalkableArea( int p )
{
	player->setPos( Vector2(63.0f, 290.0f) );

	player->walking = false;
	walktox = 0;
	walktoy = 0;
	currentwalknode = 0;
	int polyId = 0;
	polygonMap = PolygonMap( SCREEN_WIDTH, SCREEN_HEIGHT );
	polygonMap.polygons.push_back (Polygon());
	if ( p == 1 )
	{
		polygonMap.polygons[polyId].addPoint( 0, 0 );
		polygonMap.polygons[polyId].addPoint( 3839, 0 );
		polygonMap.polygons[polyId].addPoint( 3839, 2159 );
		polygonMap.polygons[polyId].addPoint( 0, 2159 );



		/*polygonMap.polygons[polyId].addPoint( 4, 100 );
		polygonMap.polygons[polyId].addPoint( 400, 100 );
		polygonMap.polygons[polyId].addPoint( 400, 200 );
		polygonMap.polygons[polyId].addPoint( 500, 200 );
		polygonMap.polygons[polyId].addPoint( 500, 100 );
		polygonMap.polygons[polyId].addPoint( 800, 101 );
		polygonMap.polygons[polyId].addPoint( 1000, 340 );
		polygonMap.polygons[polyId].addPoint( 1000, 700 );
		polygonMap.polygons[polyId].addPoint( 3, 700 );*/
		polyId++;

		polygonMap.polygons.push_back( Polygon() );
		polygonMap.polygons[polyId].addPoint( 1400, 1400 );
		polygonMap.polygons[polyId].addPoint( 1600, 1400 );
		polygonMap.polygons[polyId].addPoint( 1600, 1600 );
		polygonMap.polygons[polyId].addPoint( 1400, 1600 );
	}

	polygonMap.CreateGraph();
}

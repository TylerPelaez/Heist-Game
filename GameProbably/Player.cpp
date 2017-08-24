#include "Player.h"


void Player::move()
{
	setPos(Vector2(pos.X() + velocity.X(), pos.Y() + velocity.Y()));

	float playerRight = pos.X() + sprite.getWidth();
	float playerLeft = pos.X();
	float playerBottom = pos.Y() + sprite.getHeight();
	float playerTop = pos.Y();


	Vector2 xCheck( pos.X() + sprite.getWidth(), pos.Y() );
	Vector2 yCheck( pos.X(), pos.Y() + sprite.getHeight() );

	if (pos.X() < 0 || xCheck.X() > LEVEL_WIDTH || pos.Y() < 0 || yCheck.Y() > LEVEL_HEIGHT)
	{
		bool notIn = ((!map->polygons[0].pointInside(pos)) || (!map->polygons[0].pointInside(xCheck)) || (!map->polygons[0].pointInside(yCheck)));
		if (notIn)
		{
			pos.X(pos.X() - velocity.X()); // There's a collision! what if we don't move in the X?
			collider.boundingBox.x = pos.X();
			xCheck.X(pos.X() + sprite.getWidth());
			yCheck.X(pos.X());

			notIn = ((!map->polygons[0].pointInside(pos)) || (!map->polygons[0].pointInside(xCheck)) || (!map->polygons[0].pointInside(yCheck)));
			if (notIn)
			{
				pos.X(pos.X() + velocity.X()); // not moving X is still out of bounds. maybe if we try not moving in Y instead...
				collider.boundingBox.x = pos.X();
				xCheck.X(pos.X() + sprite.getWidth());
				yCheck.X(pos.X());

				pos.Y(pos.Y() - velocity.Y());
				collider.boundingBox.y = pos.Y();
				xCheck.Y(pos.Y());
				yCheck.Y(pos.Y() + sprite.getHeight());

				notIn = ((!map->polygons[0].pointInside(pos)) || (!map->polygons[0].pointInside(xCheck)) || (!map->polygons[0].pointInside(yCheck)));
				if (notIn)
				{
					pos.X(pos.X() - velocity.X()); // No luck! You're still colliding (Must be a cozy corner!), don't move.
					collider.boundingBox.x = pos.X();
					velocity.X(0);
					velocity.Y(0);
				}
			}
		}
		return;
	}

	if ( map->polygons.size() > 1 )
	{
		bool movedX = false;
		bool movedY = false;

		for ( unsigned int i = 1; i < map->polygons.size(); i++ )
		{
			float polyLeft = map->polygons[i].vertices[0].X();
			float polyRight = map->polygons[i].vertices[2].X();
			float polyTop = map->polygons[i].vertices[0].Y();
			float polyBottom = map->polygons[i].vertices[2].Y();
			SDL_Rect polyRect = { polyLeft, polyTop, polyRight - polyLeft, polyBottom - polyTop };

			if ( collider.checkCollision( polyRect ) )
			{
				// There's a collision! what if we don't move in the X?
				// if it's already been tried, then just move back the Y and return
				if ( !movedX ) 
				{
					pos.X( pos.X() - velocity.X() ); 
					collider.boundingBox.x = pos.X();
				}
				if ( collider.checkCollision( polyRect ) )
				{
					if ( !movedX ) // X wasn't moved by a previous check, safe to put it back and try not moving in Y only
					{
						pos.X( pos.X() + velocity.X() ); 
						collider.boundingBox.x = pos.X();
					}

					if ( !movedY ) // Y wasn't moved by a previous check, so try moving it and checking (if the previous one already moved it, and the x Check failed, then theres no point in checking.
					{
						pos.Y( pos.Y() - velocity.Y() );
						collider.boundingBox.y = pos.Y();

						if ( collider.checkCollision( polyRect ) )
						{
							if ( !movedX )
							{
								pos.X( pos.X() - velocity.X() ); // No luck! You're still colliding (Must be a cozy corner!), don't move.
								collider.boundingBox.x = pos.X();
								velocity.X( 0 );
							}
							// moving back both x and y fails, so just give up.
							velocity.Y( 0 );
							return;
						}
						else
						{
							movedY = true;
							// moving Y was successful, keep checking...
						}
					}
					
				}
				else
				{
					// Moving the X was successful! keep checking...
					movedX = true;
				}
			}
		}
	}
}

void Player::render()
{
	float camX = camera.x;
	float camY = camera.y;
	SDL_Rect renderQuad = { pos.X() - camX, pos.Y() - camY, sprite.getWidth(), sprite.getHeight() };
	sprite.render( NULL, renderQuad );

	int mouseX, mouseY;
	SDL_GetMouseState( &mouseX, &mouseY );
	SDL_Rect mouseRenderQuad = { mouseX, mouseY, mouseSprite.getWidth(), mouseSprite.getHeight() };
	mouseSprite.render( NULL, mouseRenderQuad );

	for ( std::list<Bullet>::iterator itr = bulletList.begin(); itr != bulletList.end(); ++itr )
	{
		itr->render();
	}
}

void Player::update( float dt )
{
	int keySize = 0;
	const Uint8* state = SDL_GetKeyboardState( &keySize );

	if ( state[SDL_SCANCODE_W] )
	{
		if ( velocity.Y() > 0.0f )
			velocity.Y( 0.0f );
		else
		{
			velocity.Y( velocity.Y() - ( acceleration ) );
			if ( velocity.Y() < -maxWalkSpeed )
				velocity.Y( -maxWalkSpeed );
		}
	}
	else if ( state[SDL_SCANCODE_S] )
	{
		if ( velocity.Y() < 0.0f )
			velocity.Y( 0.0f );
		else
		{
			velocity.Y( velocity.Y() + ( acceleration ) );
			if ( velocity.Y() > maxWalkSpeed )
				velocity.Y( maxWalkSpeed );
		}
	}
	else
	{
		velocity.Y( 0.0f );
	}

	if ( state[SDL_SCANCODE_A] )
	{
		if ( velocity.X() > 0.0f )
			velocity.X( 0.0f );
		else
		{
			velocity.X( velocity.X() - ( acceleration ) );
			if ( velocity.X() < -maxWalkSpeed )
				velocity.X( -maxWalkSpeed );
		}
	}
	else if ( state[SDL_SCANCODE_D] )
	{
		if ( velocity.X() < 0.0f )
			velocity.X( 0.0f );
		else
		{
			velocity.X( velocity.X() + ( acceleration ) );
			if ( velocity.X() > maxWalkSpeed )
				velocity.X( maxWalkSpeed );
		}
	}
	else
	{
		velocity.X( 0.0f );
	}

	cooldownTimer -= dt;

	int mouseX, mouseY;

	if ( ( SDL_GetMouseState( &mouseX, &mouseY ) & SDL_BUTTON( 1 ) ) && cooldownTimer <= 0.0f )
	{
		int camX = camera.x;
		int camY = camera.y;

		mouseX += camX;
		mouseY += camY;

		Vector2 target( mouseX + ( mouseSprite.getWidth() / 2 ), mouseY + ( mouseSprite.getHeight() / 2 ) );

		fireAt( target );
	}


	updateBulletList( dt );
	move();
}

bool Player::setMouseSprite(std::string spritePath)
{
	return mouseSprite.loadFromFile(spritePath);
}

const FTexture* Player::getMouseSprite()
{
	return &mouseSprite;
}
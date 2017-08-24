#pragma once
#ifndef _VECTOR2_H
#include <array>
#include <cmath>

class Vector2 : public std::array<float, 2>
{
private:
	float x;
	float y;


public:
	Vector2()
		: std::array<float, 2>()
	{}

	Vector2( float _x, float _y )
	{
		( *this )[0] = _x;
		( *this )[1] = _y;
		x = _x;
		y = _y;
	}

	Vector2( const Vector2 &other )
		: std::array<float, 2>()
	{
		( *this )[0] = other.x;
		( *this )[1] = other.y;
		x = other.x;
		y = other.y;
	}


	void X( float _x )
	{
		x = _x;
		( *this )[0] = _x;
	}

	float X() const
	{
		return x;
	}

	void Y( float _y )
	{
		y = _y;
		( *this )[1] = _y;
	}

	float Y() const
	{
		return y;
	}

	void setXY( float _x, float _y )
	{
		X( _x );
		Y( _y );
	}

	static Vector2 Subtract( const Vector2 &left, const Vector2 &right )
	{
		return Vector2( left.X() - right.X(), left.Y() - right.Y() );
	}

	static Vector2 Add( const Vector2 &left, const Vector2 &right )
	{
		return Vector2( left.X() + right.X(), left.Y() + right.Y() );
	}

	float length()
	{
		return sqrt( x * x + y * y );
	}

	Vector2* divideScalar( float v )
	{
		if ( v != 0 )
		{
			x = x / v;
			y = y / v;
		}
		else
		{
			x = 0;
			y = 0;
		}

		return this;
	}
	
	Vector2* normalize()
	{
		return divideScalar( length() );
	}

};
#endif
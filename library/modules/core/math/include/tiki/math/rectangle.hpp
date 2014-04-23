#ifndef TIKI_RECTANGLE_HPP__
#define TIKI_RECTANGLE_HPP__

#include "tiki/base/types.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	struct Rectangle
	{
		float x;
		float y;
		float width;
		float height;

		Rectangle( )
			: x ( 0.0f ), y( 0.0f ), width( 0.0f ), height( 0.0f )
		{
		}

		Rectangle( float _x, float _y, float _width, float _height )
			: x( _x ), y( _y ), width( _width ), height( _height )
		{
		}

		bool contains( const Vector2& point )
		{
			return ( point.x >= x ) && ( point.x <= x + width ) && ( point.y >= y ) && ( point.y <= y + height );
		}

		Vector2 xy()
		{
			const Vector2 xy = { x, y };
			return xy;
		}
	};
}

#endif // TIKI_RECTANGLE_HPP__

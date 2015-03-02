#ifndef TIKI_RECTANGLE_HPP__
#define TIKI_RECTANGLE_HPP__

#include "tiki/base/inline.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	struct Rectangle
	{
		float x;
		float y;
		float width;
		float height;

		TIKI_FORCE_INLINE Rectangle( )
			: x( 0.0f ), y( 0.0f ), width( 0.0f ), height( 0.0f )
		{
		}

		TIKI_FORCE_INLINE Rectangle( float _x, float _y, float _width, float _height )
			: x( _x ), y( _y ), width( _width ), height( _height )
		{
		}

		TIKI_FORCE_INLINE bool contains( const Vector2& point ) const
		{
			return ( point.x >= x ) && ( point.x <= x + width ) && ( point.y >= y ) && ( point.y <= y + height );
		}

		TIKI_FORCE_INLINE float getRight() const
		{
			return x + width;
		}

		TIKI_FORCE_INLINE float getBottom() const
		{
			return y + height;
		}

		TIKI_FORCE_INLINE Vector2 getXY() const
		{
			return vector::create( x, y );
		}

		TIKI_FORCE_INLINE Vector2 getWidthHeight() const
		{
			return vector::create( width, height );
		}
	};
}

#endif // TIKI_RECTANGLE_HPP__

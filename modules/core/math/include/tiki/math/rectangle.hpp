#ifndef TIKI_RECTANGLE_HPP__
#define TIKI_RECTANGLE_HPP__

#include "tiki/math/vector2.hpp"

namespace tiki
{
	struct Rectangle
	{
		float x, y, width, height;

		Rectangle( ) :
			x ( 0 ), y( 0 ), width( 1 ), height( 1 )
		{

		}

		Rectangle( float X, float Y, float Width, float Height ):
			x( X ), y( Y ), width( Width ), height( Height )
		{

		}

		Rectangle( const Rectangle& other )
			: x( other.x ), y( other.y ), width( other.width ), height( other.height )
		{
			x = x;
		}

		Rectangle& operator=( const Rectangle& other )
		{
			x		= other.x;
			y		= other.y;
			width	= other.width;
			height	= other.height;
			return *this;
		}

		void mul( const Vector2& scal )
		{
			x		*= scal.x;
			y		*= scal.y;
			width	*= scal.x;
			height	*= scal.y;
		}
	};

}

#endif // TIKI_RECTANGLE_HPP__

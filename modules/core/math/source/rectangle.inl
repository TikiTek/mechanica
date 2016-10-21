#pragma once
#ifndef TIKI_RECTANGLE_INL_INCLUDED
#define TIKI_RECTANGLE_INL_INCLUDED

#include "tiki/math/matrix.hpp"

namespace tiki
{
	TIKI_FORCE_INLINE void Rectangle::createFromCenterExtends( const Vector2& _center, const Vector2& _extents, float _rotation )
	{
		center		= _center;
		extends		= _extents;
		rotation	= _rotation;
	}

	TIKI_FORCE_INLINE void Rectangle::createFromMinMax( const Vector2& min, const Vector2& max )
	{
		vector::add( center, min, max );
		vector::scale( center, 0.5f );

		vector::sub( extends, max, min );

		rotation = 0.0f;
	}

	TIKI_FORCE_INLINE void Rectangle::createFromMinMax( float minX, float minY, float maxX, float maxY )
	{
		createFromMinMax( vector::create( minX, minY ), vector::create( minX, minY ) );
	}

	TIKI_FORCE_INLINE void Rectangle::getVertices( Vector2 aVertices[ RectanglePoint_Count ] ) const
	{
		Vector2 halfExtends = extends;
		vector::scale( halfExtends, 0.5f );

		Vector2 min;
		Vector2 max;
		vector::sub( min, center, halfExtends );
		vector::add( max, center, halfExtends );

		vector::set( aVertices[ RectanglePoint_XMinYMin ], min.x, min.y );
		vector::set( aVertices[ RectanglePoint_XMaxYMin ], max.x, min.y );
		vector::set( aVertices[ RectanglePoint_XMaxYMax ], max.x, max.y );
		vector::set( aVertices[ RectanglePoint_XMinYMax ], min.x, max.y );

		Matrix22 rotationMatrix;
		matrix::createRotationZ( rotationMatrix, rotation );

		for( uint i = 0u; i < TIKI_COUNT( aVertices ); ++i )
		{
			matrix::transform( aVertices[ i ], rotationMatrix );
		}
	}

	TIKI_FORCE_INLINE void Rectangle::clear()
	{
		vector::clear( center );
		vector::clear( extends );
		rotation = 0.0f;
	}

	TIKI_FORCE_INLINE void Rectangle::translate( const Vector2& translation )
	{
		vector::add( center, translation );
	}

	TIKI_FORCE_INLINE void Rectangle::extend( const Vector2& _extents )
	{
		vector::add( extends, _extents );
	}
}

#endif // TIKI_RECTANGLE_INL_INCLUDED

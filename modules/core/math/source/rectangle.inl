#pragma once
#ifndef TIKI_RECTANGLE_INL_INCLUDED
#define TIKI_RECTANGLE_INL_INCLUDED

namespace tiki
{
	TIKI_FORCE_INLINE void Rectangle::createFromCenterExtends( const Vector2& center, const Vector2& extents )
	{
		Vector2 halfExtents = extents;
		vector::scale( halfExtents, 0.5f );

		vector::sub( min, center, halfExtents );
		vector::add( max, center, halfExtents );
	}

	TIKI_FORCE_INLINE void Rectangle::createFromMinMax( const Vector2& _min, const Vector2& _max )
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

	TIKI_FORCE_INLINE void Rectangle::getVertices( Vector2 aVertices[ RectangleVertex_Count ] ) const
	{
		vector::set( aVertices[ RectangleVertex_XMinYMin ], min.x, min.y );
		vector::set( aVertices[ RectangleVertex_XMaxYMin ], max.x, min.y );
		vector::set( aVertices[ RectangleVertex_XMaxYMax ], max.x, max.y );
		vector::set( aVertices[ RectangleVertex_XMinYMax ], min.x, max.y );

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

	TIKI_FORCE_INLINE bool Rectangle::contains( const Vector2& point ) const
	{
		return min.x <= point.x && min.y <= point.y &&
			max.x >= point.x && max.y >= point.y
	}

	TIKI_FORCE_INLINE Vector2 Rectangle::getCenter() const
	{
		Vector2 center;
		vector::add( center, min, max );
		vector::scale( center, 0.5f );
		return center;
	}

	TIKI_FORCE_INLINE Vector2 Rectangle::getSize() const
	{
		Vector2 size;
		vector::sub( size, max, min );
		return size;
	}

	TIKI_FORCE_INLINE float Rectangle::getLeft() const
	{
		return min.x;
	}

	TIKI_FORCE_INLINE float Rectangle::getTop() const
	{
		return min.y;
	}

	TIKI_FORCE_INLINE float Rectangle::getRight() const
	{
		return max.x;
	}

	TIKI_FORCE_INLINE float Rectangle::getBottom() const
	{
		return max.y;
	}
}

#endif // TIKI_RECTANGLE_INL_INCLUDED

#pragma once
#ifndef TIKI_AXISALIGNEDRECTANGLE_INL_INCLUDED
#define TIKI_AXISALIGNEDRECTANGLE_INL_INCLUDED

namespace tiki
{
	TIKI_FORCE_INLINE void AxisAlignedRectangle::clear()
	{
		vector::clear( min );
		vector::clear( max );
	}

	TIKI_FORCE_INLINE void AxisAlignedRectangle::getVertices( Vector2 aVertices[ RectanglePoint_Count ] ) const
	{
		vector::set( aVertices[ RectanglePoint_XMinYMin ], min.x, min.y );
		vector::set( aVertices[ RectanglePoint_XMaxYMin ], max.x, min.y );
		vector::set( aVertices[ RectanglePoint_XMaxYMax ], max.x, max.y );
		vector::set( aVertices[ RectanglePoint_XMinYMax ], min.x, max.y );
	}
	
	TIKI_FORCE_INLINE void AxisAlignedRectangle::translate( const Vector2& translation )
	{
		vector::add( min, translation );
		vector::add( max, translation );
	}

	TIKI_FORCE_INLINE void AxisAlignedRectangle::extend( const Vector2& extents )
	{
		Vector2 halfExtents = extents;
		vector::scale( halfExtents, 0.5f );

		vector::sub( min, halfExtents );
		vector::add( max, halfExtents );
	}

	TIKI_FORCE_INLINE bool AxisAlignedRectangle::contains( const Vector2& point ) const
	{
		return min.x <= point.x && min.y <= point.y &&
			max.x >= point.x && max.y >= point.y;
	}

	TIKI_FORCE_INLINE Vector2 AxisAlignedRectangle::getCenter() const
	{
		Vector2 center;
		vector::add( center, min, max );
		vector::scale( center, 0.5f );
		return center;
	}

	TIKI_FORCE_INLINE Vector2 AxisAlignedRectangle::getSize() const
	{
		Vector2 size;
		vector::sub( size, max, min );
		return size;
	}

	TIKI_FORCE_INLINE float AxisAlignedRectangle::getWidth() const
	{
		return max.x - min.x;
	}

	TIKI_FORCE_INLINE float AxisAlignedRectangle::getHeight() const
	{
		return max.y - min.y;
	}

	TIKI_FORCE_INLINE float AxisAlignedRectangle::getLeft() const
	{
		return min.x;
	}

	TIKI_FORCE_INLINE float AxisAlignedRectangle::getTop() const
	{
		return min.y;
	}

	TIKI_FORCE_INLINE float AxisAlignedRectangle::getRight() const
	{
		return max.x;
	}

	TIKI_FORCE_INLINE float AxisAlignedRectangle::getBottom() const
	{
		return max.y;
	}

	TIKI_FORCE_INLINE AxisAlignedRectangle createAxisAlignedRectangle( float x, float y, float width, float height )
	{
		AxisAlignedRectangle rect;

		rect.min.x	= x;
		rect.min.y	= y;
		rect.max.x	= x + width;
		rect.max.y	= y + height;

		return rect;
	}

	TIKI_FORCE_INLINE AxisAlignedRectangle createAxisAlignedRectangle( const Vector2& center, const Vector2& extents )
	{
		AxisAlignedRectangle rect;

		Vector2 halfExtents = extents;
		vector::scale( halfExtents, 0.5f );

		vector::sub( rect.min, center, halfExtents );
		vector::add( rect.max, center, halfExtents );

		return rect;
	}

	TIKI_FORCE_INLINE AxisAlignedRectangle createAxisAlignedRectangleMinMax( const Vector2& _min, const Vector2& _max )
	{
		AxisAlignedRectangle rect;

		rect.min = _min;
		rect.max = _max;

		return rect;
	}

	TIKI_FORCE_INLINE AxisAlignedRectangle createAxisAlignedRectangleMinMax( float minX, float minY, float maxX, float maxY )
	{
		AxisAlignedRectangle rect;

		vector::set( rect.min, minX, minY );
		vector::set( rect.max, maxX, maxY );

		return rect;
	}
}

#endif // TIKI_AXISALIGNEDRECTANGLE_INL_INCLUDED

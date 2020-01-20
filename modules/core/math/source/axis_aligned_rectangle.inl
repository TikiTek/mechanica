#pragma once
#ifndef TIKI_AXISALIGNEDRECTANGLE_INL_INCLUDED
#define TIKI_AXISALIGNEDRECTANGLE_INL_INCLUDED

namespace tiki
{
	inline void AxisAlignedRectangle::clear()
	{
		vector::clear( min );
		vector::clear( max );
	}

	inline void AxisAlignedRectangle::getVertices( Vector2 aVertices[ RectanglePoint_Count ] ) const
	{
		vector::set( aVertices[ RectanglePoint_XMinYMin ], min.x, min.y );
		vector::set( aVertices[ RectanglePoint_XMaxYMin ], max.x, min.y );
		vector::set( aVertices[ RectanglePoint_XMaxYMax ], max.x, max.y );
		vector::set( aVertices[ RectanglePoint_XMinYMax ], min.x, max.y );
	}

	inline void AxisAlignedRectangle::translate( const Vector2& translation )
	{
		vector::add( min, translation );
		vector::add( max, translation );
	}

	inline void AxisAlignedRectangle::extend( const Vector2& extents )
	{
		Vector2 halfExtents = extents;
		vector::scale( halfExtents, 0.5f );

		vector::sub( min, halfExtents );
		vector::add( max, halfExtents );
	}

	inline bool AxisAlignedRectangle::contains( const Vector2& point ) const
	{
		return min.x <= point.x && min.y <= point.y &&
			max.x >= point.x && max.y >= point.y;
	}

	inline AxisAlignedRectangle AxisAlignedRectangle::create( float x, float y, float width, float height )
	{
		AxisAlignedRectangle rect;
		rect.min.x	= x;
		rect.min.y	= y;
		rect.max.x	= x + width;
		rect.max.y	= y + height;

		return rect;
	}

	inline AxisAlignedRectangle AxisAlignedRectangle::create( const Vector2& position, const Vector2& size )
	{
		AxisAlignedRectangle rect;
		rect.min.x	= position.x;
		rect.min.y	= position.y;
		rect.max.x	= position.x + size.x;
		rect.max.y	= position.y + size.y;

		return rect;
	}

	inline AxisAlignedRectangle AxisAlignedRectangle::createCentered( const Vector2& center, const Vector2& extents )
	{
		AxisAlignedRectangle rect;

		Vector2 halfExtents = extents;
		vector::scale( halfExtents, 0.5f );

		vector::sub( rect.min, center, halfExtents );
		vector::add( rect.max, center, halfExtents );

		return rect;
	}

	inline AxisAlignedRectangle AxisAlignedRectangle::createMinMax( const Vector2& _min, const Vector2& _max )
	{
		AxisAlignedRectangle rect;

		rect.min = _min;
		rect.max = _max;

		return rect;
	}

	inline AxisAlignedRectangle AxisAlignedRectangle::createMinMax( float minX, float minY, float maxX, float maxY )
	{
		AxisAlignedRectangle rect;

		vector::set( rect.min, minX, minY );
		vector::set( rect.max, maxX, maxY );

		return rect;
	}

	inline AxisAlignedRectangle AxisAlignedRectangle::createFill( const Vector2& targetSize, const Vector2& sourceSize )
	{
		const Vector2 diff		= Vector2::div( targetSize, sourceSize );
		const Vector2 extends	= Vector2::scale( sourceSize, (diff.x > diff.y ? diff.x : diff.y) );

		return AxisAlignedRectangle::createCentered( Vector2::zero, extends );
	}
}

#endif // TIKI_AXISALIGNEDRECTANGLE_INL_INCLUDED

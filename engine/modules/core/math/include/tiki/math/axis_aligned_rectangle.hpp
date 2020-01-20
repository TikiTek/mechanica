#pragma once

#include "tiki/math/rectangle_point.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	//	3-------2
	//  |       |
	//	|  	    |
	//	|       |
	//	0-------1
	struct AxisAlignedRectangle
	{
		Vector2								min;
		Vector2								max;

		inline void							clear();

		inline void							getVertices( Vector2 aVertices[ RectanglePoint_Count ] ) const;

		inline void							translate( const Vector2& translation );
		inline void							extend( const Vector2& extents );

		inline bool							contains( const Vector2& point ) const;

		inline Vector2						getCenter() const	{ return Vector2::scale( Vector2::add( min, max ), 0.5f ); }
		inline Vector2						getSize() const		{ return Vector2::sub( max, min ); }

		inline float						getWidth() const	{ return max.x - min.x; }
		inline float						getHeight() const	{ return max.y - min.y; }

		inline float						getLeft() const		{ return min.x; }
		inline float						getTop() const		{ return min.y; }
		inline float						getRight() const	{ return max.x; }
		inline float						getBottom() const	{ return max.y; }

		static inline AxisAlignedRectangle create( float x, float y, float width, float height );
		static inline AxisAlignedRectangle create( const Vector2& position, const Vector2& size );
		static inline AxisAlignedRectangle createCentered( const Vector2& center, const Vector2& extents );
		static inline AxisAlignedRectangle createMinMax( const Vector2& _min, const Vector2& _max );
		static inline AxisAlignedRectangle createMinMax( float minX, float minY, float maxX, float maxY );
		static inline AxisAlignedRectangle createFill( const Vector2& targetSize, const Vector2& sourceSize );
	};
}

#include "../../../source/axis_aligned_rectangle.inl"

#pragma once
#ifndef TIKI_AXISALIGNEDRECTANGLE_HPP_INCLUDED
#define TIKI_AXISALIGNEDRECTANGLE_HPP_INCLUDED

#include "tiki/math/rectanglepoint.hpp"
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
	public:

		Vector2						min;
		Vector2						max;

		TIKI_FORCE_INLINE void		clear();

		TIKI_FORCE_INLINE void		getVertices( Vector2 aVertices[ RectanglePoint_Count ] ) const;

		TIKI_FORCE_INLINE void		translate( const Vector2& translation );
		TIKI_FORCE_INLINE void		extend( const Vector2& extents );

		TIKI_FORCE_INLINE bool		contains( const Vector2& point ) const;

		TIKI_FORCE_INLINE Vector2	getCenter() const;
		TIKI_FORCE_INLINE Vector2	getSize() const;

		TIKI_FORCE_INLINE float		getWidth() const;
		TIKI_FORCE_INLINE float		getHeight() const;

		TIKI_FORCE_INLINE float		getLeft() const;
		TIKI_FORCE_INLINE float		getTop() const;
		TIKI_FORCE_INLINE float		getRight() const;
		TIKI_FORCE_INLINE float		getBottom() const;
	};

	TIKI_FORCE_INLINE AxisAlignedRectangle createAxisAlignedRectangle( float x, float y, float width, float height );
	TIKI_FORCE_INLINE AxisAlignedRectangle createAxisAlignedRectangle( const Vector2& center, const Vector2& extents );
	TIKI_FORCE_INLINE AxisAlignedRectangle createAxisAlignedRectangleMinMax( const Vector2& _min, const Vector2& _max );
	TIKI_FORCE_INLINE AxisAlignedRectangle createAxisAlignedRectangleMinMax( float minX, float minY, float maxX, float maxY );
}

#include "../../../source/axisalignedrectangle.inl"

#endif // TIKI_AXISALIGNEDRECTANGLE_HPP_INCLUDED

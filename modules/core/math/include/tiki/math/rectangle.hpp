#ifndef TIKI_RECTANGLE_HPP__
#define TIKI_RECTANGLE_HPP__

#include "tiki/base/types.hpp"
#include "tiki/math/rectanglepoint.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	//      2
	//     / \
	//    /   \
	//   /     \
	//	3       1
	//   \     /
	//	  \   /
	//	   \ /
	//	    0
	struct Rectangle
	{
		Vector2						center;
		Vector2						extends;
		float						rotation;

		TIKI_FORCE_INLINE void		createFromCenterExtends( const Vector2& _center, const Vector2& _extents, float _rotation );
		TIKI_FORCE_INLINE void		createFromMinMax( const Vector2& min, const Vector2& max );
		TIKI_FORCE_INLINE void		createFromMinMax( float minX, float minY, float maxX, float maxY );

		TIKI_FORCE_INLINE void		clear();

		TIKI_FORCE_INLINE void		getVertices( Vector2 aVertices[ RectanglePoint_Count ] ) const;

		TIKI_FORCE_INLINE void		translate( const Vector2& translation );
		TIKI_FORCE_INLINE void		extend( const Vector2& _extents );
	};
}
#include "../../../source/rectangle.inl"

#endif // TIKI_RECTANGLE_HPP__

#ifndef TIKI_RECTANGLE_HPP__
#define TIKI_RECTANGLE_HPP__

#include "tiki/base/types.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	enum RectangleVertex
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
		

		RectangleVertex_XMinYMin,
		RectangleVertex_XMaxYMin,
		RectangleVertex_XMaxYMax,
		RectangleVertex_XMinYMax,

		RectangleVertex_Count
	};

	struct Rectangle
	{
		Vector2						center;
		Vector2						extends;
		float						rotation;

		TIKI_FORCE_INLINE void		createFromCenterExtends( const Vector2& center, const Vector2& extents, float rotation );
		TIKI_FORCE_INLINE void		createFromMinMax( const Vector2& _min, const Vector2& _max );
		TIKI_FORCE_INLINE void		createFromMinMax( float minX, float minY, float maxX, float maxY );

		TIKI_FORCE_INLINE void		clear();

		TIKI_FORCE_INLINE void		getVertices( Vector2 aVertices[ RectangleVertex_Count ] ) const;

		TIKI_FORCE_INLINE void		translate( const Vector2& translation );
		TIKI_FORCE_INLINE void		extend( const Vector2& _extents );
	};
}

#endif // TIKI_RECTANGLE_HPP__

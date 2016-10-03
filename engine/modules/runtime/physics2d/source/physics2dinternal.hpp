#pragma once
#ifndef TIKI_PHYSICS2DINTERNAL_HPP_INCLUDED
#define TIKI_PHYSICS2DINTERNAL_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/math/vector.hpp"

#include <Box2D/Common/b2Math.h>

namespace tiki
{
	TIKI_FORCE_INLINE b2Vec2 toBox2DVector( const Vector2& vec )
	{
		return b2Vec2( vec.x, vec.y );
	}

	TIKI_FORCE_INLINE Vector2 toTikiVector( const b2Vec2& vec )
	{
		return vector::create( vec.x, vec.y );
	}

	TIKI_FORCE_INLINE Vector3 toTikiVector( const b2Vec2& vec, float z )
	{
		return vector::create( vec.x, vec.y, z );
	}
}

#endif // TIKI_PHYSICS2DINTERNAL_HPP_INCLUDED

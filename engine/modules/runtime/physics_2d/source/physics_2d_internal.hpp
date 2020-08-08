#pragma once

#include "tiki/base/types.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/math/axis_aligned_rectangle.hpp"

#include <box2d/b2_math.h>

namespace tiki
{
	TIKI_FORCE_INLINE b2Vec2 toPhysicsVector( const Vector2& vec )
	{
		return b2Vec2( vec.x, vec.y );
	}

	TIKI_FORCE_INLINE Vector2 toEngineVector( const b2Vec2& vec )
	{
		return vector::create( vec.x, vec.y );
	}

	TIKI_FORCE_INLINE Vector3 toEngineVector( const b2Vec2& vec, float z )
	{
		return vector::create( vec.x, vec.y, z );
	}
}

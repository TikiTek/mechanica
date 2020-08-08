#pragma once

#include "tiki/math/vector.hpp"

namespace tiki
{
	struct Ray2
	{
		Vector2 origin;
		Vector2 direction;

		TIKI_FORCE_INLINE Ray2()
		{
		}

		TIKI_FORCE_INLINE Ray2( const Vector2& _origin, const Vector2& _direction )
		{
			TIKI_ASSERT( vector::isNormalized( _direction ) );

			origin		= _origin;
			direction	= _direction;
		}

		TIKI_FORCE_INLINE Vector2 getEnd( float length ) const
		{
			TIKI_ASSERT( vector::isNormalized( direction ) );

			Vector2 offset = direction;
			vector::scale( offset, length );

			return vector::add( offset, origin, offset );
		}
	};
}

#pragma once

#include "tiki/math/vector.hpp"

namespace tiki
{
	struct Line3
	{
		Vector3 start;
		Vector3 end;

		TIKI_FORCE_INLINE Line3()
		{
		}

		TIKI_FORCE_INLINE Line3( const Vector3& _start, const Vector3& _end )
		{
			start	= _start;
			end		= _end;
		}

		TIKI_FORCE_INLINE Vector3 getDistance() const
		{
			Vector3 distance;
			vector::sub( distance, end, start );

			return distance;
		}

		TIKI_FORCE_INLINE Vector3 getDirection() const
		{
			Vector3 direction = getDistance();
			vector::normalize( direction );

			return direction;
		}

		TIKI_FORCE_INLINE float getLength() const
		{
			return vector::length( getDistance() );
		}
	};
}

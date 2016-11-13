#pragma once
#ifndef TIKI_LINE2_HPP_INCLUDED
#define TIKI_LINE2_HPP_INCLUDED

#include "tiki/math/vector.hpp"

namespace tiki
{
	struct Line2
	{
		Vector2 start;
		Vector2 end;

		TIKI_FORCE_INLINE Line2()
		{
		}

		TIKI_FORCE_INLINE Line2( const Vector2& _start, const Vector2& _end )
		{
			start	= _start;
			end		= _end;
		}

		TIKI_FORCE_INLINE Vector2 getDistance() const
		{
			Vector2 distance;
			vector::sub( distance, end, start );

			return distance;
		}

		TIKI_FORCE_INLINE Vector2 getDirection() const
		{
			Vector2 direction = getDistance();
			vector::normalize( direction );

			return direction;
		}

		TIKI_FORCE_INLINE float getLength() const
		{
			return vector::length( getDistance() );
		}
	};
}

#endif // TIKI_LINE2_HPP_INCLUDED

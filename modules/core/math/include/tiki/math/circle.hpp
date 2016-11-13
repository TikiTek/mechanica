#pragma once
#ifndef TIKI_CIRCLE_HPP_INCLUDED
#define TIKI_CIRCLE_HPP_INCLUDED

#include "tiki/math/vector.hpp"

namespace tiki
{
	struct Circle
	{
		Vector2	center;
		float	radius;

		TIKI_FORCE_INLINE Circle()
		{
		}

		TIKI_FORCE_INLINE Circle( const Vector2& _center, float _radius )
		{
			center	= _center;
			radius	= _radius;
		}

		TIKI_FORCE_INLINE Vector2 getPoint( float angle )
		{
			Vector2 point = getPointWithoutCenter( angle );
			vector::add( point, center );

			return point;
		}

		TIKI_FORCE_INLINE Vector2 getPointWithoutCenter( float angle )
		{
			Vector2 point;
			vector::set( point, f32::sin( angle ), f32::cos( angle ) );
			vector::scale( point, radius );

			return point;
		}
	};
}

#endif // TIKI_CIRCLE_HPP_INCLUDED

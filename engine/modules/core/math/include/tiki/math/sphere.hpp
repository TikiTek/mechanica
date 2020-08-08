#pragma once

#include "tiki/math/vector.hpp"

namespace tiki
{
	struct Sphere
	{
		Vector3 center;
		float	radius;

		TIKI_FORCE_INLINE Sphere()
		{
		}

		TIKI_FORCE_INLINE Sphere( const Vector3& _center, float _radius )
		{
			center = _center;
			radius = _radius;
		}

		TIKI_FORCE_INLINE void translate( const Vector3& translation )
		{
			vector::add( center, translation );
		}
	};
}

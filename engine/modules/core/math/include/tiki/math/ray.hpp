#pragma once
#ifndef TIKI_RAY_HPP_INCLUDED
#define TIKI_RAY_HPP_INCLUDED

#include "tiki/math/vector.hpp"

namespace tiki
{
	struct Ray
	{
		Vector3 origin;
		Vector3 direction;

		TIKI_FORCE_INLINE Ray()
		{
		}

		TIKI_FORCE_INLINE Ray( const Vector3& _origin, const Vector3& _direction )
		{
			TIKI_ASSERT( vector::isNormalized( _direction ) );

			origin		= _origin;
			direction	= _direction;
		}
	};
}

#endif // TIKI_RAY_HPP_INCLUDED

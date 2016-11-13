#pragma once
#ifndef TIKI_RAY3_HPP_INCLUDED
#define TIKI_RAY3_HPP_INCLUDED

#include "tiki/math/vector.hpp"

namespace tiki
{
	struct Ray3
	{
		Vector3 origin;
		Vector3 direction;

		TIKI_FORCE_INLINE Ray3()
		{
		}

		TIKI_FORCE_INLINE Ray3( const Vector3& _origin, const Vector3& _direction )
		{
			TIKI_ASSERT( vector::isNormalized( _direction ) );

			origin		= _origin;
			direction	= _direction;
		}

		TIKI_FORCE_INLINE Vector3 getEnd( float length ) const
		{
			TIKI_ASSERT( vector::isNormalized( direction ) );

			Vector3 offset = direction;
			vector::scale( offset, length );

			return vector::add( offset, origin, offset );
		}
	};
}

#endif // TIKI_RAY3_HPP_INCLUDED

#ifndef TIKI_MATHFUNCTIONS_HPP__
#define TIKI_MATHFUNCTIONS_HPP__

#include "tiki/base/types.hpp"
#include "tiki/base/inline.hpp"

namespace tiki
{
	namespace math
	{
		TIKI_FORCE_INLINE float rsqrt( float x )
		{
			float xhalf = x * 0.5f;
			int i = *(int*) &x;
			i = 0x5f3759df - ( i >> 1 );
			x = *(float*) &i;
			x = x * ( 1.5f - xhalf * x * x );
			return x;
		}

		TIKI_FORCE_INLINE float sqrt( float x )
		{
			return x * rsqrt( x );
		}
	}
}

#endif // TIKI_MATHFUNCTIONS_HPP__

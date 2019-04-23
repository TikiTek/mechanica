#pragma once

#include "tiki/base/types.hpp"
#include "tiki/base/number_limits.hpp"

#include <math.h>
#include <stdlib.h>

namespace tiki
{
	namespace f64
	{
		static const float64 epsilon		= 1.192092896e-07;
		static const float64 zeroTolerance	= 1.0e-20;
		static const float64 pi				= 3.1415926535897932384626433832795;
		static const float64 twoPi			= pi * 2.0;
		static const float64 piOver2		= pi / 2.0;
		static const float64 piOver4		= pi / 4.0;

		TIKI_FORCE_INLINE float64 abs( float64 a )
		{
			return -a > a ? -a : a;
		}

		TIKI_FORCE_INLINE bool isZero( float64 a, float64 epsilon2 = f64::zeroTolerance )
		{
			return f64::abs( a ) < epsilon2;
		}

		TIKI_FORCE_INLINE bool isEquals( float64 a, float64 b, float64 epsilon2 = f64::epsilon )
		{
			return f64::abs( a - b ) < epsilon2;
		}

		TIKI_FORCE_INLINE float64 pow( float64 value, float64 power )
		{
			return ::pow( value, power );
		}

		TIKI_FORCE_INLINE float64 sin( float64 value )
		{
			return ::sin( value );
		}

		TIKI_FORCE_INLINE float64 cos( float64 value )
		{
			return ::cos( value );
		}

		TIKI_FORCE_INLINE float64 acos( float64 value )
		{
			return ::acos( value );
		}

		TIKI_FORCE_INLINE float64 atan2( float64 y, float64 x )
		{
			return ::atan2( y, x );
		}

		TIKI_FORCE_INLINE float64 lerp( float64 from, float64 to, float64 position )
		{
			return from + ( to - from ) * position;
		}

		TIKI_FORCE_INLINE float64 sign( float64 val )
		{
			return ( val < 0.0f ? -1.0f : ( val > 0.0f ? 1.0f : 0.0f ) );
		}

		TIKI_FORCE_INLINE float64 clamp( float64 value, float64 min, float64 max )
		{
			return ( value < min ? min : ( value > max ? max : value ) );
		}

		TIKI_FORCE_INLINE float64 rsqrt( float64 x )
		{
			return 1.0f / sqrt( x );
		}

		TIKI_FORCE_INLINE float64 sqrt( float64 x )
		{
			return ::sqrt( x );
		}

		TIKI_FORCE_INLINE float64 mod( float64 value, float64 divisor )
		{
			return ::fmod( value, divisor );
		}

		TIKI_FORCE_INLINE float64 floor( float64 value )
		{
			return ::floor( value );
		}

		TIKI_FORCE_INLINE float64 ceil( float64 value )
		{
			return ::ceil( value );
		}

		// return random value between 0 and 1
		TIKI_FORCE_INLINE float64 random()
		{
			return float64( rand() ) / RAND_MAX;
		}

		TIKI_FORCE_INLINE float64 random( float64 min, float64 max )
		{
			return min + ( random() * ( max - min ) );
		}
	}
}

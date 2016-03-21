#pragma once
#ifndef TIKI_FLOAT_HPP
#define TIKI_FLOAT_HPP

#include "tiki/base/types.hpp"

#include <math.h>
#include <stdlib.h>

namespace tiki
{
	namespace f32
	{
		static const float epsilon			= 1.192092896e-07f;
		static const float pi				= 3.141592653f;
		static const float twoPi			= 6.283185307f;
		static const float piOver2			= 1.570796326f;
		static const float piOver4			= 0.785398163f;
		static const float minValue			= 1.175494351e-38f;
		static const float maxValue			= 3.402823466e+38f;
		static const float zeroTolerance	= 1e-06f;

		TIKI_FORCE_INLINE float	abs( float a )
		{
			return -a > a ? -a : a;
		}

		TIKI_FORCE_INLINE bool	isZero( float a, float epsilon2 = 1.0e-20f )
		{
			return f32::abs( a ) < epsilon2;
		}

		TIKI_FORCE_INLINE bool	isEquals( float a, float b, float epsilon2 = f32::epsilon )
		{
			return f32::abs( a - b ) < epsilon2;
		}

		TIKI_FORCE_INLINE float pow( float value, float power )
		{
			return powf( value, power );
		}

		TIKI_FORCE_INLINE float sin( float value )
		{
			return sinf( value );
		}

		TIKI_FORCE_INLINE float cos( float value )
		{
			return cosf( value );
		}

		TIKI_FORCE_INLINE float acos( float value )
		{
			return acosf( value );
		}

		TIKI_FORCE_INLINE float lerp( float from, float to, float position )
		{
			return from + ( to - from ) * position;
		}

		TIKI_FORCE_INLINE float sign( float val )
		{
			return ( val < 0.0f ? -1.0f : ( val > 0.0f ? 1.0f : 0.0f ) );
		}

		TIKI_FORCE_INLINE float clamp( float value, float min, float max )
		{
			return ( value < min ? min : ( value > max ? max : value ) );
		}

		// approximated 1.0f / sqrt( x )
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

		TIKI_FORCE_INLINE float mod( float value, float divisor )
		{
			return (float)fmod( value, divisor );
		}

		// return random value between 0 and 1
		TIKI_FORCE_INLINE float random()
		{
			return float( rand() ) / RAND_MAX;
		}

		TIKI_FORCE_INLINE float random( float min, float max )
		{
			return min + ( random() * ( max - min ) );
		}
	}
}

#endif // TIKI_FLOAT_HPP

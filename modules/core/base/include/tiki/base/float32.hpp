#pragma once
#ifndef TIKI_FLOAT_HPP
#define TIKI_FLOAT_HPP

#include "tiki/base/types.hpp"
#include "tiki/base/inline.hpp"

#include <math.h>

namespace tiki
{
	namespace f32
	{
		static const float epsilon	= 4.37114e-08f;
		static const float pi		= 3.141592653f;
		static const float twoPi	= 6.283185307f;
		static const float piOver2	= 1.570796326f;
		static const float piOver4	= 0.785398163f;

		TIKI_FORCE_INLINE float	abs( float a )
		{
			return -a > a ? -a : a;
		}

		TIKI_FORCE_INLINE bool	isZero( float a, float epsilon2 = f32::epsilon )
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
	}
}

#endif // TIKI_FLOAT_HPP

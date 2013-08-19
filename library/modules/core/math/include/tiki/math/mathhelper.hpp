#ifndef TIKI_MATHHELPER_HPP__
#define TIKI_MATHHELPER_HPP__

#include "tiki/base/types.hpp"
#include "tiki/base/alignment.hpp"
#include "tiki/base/assert.hpp"
#include "tiki/base/inline.hpp"
#include "tiki/base/float32.hpp"
#include "tiki/base/functions.hpp"

namespace tiki
{
	template<typename T>
	TIKI_FORCE_INLINE T lerp( const T& from, const T& to, const T& val )
	{
		return from + ( to - from ) * val;
	}

	template<typename T>
	TIKI_FORCE_INLINE T clamp( const T& val, const T& min, const T& max )
	{
		if( val < min )
			return min;
		if( val > max )
			return max;
		return val;
	}

	template<typename T>
	TIKI_FORCE_INLINE void tikiSwap( T& val1, T& val2 )
	{
		const T tmp = val1;
		val1 = val2;
		val2 = tmp;
	}

	template<typename T>
	TIKI_FORCE_INLINE T snap( const T& val )
	{
		return float( int( val ) );
	}

	template<typename T>
	TIKI_FORCE_INLINE bool isZero( const T& val )
	{
		return val == 0.0f || val == -0.0f;
	}

	//template<typename T>
	//TIKI_FORCE_INLINE T min( const T& a, const T& b )
	//{
	//	return a < b ? a : b;
	//}

	//template<typename T>
	//TIKI_FORCE_INLINE T max( const T& a, const T& b )
	//{
	//	return a > b ? a : b;
	//}

	TIKI_FORCE_INLINE float toDegrees( const float radians )
	{
		return radians * 57.2957795f;
	}

	TIKI_FORCE_INLINE float toRadians( const float degrees )
	{
		return degrees * 0.0174532925f;
	}

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

#endif // TIKI_MATHHELPER_HPP__

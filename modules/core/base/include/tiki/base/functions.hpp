#pragma once
#ifndef TIKI_BASE_FUNCTIONS_HPP
#define TIKI_BASE_FUNCTIONS_HPP

#include "tiki/base/types.hpp"
#include "tiki/base/numberlimits.hpp"

#if TIKI_ENABLED( TIKI_BUILD_MSVC ) && TIKI_ENABLED( TIKI_BUILD_64BIT )
#	include <intrin.h>
#endif

namespace tiki
{
	template<typename T>
	TIKI_FORCE_INLINE T* addPointer( T* pPtr, uint byteCountToAdd )
	{
		return static_cast< T* >( (void*)( (uint8*)pPtr + byteCountToAdd ) );
	}

	template<typename T>
	TIKI_FORCE_INLINE const T* addPointer( const T* pPtr, uint byteCountToAdd )
	{
		return static_cast< const T* >( (const void*)( (const uint8*)pPtr + byteCountToAdd ) );
	}

	template<typename T>
	TIKI_FORCE_INLINE T* addPointerCast( void* pPtr, uint byteCountToAdd )
	{
		return static_cast< T* >( static_cast< void* >( static_cast< uint8* >( pPtr ) + byteCountToAdd ) );
	}

	template<typename T>
	TIKI_FORCE_INLINE const T* addPointerCast( const void* pPtr, uint byteCountToAdd )
	{
		return static_cast< const T* >( static_cast< const void* >( static_cast< const uint8* >( pPtr ) + byteCountToAdd ) );
	}

	TIKI_FORCE_INLINE bool isBitSet( uint32 bitMask, uint32 flag )
	{
		return ( bitMask & flag ) == flag;
	}

	TIKI_FORCE_INLINE bool isBitSet64( uint64 bitMask, uint64 flag )
	{
		return ( bitMask & flag ) == flag;
	}

	template<typename T>
	TIKI_FORCE_INLINE bool isPowerOfTwo( T x )
	{
		return ( x & ( x - 1 ) ) == 0;
	}

	template<typename T>
	TIKI_FORCE_INLINE T alignValue( T value, T alignment )
	{
		TIKI_ASSERT( alignment > 0 );
		TIKI_ASSERT( isPowerOfTwo( alignment ) );
		return ( value + alignment - 1 ) & ( 0 - alignment );
	}

	template<typename T>
	TIKI_FORCE_INLINE bool isValueAligned( T value, T alignment )
	{
		return (value % alignment) == 0;
	}
		
	TIKI_FORCE_INLINE bool isPointerAligned( const void* value, uint alignment )
	{
		return (uint( value ) % alignment) == 0;
	}

	template<typename T>
	TIKI_FORCE_INLINE T* alignPointer( T* pPtr, uint alignment )
	{
		return (T*)alignValue( (uint)pPtr, alignment );
	}

	template<typename T>
	TIKI_FORCE_INLINE const T* alignPointer( const T* pPtr, uint alignment )
	{
		return (const T*)alignValue( (uint)pPtr, alignment );
	}

	TIKI_FORCE_INLINE uint countPopulation64( uint64 w )
	{
		uint64 w1 = (w & 0x2222222222222222) + ((w+w) & 0x2222222222222222);
		uint64 w2 = (w >> 1 & 0x2222222222222222) + (w >> 2 & 0x2222222222222222);
		w1 = w1 + (w1 >> 4) & 0x0f0f0f0f0f0f0f0f;
		w2 = w2 + (w2 >> 4) & 0x0f0f0f0f0f0f0f0f;
		return (w1 + w2) * 0x0101010101010101 >> 57;
	}

	TIKI_FORCE_INLINE uint countLeadingZeros64( uint64 value )
	{
#if TIKI_ENABLED( TIKI_BUILD_MSVC ) && TIKI_ENABLED( TIKI_BUILD_64BIT )
		unsigned long result = 0u;
		if( _BitScanReverse64( &result, value ) )
		{
			return 63u - result;
		}
		else
		{
			return 64u;
		}
#else
		register uint64 x = value;
		x |= (x >> 1);
		x |= (x >> 2);
		x |= (x >> 4);
		x |= (x >> 8);
		x |= (x >> 16);
		x |= (x >> 32);
		return ( 64u - countPopulation64( x ) );
#endif
	}

	TIKI_FORCE_INLINE uint clamp( uint value, uint min, uint max )
	{
		return ( value < min ? min : value > max ? max : value );
	}

	template<typename T>
	TIKI_FORCE_INLINE T getNextPowerOfTwo( T value )
	{
		const T shift = (T)( 64u - countLeadingZeros64( value - 1 ) );
		return T( 1 ) << shift;
	}

	template<typename TTarget, typename TSource>
	TIKI_FORCE_INLINE bool rangeCheckCast( TTarget& outValue, const TSource& value )
	{
		if ( (TTarget)value < NumberLimits< TTarget >::lowest() )
		{
			return false;
		}
		else if ( (TTarget)value > NumberLimits< TTarget >::max() )
		{
			return false;
		}

		outValue = (TTarget)value;
		return true;
	}

	template<typename T>
	TIKI_FORCE_INLINE void swap( T& value1, T& value2 )
	{
		T backup = value1;
		value1 = value2;
		value2 = backup;
	}
}

#endif // TIKI_BASE_FUNCTIONS_HPP

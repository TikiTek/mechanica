#pragma once
#ifndef TIKI_BASE_FUNCTIONS_HPP
#define TIKI_BASE_FUNCTIONS_HPP

#include "tiki/base/inline.hpp"

#define TIKI_MIN( a, b ) ( a < b ? a : b )
#define TIKI_MAX( a, b ) ( a > b ? a : b )

namespace tiki
{
	template<typename T>
	TIKI_FORCE_INLINE T alignValue( T value, T alignment )
	{
		TIKI_ASSERT( isPowerOfTwo( alignment ) );
		return ( value + alignment - 1 ) & ( 0 - alignment );
	}

	template<typename T>
	TIKI_FORCE_INLINE bool isValueAligned( T value, T alignment )
	{
		return alignValue( value, alignment ) == value;
	}

	template<typename T>
	TIKI_FORCE_INLINE T nextPowerOfTwo( T value )
	{
		const T shift = (T)( 64u - countLeadingZeros64( value - 1 ) );
		return 1 << shift;
	}

	template<typename T>
	TIKI_FORCE_INLINE T* alignPtr( T* pPtr, size_t alignment )
	{
		return (T*)alignValue( (size_t)pPtr, alignment );
	}

	template<typename T>
	TIKI_FORCE_INLINE const T* alignPtr( const T* pPtr, size_t alignment )
	{
		return (const T*)alignValue( (size_t)pPtr, alignment );
	}

	template<typename T>
	TIKI_FORCE_INLINE T* addPtr( T* pPtr, size_t byteCountToAdd )
	{
		return static_cast< T* >( (void*)( (uint8*)pPtr + byteCountToAdd ) );
	}

	template<typename T>
	TIKI_FORCE_INLINE const T* addPtr( const T* pPtr, size_t byteCountToAdd )
	{
		return static_cast< const T* >( (const void*)( (const uint8*)pPtr + byteCountToAdd ) );
	}

	template<typename T>
	TIKI_FORCE_INLINE T* addPtrCast( void* pPtr, size_t byteCountToAdd )
	{
		return static_cast< T* >( static_cast< void* >( static_cast< uint8* >( pPtr ) + byteCountToAdd ) );
	}

	template<typename T>
	TIKI_FORCE_INLINE const T* addPtrCast( const void* pPtr, size_t byteCountToAdd )
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

	TIKI_FORCE_INLINE size_t countPopulation64( uint64 w )
	{
		uint64 w1 = (w & 0x2222222222222222) + ((w+w) & 0x2222222222222222);
		uint64 w2 = (w >> 1 & 0x2222222222222222) + (w >> 2 & 0x2222222222222222);
		w1 = w1 + (w1 >> 4) & 0x0f0f0f0f0f0f0f0f;
		w2 = w2 + (w2 >> 4) & 0x0f0f0f0f0f0f0f0f;
		return (w1 + w2) * 0x0101010101010101 >> 57;
	}

	TIKI_FORCE_INLINE size_t countLeadingZeros64( uint64 value )
	{
		register uint64 x = value;
		x |= (x >> 1);
		x |= (x >> 2);
		x |= (x >> 4);
		x |= (x >> 8);
		x |= (x >> 16);
		x |= (x >> 32);
		return ( 64u - countPopulation64( x ) );
	}

}

#endif // TIKI_BASE_FUNCTIONS_HPP

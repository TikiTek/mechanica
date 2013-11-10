#pragma once
#ifndef __TIKI_BITS_INL_INCLUDED__
#define __TIKI_BITS_INL_INCLUDED__

namespace tiki
{
	TIKI_FORCE_INLINE uint32 clearBits( uint32 sourceValue, uint32 startBit, uint32 bitCount )
	{
		const uint32 maxValue = ( 1u << bitCount ) - 1u;
		const uint32 andMask = ~( maxValue << startBit );

		return sourceValue & andMask;
	}

	TIKI_FORCE_INLINE uint32 setBitValue( uint32 sourceValue, uint32 startBit, uint32 bitCount, uint32 value )
	{
		TIKI_ASSERT( value < ( 1u << bitCount ) );
		
		const uint32 orMask = value << startBit;

		return sourceValue | orMask;
	}

	TIKI_FORCE_INLINE uint32 replaceBitValue( uint32 sourceValue, uint32 startBit, uint32 bitCount, uint32 value )
	{
		sourceValue = clearBits( sourceValue, startBit, bitCount );
		return setBitValue( sourceValue, startBit, bitCount, value );
	}
}

#endif // __TIKI_BITS_INL_INCLUDED__

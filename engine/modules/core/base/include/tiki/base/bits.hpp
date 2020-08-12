#pragma once
#ifndef __TIKI_BITS_HPP_INCLUDED__
#define __TIKI_BITS_HPP_INCLUDED__

#include "tiki/base/types.hpp"

namespace tiki
{
	TIKI_FORCE_INLINE uint32 clearBits( uint32 sourceValue, uint32 startBit, uint32 bitCount );
	TIKI_FORCE_INLINE uint32 getBitValue( uint32 sourceValue, uint32 startBit, uint32 bitCount );
	TIKI_FORCE_INLINE uint32 setBitValue( uint32 sourceValue, uint32 startBit, uint32 bitCount, uint32 value );
	TIKI_FORCE_INLINE uint32 replaceBitValue( uint32 sourceValue, uint32 startBit, uint32 bitCount, uint32 value );
}

#include "../../../source/bits.inl"

#endif // __TIKI_BITS_HPP_INCLUDED__

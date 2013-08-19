#ifndef TIKI_BITS_HPP__
#define TIKI_BITS_HPP__

#include "tiki/base/types.hpp"
#include "tiki/base/inline.hpp"

namespace tiki
{
	TIKI_FORCE_INLINE bool isBitSet( uint32 bitMask, uint32 flag )
	{
		return ( bitMask & flag ) == flag;
	}

	TIKI_FORCE_INLINE bool isBitSet64( uint64 bitMask, uint64 flag )
	{
		return ( bitMask & flag ) == flag;
	}
}

#endif // TIKI_BITS_HPP__

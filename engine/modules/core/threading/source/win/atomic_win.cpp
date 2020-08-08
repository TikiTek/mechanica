#include "tiki/threading/atomic.hpp"

#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
#	include <xatomic.h>
#endif

#include <atomic>

namespace tiki
{
	/*static*/ uint32 AtomicAccesorTraits< uint32_atomic >::loadRelaxed( const uint32_atomic* pValue )
	{
		return __iso_volatile_load32( (const volatile __int32*)pValue );
	}

	/*static*/ uint32 AtomicAccesorTraits< uint32_atomic >::loadAquire( const uint32_atomic* pValue )
	{
		const __int32 value = __iso_volatile_load32( (const volatile __int32*)pValue );
		_ReadWriteBarrier();
		return (uint32)value;
	}

	/*static*/ uint32 AtomicAccesorTraits< uint32_atomic >::loadOrdered( const uint32_atomic* pValue )
	{
		const __int32 value = __iso_volatile_load32( (const volatile __int32*)pValue );
		_ReadWriteBarrier();
		return (uint32)value;
	}

	/*static*/ void AtomicAccesorTraits< uint32_atomic >::storeRelaxed( uint32_atomic* pValue, uint32 newValue )
	{
		_InterlockedExchange( (volatile long*)pValue, newValue );
	}

	/*static*/ void AtomicAccesorTraits< uint32_atomic >::storeRelease( uint32_atomic* pValue, uint32 newValue )
	{
		_InterlockedExchange( (volatile long*)pValue, newValue );
	}

	/*static*/ void AtomicAccesorTraits< uint32_atomic >::storeOrdered( uint32_atomic* pValue, uint32 newValue )
	{
		_InterlockedExchange( (volatile long*)pValue, newValue );
	}

	/*static*/ uint32 AtomicAccesorTraits< uint32_atomic >::orRelaxed( uint32_atomic* pValue, uint32 mask )
	{
		return _InterlockedOr( (volatile long*)pValue, mask );
	}

	/*static*/ uint32 AtomicAccesorTraits< uint32_atomic >::orRelease( uint32_atomic* pValue, uint32 mask )
	{
		return _InterlockedOr( (volatile long*)pValue, mask );
	}

	/*static*/ uint32 AtomicAccesorTraits< uint32_atomic >::orOrdered( uint32_atomic* pValue, uint32 mask )
	{
		return _InterlockedOr( (volatile long*)pValue, mask );
	}

	/*static*/ uint32 AtomicAccesorTraits< uint32_atomic >::andRelaxed( uint32_atomic* pValue, uint32 mask )
	{
		return _InterlockedAnd( (volatile long*)pValue, mask );
	}

	/*static*/ uint32 AtomicAccesorTraits< uint32_atomic >::andRelease( uint32_atomic* pValue, uint32 mask )
	{
		return _InterlockedAnd( (volatile long*)pValue, mask );
	}

	/*static*/ uint32 AtomicAccesorTraits< uint32_atomic >::andOrdered( uint32_atomic* pValue, uint32 mask )
	{
		return _InterlockedAnd( (volatile long*)pValue, mask );
	}
}
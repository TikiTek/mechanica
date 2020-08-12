#pragma once
#ifndef __TIKI_POOLALLOCATOR_INL_INCLUDED__
#define __TIKI_POOLALLOCATOR_INL_INCLUDED__

#include "tiki/base/assert.hpp"

namespace tiki
{
	template<typename T>
	TIKI_FORCE_INLINE PoolAllocator< T >::PoolAllocator()
	{
		m_pPool			= nullptr;
		m_count			= 0u;

		m_pUsageBitmask	= nullptr;
	}

	template<typename T>
	TIKI_FORCE_INLINE PoolAllocator< T >::~PoolAllocator()
	{
		TIKI_ASSERT( m_pPool == nullptr );
	}

	template<typename T>
	TIKI_FORCE_INLINE bool PoolAllocator< T >::create( uint count, uint alignment /* = TIKI_DEFAULT_ALIGNMENT */ )
	{
		const uint usageCount = alignValue( count, 64u ) / 64u;

		m_pPool			= static_cast< T* >( memory::allocAlign( count * sizeof( T ), alignment ) );
		m_pUsageBitmask	= static_cast< uint64* >( memory::allocAlign( usageCount * sizeof( uint64 ), sizeof( uint64 ) ) );
		m_count	= count;

		if ( m_pPool == nullptr || m_pUsageBitmask == nullptr )
		{
			dispose();
			TIKI_TRACE_ERROR( "[PoolAllocator] Could not allocator Memory.\n" );
			return false;
		}

		memory::zero( m_pUsageBitmask, usageCount * sizeof( uint64 ) );

		return true;
	}

	template<typename T>
	TIKI_FORCE_INLINE void PoolAllocator< T >::dispose()
	{
		if ( m_pPool != nullptr )
		{
			memory::freeAlign( m_pPool );
			m_pPool = nullptr;
		}

		if ( m_pUsageBitmask != nullptr )
		{
			memory::freeAlign( m_pUsageBitmask );
			m_pUsageBitmask = nullptr;
		}

		m_count = 0u;
	}

	template<typename T>
	TIKI_FORCE_INLINE T* PoolAllocator< T >::allocate()
	{
		TIKI_ASSERT( m_pPool != nullptr );

		uint usageIndex = 0u;
		while ( countLeadingZeros64( m_pUsageBitmask[ usageIndex ] ) == 0u )
		{
			usageIndex++;
		}
		const uint maskIndex = 64u - countLeadingZeros64( m_pUsageBitmask[ usageIndex ] );
		const uint finalIndex = ( usageIndex * 64u ) + maskIndex;

		if ( finalIndex >= m_count )
		{
			return nullptr;
		}
		
		m_pUsageBitmask[ usageIndex ] |= 1ull << maskIndex;

		return ::new( &m_pPool[ finalIndex ] ) T();
	}

	template<typename T>
	TIKI_FORCE_INLINE void PoolAllocator< T >::free( T* pObject )
	{
		TIKI_ASSERT( m_pPool != nullptr );

		pObject->~T();

		const uint index = pObject - m_pPool;
		TIKI_ASSERT( index < m_count );

		const uint usageIndex = index / 64u;
		const uint maskIndex = index - ( usageIndex * 64u );

		m_pUsageBitmask[ usageIndex ] &= ~( 1ull << maskIndex );
	}
}

#endif // __TIKI_POOLALLOCATOR_INL_INCLUDED__
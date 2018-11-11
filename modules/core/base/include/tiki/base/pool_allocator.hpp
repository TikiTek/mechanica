#pragma once
#ifndef __TIKI_POOLALLOCATOR_HPP_INCLUDED__
#define __TIKI_POOLALLOCATOR_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/base/inline.hpp"

namespace tiki
{
	template<typename T>
	class PoolAllocator
	{
		TIKI_NONCOPYABLE_CLASS( PoolAllocator );

	public:

		TIKI_FORCE_INLINE		PoolAllocator();
		TIKI_FORCE_INLINE		~PoolAllocator();

		TIKI_FORCE_INLINE bool	create( uint count, uint alignment = TIKI_DEFAULT_ALIGNMENT );
		TIKI_FORCE_INLINE void	dispose();

		TIKI_FORCE_INLINE bool	isCreated() const { return m_pPool != nullptr; }

		TIKI_FORCE_INLINE T*	allocate();
		TIKI_FORCE_INLINE void	free( T* pObject );

	private:

		T*		m_pPool;
		uint	m_count;

		uint64*	m_pUsageBitmask;

	};
}

#include "../../../source/poolallocator.inl"

#endif // __TIKI_POOLALLOCATOR_HPP_INCLUDED__

#pragma once
#ifndef __TIKI_POOLALLOCATOR_HPP_INCLUDED__
#define __TIKI_POOLALLOCATOR_HPP_INCLUDED__

#include "tiki/base/types.hpp"

namespace tiki
{
	template<typename T>
	class PoolAllocator
	{
		TIKI_NONCOPYABLE_CLASS( PoolAllocator );

	public:

		PoolAllocator();
		~PoolAllocator();

		bool		create( uint count, uint alignment = TIKI_DEFAULT_ALIGNMENT );
		void		dispose();

		T*			allocate();
		void		free( T* pObject );

	private:

		T*			m_pPool;
		uint		m_count;

		uint64*		m_pUsageBitmask;

	};
}

#include "../../../source/poolallocator.inl"

#endif // __TIKI_POOLALLOCATOR_HPP_INCLUDED__

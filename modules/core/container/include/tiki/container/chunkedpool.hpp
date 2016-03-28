#pragma once
#ifndef TIKI_CHUNKEDPOOL_HPP_INCLUDED
#define TIKI_CHUNKEDPOOL_HPP_INCLUDED

#include "tiki/base/assert.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/linkedlist.hpp"

namespace tiki
{
	template<typename T>
	class ChunkedPool
	{
		TIKI_NONCOPYABLE_CLASS( ChunkedPool );

	public:

		typedef T							Type;
		typedef T&							Reference;
		typedef const T&					ConstReference;

		TIKI_FORCE_INLINE					ChunkedPool();
		TIKI_FORCE_INLINE					~ChunkedPool();

		TIKI_FORCE_INLINE bool				create( uint grow );
		TIKI_FORCE_INLINE void				dispose();

		TIKI_FORCE_INLINE uint				getCount() const;
		TIKI_FORCE_INLINE bool				isEmpty() const;

		TIKI_FORCE_INLINE bool				contains( const T& item ) const;

		TIKI_FORCE_INLINE T*				push();
		TIKI_FORCE_INLINE T*				push( const T& item );

		TIKI_FORCE_INLINE void				removeUnsortedByValue( const T& item );

	private:

		struct PoolChunk : LinkedItem< PoolChunk >
		{
			T*						pData;
		};

		struct FreeElement
		{
			FreeElement*			pNextFree;
		};

		LinkedList< PoolChunk >		m_chunks;

		FreeElement*				m_pFirstFree;
		FreeElement*				m_pLastFree;

		uint						m_count;
		uint						m_freeCount;
		uint						m_grow;

		TIKI_FORCE_INLINE bool		allocateChunk();

	};
}

#include "../../../source/chunkedpool.inl"

#endif // TIKI_CHUNKEDPOOL_HPP_INCLUDED

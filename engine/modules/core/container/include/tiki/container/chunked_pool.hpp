#pragma once

#include "tiki/base/assert.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/linked_list.hpp"

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

		TIKI_FORCE_INLINE bool				create( uint chunkSize );
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
			T						data[ 1u ];
		};

		struct FreeElement
		{
			FreeElement*			pNextFree;
		};

		LinkedList< PoolChunk >		m_chunks;

		FreeElement*				m_pFirstFree;

		uint						m_count;
		uint						m_chunkSize;

		TIKI_FORCE_INLINE bool		allocateChunk();

	};
}

#include "../../../source/chunked_pool.inl"

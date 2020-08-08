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
		typedef T*							Pointer;
		typedef const T*					ConstPointer;

		TIKI_FORCE_INLINE					ChunkedPool();
		TIKI_FORCE_INLINE					~ChunkedPool();

		TIKI_FORCE_INLINE bool				create( uint chunkSize );
		TIKI_FORCE_INLINE void				dispose();

		TIKI_FORCE_INLINE uint				getCount() const;
		TIKI_FORCE_INLINE bool				isEmpty() const;

		TIKI_FORCE_INLINE bool				contains( ConstReference item ) const;

		TIKI_FORCE_INLINE Pointer			push();
		TIKI_FORCE_INLINE Pointer			push( const T& item );

		TIKI_FORCE_INLINE void				removeUnsortedByValue( ConstReference item );
		TIKI_FORCE_INLINE void				removeUnsortedByValue( ConstPointer pItem );

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

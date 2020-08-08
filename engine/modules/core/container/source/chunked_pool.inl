#pragma once

#include "tiki/base/types.hpp"
#include "tiki/base/memory.hpp"

namespace tiki
{
	template<typename T>
	TIKI_FORCE_INLINE ChunkedPool<T>::ChunkedPool()
	{
		m_pFirstFree	= nullptr;
		m_count			= 0u;
		m_chunkSize		= 0u;
	}

	template<typename T>
	TIKI_FORCE_INLINE ChunkedPool<T>::~ChunkedPool()
	{
	}

	template<typename T>
	TIKI_FORCE_INLINE bool ChunkedPool<T>::create( uint chunkSize )
	{
		TIKI_COMPILETIME_ASSERT( sizeof( T ) >= sizeof( FreeElement ) );

		m_count		= 0u;
		m_chunkSize	= chunkSize - 1u;

		if( !allocateChunk() )
		{
			dispose();
			return false;
		}

		return true;
	}

	template<typename T>
	TIKI_FORCE_INLINE void ChunkedPool<T>::dispose()
	{
		TIKI_ASSERT( m_count == 0u );

		while( !m_chunks.isEmpty() )
		{
			TIKI_FREE( m_chunks.popFirst() );
		}

		m_pFirstFree	= nullptr;
		m_count			= 0u;
		m_chunkSize		= 0u;
	}

	template<typename T>
	TIKI_FORCE_INLINE uint ChunkedPool<T>::getCount() const
	{
		return m_count;
	}

	template<typename T>
	TIKI_FORCE_INLINE bool ChunkedPool<T>::isEmpty() const
	{
		return m_count == 0u;
	}

	template<typename T>
	TIKI_FORCE_INLINE bool ChunkedPool<T>::contains( ConstReference item ) const
	{
		for( const PoolChunk& chunk : m_chunks )
		{
			if( &item >= &chunk.data[ 0u ] && &item < &chunk.data[ m_chunkSize ] )
			{
				return true;
			}
		}

		return false;
	}

	template<typename T>
	TIKI_FORCE_INLINE T* ChunkedPool<T>::push()
	{
		if( m_pFirstFree == nullptr && !allocateChunk() )
		{
			return nullptr;
		}

		T* pElement = (T*)m_pFirstFree;
		m_pFirstFree = m_pFirstFree->pNextFree;
		m_count++;

		return new (pElement) T;
	}

	template<typename T>
	TIKI_FORCE_INLINE T* ChunkedPool<T>::push( const T& item )
	{
		T* pItem = push();
		if( pItem != nullptr )
		{
			*pItem = item;
		}
		return pItem;
	}

	template<typename T>
	TIKI_FORCE_INLINE void ChunkedPool<T>::removeUnsortedByValue( ConstReference item )
	{
		TIKI_ASSERT( contains( item ) );

		item.~T();

		FreeElement* pNewFree = (FreeElement*)&item;
		pNewFree->pNextFree = m_pFirstFree;

		m_pFirstFree = pNewFree;

		m_count--;
	}

	template<typename T>
	TIKI_FORCE_INLINE void ChunkedPool<T>::removeUnsortedByValue( ConstPointer pItem )
	{
		TIKI_ASSERT( pItem != nullptr);
		removeUnsortedByValue( *pItem );
	}

	template<typename T>
	TIKI_FORCE_INLINE bool ChunkedPool<T>::allocateChunk()
	{
		TIKI_ASSERT( m_chunkSize > 0u );

		const uint chunkSize = sizeof( PoolChunk ) + (m_chunkSize * sizeof( T ));
		PoolChunk* pChunk = (PoolChunk*)TIKI_ALLOC_ALIGNED( chunkSize, TIKI_ALIGNOF( PoolChunk ) );
		if( pChunk == nullptr )
		{
			return false;
		}

		memory::zero( *pChunk );
		m_chunks.push( pChunk );

		for( uint i = 0u; i < m_chunkSize; ++i )
		{
			FreeElement* pElement = (FreeElement*)&pChunk->data[ i ];
			pElement->pNextFree = m_pFirstFree;

			m_pFirstFree = pElement;
		}

		return true;
	}
}

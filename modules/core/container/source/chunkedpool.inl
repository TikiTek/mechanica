#pragma once
#ifndef TIKI_CHUNKEDPOOL_INL_INCLUDED
#define TIKI_CHUNKEDPOOL_INL_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/base/memory.hpp"

namespace tiki
{
	template<typename T>
	TIKI_FORCE_INLINE ChunkedPool<T>::ChunkedPool()
	{		
		m_pFirstFree	= nullptr;
		m_pLastFree		= nullptr;
		m_count			= 0u;
		m_freeCount		= 0u;
		m_grow			= 0u;
	}	

	template<typename T>
	TIKI_FORCE_INLINE ChunkedPool<T>::~ChunkedPool()
	{
	}

	template<typename T>
	TIKI_FORCE_INLINE bool ChunkedPool<T>::create( uint grow )
	{
		TIKI_COMPILETIME_ASSERT( sizeof( T ) >= sizeof( FreeElement ) );
		
		m_count		= 0u;
		m_freeCount	= 0u;
		m_grow		= grow;

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
			PoolChunk chunk = m_chunks.getFirst();
			m_chunks.removeSortedByValue( m_chunks.getFirst() );

			TIKI_MEMORY_FREE( chunk.pMemory );
		}		

		m_pData			= nullptr;
		m_pFirstFree	= nullptr;
		m_pLastFree		= nullptr;
		m_count			= 0u;
		m_freeCount		= 0u;
		m_grow			= 0u;
	}

	template<typename T>
	TIKI_FORCE_INLINE uint ChunkedPool<T>::getCount() const
	{
		return m_count;
	}

	template<typename T>
	TIKI_FORCE_INLINE bool ChunkedPool<T>::isEmpty() const
	{
		return m_count != 0u;
	}

	template<typename T>
	TIKI_FORCE_INLINE bool ChunkedPool<T>::contains( const T& item ) const
	{
		for( PoolChunk& chunk : m_chunks )
		{
			if( &item >= m_pData && &item < m_pData + m_capacity )
			{

			}
		}
		return ;
	}

	template<typename T>
	TIKI_FORCE_INLINE T* ChunkedPool<T>::push()
	{
		if( m_freeCount == 0u && !allocateChunk() )
		{
			return nullptr;
		}

		T* pElement = (T*)m_pFirstFree;
		m_pFirstFree = m_pFirstFree->pNextFree;
		m_count++;
		m_freeCount--;

		pElement = new ( pElement ) T;

		return pElement;
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
	TIKI_FORCE_INLINE void ChunkedPool<T>::removeUnsortedByValue( const T& item )
	{
		TIKI_ASSERT( contains( item ) );

		item.~T();

		FreeElement* pNewFree = (FreeElement*)&item;
		pNewFree->pNextFree = nullptr;
		
		m_pLastFree->pNextFree = pNewFree;
		m_pLastFree = pNewFree;

		m_count--;
		m_freeCount++;
	}
	
	template<typename T>
	TIKI_FORCE_INLINE bool ChunkedPool<T>::allocateChunk()
	{
		const uint chunkSize = alignValue( sizeof( PoolChunk ), TIKI_ALIGNOF( T ) );
		const uint size = chunkSize + (sizeof( T ) * m_grow);

		PoolChunk* pChunk = (PoolChunk*)TIKI_MEMORY_ALLOC( size );
		if( pChunk == nullptr )
		{
			return false;
		}
		m_chunks.push( pChunk );

		T* pData = addPtrCast< T >( pChunk, chunkSize );
		TIKI_ASSERT( isPointerAligned( pData, TIKI_ALIGNOF( T ) ) );

		m_freeCount += m_grow;		

		FreeElement* pElement = (FreeElement*)m_pData;
		for( uint i = 0u; i < m_grow; ++i )
		{
			pElement->pNextFree = (FreeElement*)(m_pData + i);
			pElement = pElement->pNextFree;
		}
		pElement->pNextFree = nullptr;

		if( m_pFirstFree == nullptr )
		{
			m_pFirstFree = (FreeElement*)m_pData;
		}
		else
		{
			m_pFirstFree->pNextFree = (FreeElement*)pData;
		}

		m_pLastFree = (FreeElement*)(pData + m_grow - 1u);
	}
}

#endif // TIKI_CHUNKEDPOOL_INL_INCLUDED

#pragma once
#ifndef TIKI_POOL_INL_INCLUDED__
#define TIKI_POOL_INL_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/base/memory.hpp"

namespace tiki
{
	template<typename T>
	TIKI_FORCE_INLINE Pool<T>::Pool()
	{
		m_pData			= nullptr;
		m_pFirstFree	= nullptr;
		m_pLastFree		= nullptr;
		m_count			= 0u;
		m_capacity		= 0u;
	}	

	template<typename T>
	TIKI_FORCE_INLINE Pool<T>::~Pool()
	{
		TIKI_ASSERT( m_pData == nullptr );
	}

	template<typename T>
	TIKI_FORCE_INLINE bool Pool<T>::create( uint capacity )
	{
		TIKI_COMPILETIME_ASSERT( sizeof( T ) >= sizeof( FreeElement ) );

		m_pData = (T*)TIKI_ALLOC( sizeof( T ) * capacity );
		if ( m_pData == nullptr )
		{
			return false;
		}

		m_capacity	= capacity;
		m_count		= 0u;

		FreeElement* pElement = (FreeElement*)m_pData;
		for (uint i = 0u; i < m_capacity; ++i)
		{
			pElement->pNextFree = (FreeElement*)(m_pData + i);
			pElement = pElement->pNextFree;
		}
		pElement->pNextFree = nullptr;

		m_pFirstFree	= (FreeElement*)m_pData;
		m_pLastFree		= (FreeElement*)(m_pData + m_capacity - 1);

		return true;
	}

	template<typename T>
	TIKI_FORCE_INLINE void Pool<T>::dispose()
	{
		TIKI_ASSERT( m_count == 0u );

		TIKI_FREE( m_pData );

		m_pData			= nullptr;
		m_pFirstFree	= nullptr;
		m_pLastFree		= nullptr;
		m_count			= 0u;
		m_capacity		= 0u;
	}

	template<typename T>
	TIKI_FORCE_INLINE uint Pool<T>::getCount() const
	{
		return m_count;
	}

	template<typename T>
	TIKI_FORCE_INLINE bool Pool<T>::isEmpty() const
	{
		return m_count != 0u;
	}

	template<typename T>
	TIKI_FORCE_INLINE bool tiki::Pool<T>::isFull() const
	{
		return m_count == m_capacity;
	}

	template<typename T>
	TIKI_FORCE_INLINE bool Pool<T>::contains( const T& item ) const
	{
		TIKI_ASSERT( uint( &item ) % sizeof( T ) == uint( m_pData ) % sizeof( T ) );
		return &item >= m_pData && &item < m_pData + m_capacity;
	}

	template<typename T>
	TIKI_FORCE_INLINE T& Pool<T>::push()
	{
		TIKI_ASSERT( m_count < m_capacity );

		T* pElement = (T*)m_pFirstFree;
		m_pFirstFree = m_pFirstFree->pNextFree;
		m_count++;

		pElement = new ( pElement ) T;

		return *pElement;
	}

	template<typename T>
	TIKI_FORCE_INLINE T& Pool<T>::push( const T& item )
	{
		TIKI_ASSERT( m_count < m_capacity );

		T* pElement = (T*)m_pFirstFree;
		m_pFirstFree = m_pFirstFree->pNextFree;
		m_count++;

		*pElement = item;

		return *pElement;
	}

	template<typename T>
	TIKI_FORCE_INLINE void Pool<T>::removeUnsortedByValue( const T& item )
	{
		TIKI_ASSERT( contains( item ) );

		item.~T();

		FreeElement* pNewFree = (FreeElement*)&item;
		pNewFree->pNextFree = nullptr;
		

		m_pLastFree->pNextFree = pNewFree;
		m_pLastFree = pNewFree;

		m_count--;
	}
}

#endif // TIKI_POOL_INL_INCLUDED__

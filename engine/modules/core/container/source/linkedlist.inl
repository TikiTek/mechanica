#pragma once
#ifndef TIKI_LINKEDLIST_INL_INCLUDED__
#define TIKI_LINKEDLIST_INL_INCLUDED__

namespace tiki
{
	template< typename T >
	TIKI_FORCE_INLINE LinkedList<T>::LinkedList()
	{
		m_pFirst = nullptr;
		m_pLast = nullptr;
		m_count	= 0u;
	}

	template< typename T >
	TIKI_FORCE_INLINE LinkedList<T>::~LinkedList()
	{
		TIKI_ASSERT( m_pFirst == nullptr );
	}

	template< typename T >
	TIKI_FORCE_INLINE void LinkedList<T>::clear()
	{
		Item* pItem = m_pFirst;
		while ( pItem != nullptr )
		{
			Item* pNext = pItem->pNextItem;

			pItem->pPrevItem	= nullptr;
			pItem->pNextItem	= nullptr;
#if TIKI_ENABLED( TIKI_USE_ASSERT )
			pItem->pList		= nullptr;
#endif

			pItem = pNext;
		}

		m_pFirst = nullptr;
		m_pLast = nullptr;
		m_count	= 0u;
	}

	template< typename T >
	TIKI_FORCE_INLINE T& LinkedList<T>::push( Reference value )
	{
		TIKI_ASSERT( value.pList == nullptr );

		if( m_pFirst == nullptr )
		{
			value.pPrevItem = nullptr;
			value.pNextItem = nullptr;
#if TIKI_ENABLED( TIKI_USE_ASSERT )
			value.pList		= this;
#endif

			m_pFirst = &value;
			m_pLast = &value;
		}
		else
		{
			TIKI_ASSERT( m_pLast != nullptr );

			value.pPrevItem = m_pLast;
			value.pNextItem = nullptr;
#if TIKI_ENABLED( TIKI_USE_ASSERT )
			value.pList		= this;
#endif

			m_pLast->pNextItem = &value;
			m_pLast = &value;
		}

		m_count++;

		return value;
	}

	template< typename T >
	TIKI_FORCE_INLINE T& LinkedList<T>::push( Pointer value )
	{
		return push( *value );
	}

	template< typename T >
	TIKI_FORCE_INLINE T* LinkedList<T>::pushRange( Pointer pData, uint count )
	{
		for (uint i = 0u; i < count; ++i)
		{
			push( pData[ i ] );
		}

		return pData;
	}

	template< typename T >
	TIKI_FORCE_INLINE T* LinkedList<T>::popFirst()
	{
		if( m_pFirst == nullptr )
		{
			return nullptr;
		}

		if( m_pFirst == m_pLast )
		{
			m_pLast = nullptr;
		}

		T* pFirst = m_pFirst;
		m_pFirst = pFirst->pNextItem;
		m_count--;
		return pFirst;
	}

	template< typename T >
	TIKI_FORCE_INLINE T* LinkedList<T>::popLast()
	{
		if( m_pLast == nullptr )
		{
			return nullptr;
		}

		if( m_pFirst == m_pLast )
		{
			m_pFirst = nullptr;
		}

		T* pLast = m_pLast;
		m_pLast = pFirst->pPrevItem;
		m_count--;
		return pLast;
	}

	template< typename T >
	TIKI_FORCE_INLINE void LinkedList<T>::removeSortedByValue( Reference value )
	{
		TIKI_ASSERT( value.pList == this );

		if ( &value == m_pFirst )
		{
			m_pFirst = value.pNextItem;
		}

		if ( &value == m_pLast )
		{
			m_pLast = value.pPrevItem;
		}

		if ( value.pPrevItem != nullptr )
		{
			value.pPrevItem->pNextItem = value.pNextItem;
		}

		if( value.pNextItem != nullptr )
		{
			value.pNextItem->pPrevItem = value.pPrevItem;
		}

		value.pPrevItem = nullptr;
		value.pNextItem = nullptr;
#if TIKI_ENABLED( TIKI_USE_ASSERT )
		value.pList		= nullptr;
#endif

		m_count--;
	}
}

#endif // TIKI_LINKEDLIST_INL_INCLUDED__

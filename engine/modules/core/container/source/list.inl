#pragma once
#ifndef TIKI_LIST_INL
#define TIKI_LIST_INL

#include "tiki/base/functions.hpp"
#include "tiki/base/memory.hpp"

namespace tiki
{
	template<typename T>
	TIKI_FORCE_INLINE List< T >::List()
	{
		m_pData		= nullptr;
		m_count		= 0u;
		m_capacity	= 0u;
	}

	template<typename T>
	TIKI_FORCE_INLINE List< T >::List( const List<T>& copy )
	{
		m_pData		= nullptr;
		m_count		= 0u;
		m_capacity	= 0u;

		*this = copy;
	}

	template<typename T>
	TIKI_FORCE_INLINE List< T >::List( const T* pData, uint count )
	{
		TIKI_VERIFY( create( pData, count ) );
	}

	template<typename T>
	TIKI_FORCE_INLINE List< T >::~List()
	{
		dispose();
	}

	template<typename T>
	TIKI_FORCE_INLINE bool List<T>::create( const T* pData, uint count )
	{
		m_capacity	= getNextSize( count );
		m_count		= count;
		m_pData		= TIKI_NEW_ARRAY( T, m_capacity, true );

		if( m_pData == nullptr )
		{
			return false;
		}

		for( uint i = 0u; i < m_count; ++i )
		{
			m_pData[ i ] = pData[ i ];
		}

		return true;
	}

	template<typename T>
	TIKI_FORCE_INLINE void List<T>::dispose()
	{
		if( m_pData != nullptr )
		{
			TIKI_DELETE_ARRAY( m_pData, m_capacity );
		}

		m_pData		= nullptr;
		m_count		= 0u;
		m_capacity	= 0u;
	}

	template<typename T>
	TIKI_FORCE_INLINE void List< T >::clear()
	{
		m_count = 0u;
	}

	template<typename T>
	TIKI_FORCE_INLINE void List< T >::reserve( const uint count )
	{
		checkArraySize( count );
	}

	template<typename T>
	TIKI_FORCE_INLINE void List<T>::resize( uint count )
	{
		checkArraySize( count );
		m_count = count;
	}

	template<typename T>
	TIKI_FORCE_INLINE uint List< T >::getCount() const
	{
		return m_count;
	}

	template<typename T>
	TIKI_FORCE_INLINE bool List<T>::isEmpty() const
	{
		return m_count == 0u;
	}

	template<typename T>
	TIKI_FORCE_INLINE sint List< T >::indexOf( const T& item ) const
	{
		sint i = 0;
		const sint len = (sint)m_count;

		while ( i < len )
		{
			if ( m_pData[ i ] == item ) return i;
			i++;
		}

		return -1;
	}

	template<typename T>
	TIKI_FORCE_INLINE bool List< T >::contains( const T& item ) const
	{
		return ( indexOf( item ) != -1 );
	}

	template<typename T>
	TIKI_FORCE_INLINE T& List< T >::add()
	{
		return pushBack();
	}

	template<typename T>
	TIKI_FORCE_INLINE T& List< T >::add( const T& item )
	{
		return pushBack( item );
	}

	template<typename T>
	TIKI_FORCE_INLINE void List< T >::addRange( const List<T>& list )
	{
		addRange( list.m_pData, list.m_count );
	}

	template<typename T>
	TIKI_FORCE_INLINE void List< T >::addRange( const T* pData, uint length )
	{
		if(length == 0)	return;
		checkArraySize( m_count + length );

		for (uint i = 0u; i < length; ++i)
		{
			m_pData[ m_count++ ] = pData[ i ];
		}
	}

	template<typename T>
	TIKI_FORCE_INLINE void List< T >::insert( uint index, const T& item)
	{
		uint i = m_count;

		m_count++;
		checkArraySize( m_count );

		while ( i > index )
		{
			m_pData[ i ] = m_pData[ i - 1u ];
			i--;
		}
		m_pData[ index ] = item;
	}


	template<typename T>
	TIKI_FORCE_INLINE T& List<T>::pushBack()
	{
		checkArraySize( m_count + 1u );
		return m_pData[ m_count++ ];
	}


	template<typename T>
	TIKI_FORCE_INLINE T& List<T>::pushBack( const T& item )
	{
		checkArraySize( m_count + 1u );
		return m_pData[ m_count++ ] = item;
	}


	template<typename T>
	TIKI_FORCE_INLINE void List<T>::popBack()
	{
		m_count--;
	}

	template<typename T>
	TIKI_FORCE_INLINE T* List<T>::getBegin()
	{
		return m_pData;
	}

	template<typename T>
	TIKI_FORCE_INLINE const T* List< T >::getBegin() const
	{
		return m_pData;
	}

	template<typename T>
	TIKI_FORCE_INLINE T* List<T>::getEnd()
	{
		return m_pData + m_count;
	}

	template<typename T>
	TIKI_FORCE_INLINE const T* List< T >::getEnd() const
	{
		return m_pData + m_count;
	}

	template<typename T>
	TIKI_FORCE_INLINE T& List<T>::getFirst()
	{
		TIKI_ASSERT( m_count > 0u );
		return *m_pData;
	}

	template<typename T>
	TIKI_FORCE_INLINE const T& List<T>::getFirst() const
	{
		TIKI_ASSERT( m_count > 0u );
		return *m_pData;
	}

	template<typename T>
	TIKI_FORCE_INLINE T& List<T>::getLast()
	{
		TIKI_ASSERT( m_count > 0u );
		return *(m_pData + (m_count - 1u));
	}

	template<typename T>
	TIKI_FORCE_INLINE const T& List<T>::getLast() const
	{
		TIKI_ASSERT( m_count > 0u );
		return *(m_pData + (m_count - 1u));
	}

	template<typename T>
	TIKI_FORCE_INLINE bool List< T >::removeSortedByValue( const T& item )
	{
		const sint index = indexOf( item );
		if ( index != -1 )
		{
			removeSortedAtIndex( index );
			return true;
		}

		return false;
	}

	template<typename T>
	TIKI_FORCE_INLINE void List< T >::removeSortedAtIndex( uint index )
	{
		TIKI_ASSERT( index < m_count );

		m_count--;

		uint i = index;
		while ( i < m_count )
		{
			T value			= m_pData[ i + 1u ];
			m_pData[ i++ ]	= value;
		}
	}

	template<typename T>
	TIKI_FORCE_INLINE T& List< T >::operator[]( uint index )
	{
		TIKI_ASSERT( index < m_count );
		return m_pData[ index ];
	}

	template<typename T>
	TIKI_FORCE_INLINE const T& List< T >::operator[]( uint index ) const
	{
		TIKI_ASSERT( index < m_count );
		return m_pData[ index ];
	}

	template<typename T>
	TIKI_FORCE_INLINE void List< T >::operator=( const List<T>& copy )
	{
		if ( m_pData != nullptr )
		{
			TIKI_DELETE_ARRAY( m_pData, m_capacity );
		}

		m_capacity		= copy.m_capacity;
		m_count			= copy.m_count;

		if ( m_count > 0u )
		{
			m_pData = TIKI_NEW_ARRAY( T, m_capacity, true );
		}
		else
		{
			m_pData		= nullptr;
			m_capacity	= 0u;
		}

		for (uint i = 0u; i < m_count; ++i)
		{
			m_pData[ i ] = copy.m_pData[ i ];
		}
	}

	template<typename T>
	TIKI_FORCE_INLINE uint List< T >::getNextSize(uint targetSize)
	{
		return getNextPowerOfTwo( TIKI_MAX( 8u, targetSize ) );
	}

	template<typename T>
	TIKI_FORCE_INLINE void List< T >::checkArraySize( uint neddedSize )
	{
		if ( neddedSize > m_capacity )
		{
			const uint capacity = getNextSize( neddedSize );
			T* pNewData = TIKI_NEW_ARRAY( T, capacity, true );

			for (uint i = 0u; i < m_count; ++i)
			{
				pNewData[ i ] = m_pData[ i ];
			}

			if ( m_pData != nullptr )
			{
				TIKI_DELETE_ARRAY( m_pData, m_capacity );
			}

			m_pData		= pNewData;
			m_capacity	= capacity;
		}
	}

}

#endif // TIKI_LIST_INL
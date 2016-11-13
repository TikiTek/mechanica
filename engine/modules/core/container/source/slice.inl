#pragma once
#ifndef TIKI_SLICE_INL_INCLUDED
#define TIKI_SLICE_INL_INCLUDED

#include "tiki/base/assert.hpp"

namespace tiki
{
	template< typename T >
	TIKI_FORCE_INLINE Slice< T >::Slice()
		: m_pData( nullptr )
		, m_pCount( nullptr )
		, m_capacity( 0u )
	{
	}

	template< typename T >
	Slice<T>::Slice( T* pData, uint* pCount, uint capacity )
		: m_pData( pData )
		, m_pCount( pCount )
		, m_capacity( capacity )
	{
	}

	template< typename T >
	TIKI_FORCE_INLINE void Slice< T >::clear()
	{
		TIKI_ASSERT( m_pData != nullptr );

		*m_pCount = 0u;
	}

	template< typename T >
	TIKI_FORCE_INLINE T& Slice< T >::push()
	{
		TIKI_ASSERT( !isFull() );
		return m_pData[ (*m_pCount)++ ];
	}

	template< typename T >
	TIKI_FORCE_INLINE T& Slice< T >::push( const T& value )
	{
		TIKI_ASSERT( !isFull() );
		return m_pData[ (*m_pCount)++ ] = value;
	}

	template< typename T >
	TIKI_FORCE_INLINE T* Slice< T >::pushRange( uint count )
	{
		TIKI_ASSERT( count >= 1u );
		TIKI_ASSERT( *m_pCount + count <= m_capacity );

		const uint currentCount = *m_pCount;
		*m_pCount += count;

		return &m_pData[ currentCount ];
	}

	template< typename T >
	TIKI_FORCE_INLINE T* Slice< T >::pushRange( const T* pData, uint count )
	{
		TIKI_ASSERT( count >= 1u );
		TIKI_ASSERT( *m_pCount + count <= m_capacity );

		const uint currentCount = *m_pCount;
		for( uint i = 0u; i < count; ++i )
		{
			m_pData[ (*m_pCount)++ ] = pData[ i ];
		}

		return &m_pData[ currentCount ];
	}

	template< typename T >
	TIKI_FORCE_INLINE bool Slice< T >::pop( T& value )
	{
		if( isEmpty() )
		{
			return false;
		}

		value = m_pData[ --(*m_pCount) ];
		return true;
	}

	template< typename T >
	TIKI_FORCE_INLINE void Slice< T >::removeUnsortedByIndex( uint index )
	{
		TIKI_ASSERT( index < getCount() );
		m_pData[ index ] = m_pData[ --(*m_pCount) ];
	}

	template< typename T >
	TIKI_FORCE_INLINE bool Slice< T >::removeUnsortedByValue( const T& value )
	{
		for( uint i = 0u; i < getCount(); ++i )
		{
			if( m_pData[ i ] == value )
			{
				removeUnsortedByIndex( i );
				return true;
			}
		}

		return false;
	}

	template< typename T >
	TIKI_FORCE_INLINE uint Slice< T >::getIndexOfIterator( const T* pValue ) const
	{
		TIKI_ASSERT( pValue >= m_pData );
		TIKI_ASSERT( pValue < getEnd() );

		return pValue - m_pData;
	}

	template< typename T >
	TIKI_FORCE_INLINE uint Slice< T >::getIndexOfValue( const T& value ) const
	{
		for( uint i = 0u; i < m_count; ++i )
		{
			if( m_pData[ i ] == value )
			{
				return i;
			}
		}

		return TIKI_SIZE_T_MAX;
	}

	template< typename T >
	TIKI_FORCE_INLINE T& Slice< T >::operator[]( uint index )
	{
		TIKI_ASSERT( index < getCount() );
		return m_pData[ index ];
	}

	template< typename T >
	TIKI_FORCE_INLINE const T& Slice< T >::operator[]( uint index ) const
	{
		TIKI_ASSERT( index < m_count );
		return m_pData[ index ];
	}

}

#endif // TIKI_SLICE_INL_INCLUDED

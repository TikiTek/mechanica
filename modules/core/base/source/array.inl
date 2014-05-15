#pragma once
#ifndef __TIKI_ARRAY_INL_INCLUDED__
#define __TIKI_ARRAY_INL_INCLUDED__

namespace tiki
{
	template< typename T >
	TIKI_FORCE_INLINE Array< T >::Array()
		: m_pData( nullptr ), m_capacity( 0u )
	{
	}

	template< typename T >
	TIKI_FORCE_INLINE Array< T >::~Array()
	{
		TIKI_ASSERT( m_pData == nullptr );
	}

	template< typename T >
	TIKI_FORCE_INLINE bool Array< T >::create( uint capacity, size_t aligment = TIKI_DEFAULT_ALIGNMENT )
	{
		TIKI_ASSERT( m_pData == nullptr );

		m_capacity	= capacity;
		m_pData		= memory::newArray< T >( capacity, aligment );

		return m_pData != nullptr;
	}

	template< typename T >
	TIKI_FORCE_INLINE bool Array< T >::create( const T* pInitData, uint capacity, size_t aligment = TIKI_DEFAULT_ALIGNMENT )
	{
		if ( create( capacity, aligment ) == false )
		{
			return false;
		}

		for (size_t i = 0u; i < capacity; ++i)
		{
			m_pData[ i ] = pInitData[ i ];
		}

		return true;
	}

	template< typename T >
	TIKI_FORCE_INLINE void Array< T >::dispose()
	{
		if ( m_pData != nullptr )
		{
			memory::deleteArray( m_pData, m_capacity );
		}

		m_capacity	= 0u;
		m_pData		= nullptr;
	}
	
	template< typename T >
	TIKI_FORCE_INLINE uint tiki::Array<T>::getIndexOf( ConstIterator pValue ) const
	{
		TIKI_ASSERT( pValue >= m_pData );
		TIKI_ASSERT( pValue < getEnd() );

		return pValue - m_pData;
	}

	template< typename T >
	TIKI_FORCE_INLINE size_t Array< T >::getIndexOf( const T& value ) const
	{
		return getIndexOf( &value );
	}

	template< typename T >
	TIKI_FORCE_INLINE const T& Array< T >::operator[]( uint index ) const
	{
		TIKI_ASSERT( index < m_capacity );
		return m_pData[ index ];
	}

	template< typename T >
	TIKI_FORCE_INLINE T& Array< T >::operator[]( uint index )
	{
		TIKI_ASSERT( index < m_capacity );
		return m_pData[ index ];
	}
}

#endif // __TIKI_ARRAY_INL_INCLUDED__

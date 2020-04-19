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
	TIKI_FORCE_INLINE bool Array< T >::create( uintreg capacity, uintreg aligment /* = TIKI_DEFAULT_ALIGNMENT */, bool constructElements /* = true */ )
	{
		TIKI_ASSERT( m_pData == nullptr );

		m_capacity	= capacity;
		m_pData		= TIKI_NEW_ARRAY_ALIGNED( T, capacity, aligment, constructElements );

		return m_pData != nullptr;
	}

	template< typename T >
	TIKI_FORCE_INLINE bool Array< T >::create( ConstIterator pInitData, uintreg capacity, uintreg aligment /*= TIKI_DEFAULT_ALIGNMENT*/, bool constructElements /* = true */ )
	{
		if ( !create( capacity, aligment, constructElements ) )
		{
			return false;
		}

		for( uintreg i = 0u; i < capacity; ++i )
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
			TIKI_DELETE_ARRAY( m_pData, m_capacity );
		}

		m_capacity	= 0u;
		m_pData		= nullptr;
	}

	template<typename T>
	TIKI_FORCE_INLINE void Array<T>::swap( Array< T >& other )
	{
		T* pDataBackup = m_pData;
		uintreg capacityBackup = m_capacity;

		m_pData		= other.m_pData;
		m_capacity	= other.m_capacity;

		other.m_pData		= pDataBackup;
		other.m_capacity	= capacityBackup;
	}

	template< typename T >
	TIKI_FORCE_INLINE uintreg Array<T>::getIndexOfIterator( ConstIterator pValue ) const
	{
		TIKI_ASSERT( pValue >= m_pData );
		TIKI_ASSERT( pValue < getEnd() );

		return pValue - m_pData;
	}

	template< typename T >
	TIKI_FORCE_INLINE uintreg Array< T >::getIndexOfValue( ConstReference value ) const
	{
		for (uintreg i = 0u; i < m_capacity; ++i)
		{
			if ( m_pData[ i ] == value )
			{
				return i;
			}
		}

		return TIKI_uintreg_MAX;
	}

	template< typename T >
	TIKI_FORCE_INLINE const T& Array< T >::operator[]( uintreg index ) const
	{
		TIKI_ASSERT( index < m_capacity );
		return m_pData[ index ];
	}

	template< typename T >
	TIKI_FORCE_INLINE T& Array< T >::operator[]( uintreg index )
	{
		TIKI_ASSERT( index < m_capacity );
		return m_pData[ index ];
	}
}

#endif // __TIKI_ARRAY_INL_INCLUDED__

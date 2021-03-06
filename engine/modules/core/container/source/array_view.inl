#pragma once

namespace tiki
{
	template< typename T >
	TIKI_FORCE_INLINE ArrayView< T >::ArrayView()
		: m_pData( nullptr )
		, m_count( 0u )
	{
	}

	template< typename T >
	ArrayView<T>::ArrayView( T* pData, uintreg count )
		: m_pData( pData )
		, m_count( count )
	{
	}

	template< typename T >
	ArrayView<T>::ArrayView( const T* pData, uintreg count )
		: m_pData( pData )
		, m_count( count )
	{
	}

	template< typename T >
	ArrayView<T>::ArrayView( ConstIterator pBegin, ConstIterator pEnd )
		: m_pData( pBegin )
		, m_count( pEnd - pBegin )
	{
	}

	template< typename T >
	TIKI_FORCE_INLINE uintreg ArrayView< T >::getIndexOfIterator( ConstIterator pValue ) const
	{
		TIKI_ASSERT( pValue >= m_pData );
		TIKI_ASSERT( pValue < getEnd() );

		return pValue - m_pData;
	}

	template< typename T >
	TIKI_FORCE_INLINE uintreg ArrayView< T >::getIndexOfValue( ConstReference value ) const
	{
		for( uintreg i = 0u; i < m_count; ++i )
		{
			if( m_pData[ i ] == value )
			{
				return i;
			}
		}

		return TIKI_SIZE_T_MAX;
	}

	template< typename T >
	TIKI_FORCE_INLINE const T& ArrayView< T >::operator[]( uintreg index ) const
	{
		TIKI_ASSERT( index < m_count );
		return m_pData[ index ];
	}
}

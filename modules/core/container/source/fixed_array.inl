#pragma once
#ifndef __TIKI_FIXEDARRAY_INL_INCLUDED__
#define __TIKI_FIXEDARRAY_INL_INCLUDED__

namespace tiki
{
	template< typename T, uintreg TCapacity >
	TIKI_FORCE_INLINE void CopyableFixedArray< T, TCapacity >::create( ConstReference initValue, uintreg count /*= TCapacity */ )
	{
		TIKI_ASSERT( count <= TCapacity );

		for (uintreg i = 0u; i < count; ++i)
		{
			m_aData[ i ] = initValue;
		}
	}
	template< typename T, uintreg TCapacity >
	TIKI_FORCE_INLINE void CopyableFixedArray< T, TCapacity >::create( ConstIterator pInitData, uintreg count )
	{
		TIKI_ASSERT( count <= TCapacity );
		for (uintreg i = 0u; i < count; ++i)
		{
			m_aData[ i ] = pInitData[ i ];
		}
	}

	template< typename T, uintreg TCapacity >
	TIKI_FORCE_INLINE uintreg tiki::CopyableFixedArray< T, TCapacity >::getIndexOfIterator( ConstIterator pValue ) const
	{
		TIKI_ASSERT( pValue >= m_aData );
		TIKI_ASSERT( pValue < getEnd() );

		return pValue - m_aData;
	}

	template< typename T, uintreg TCapacity >
	TIKI_FORCE_INLINE uintreg CopyableFixedArray< T, TCapacity >::getIndexOfValue( ConstReference value ) const
	{
		for (uintreg i = 0u; i < TCapacity; ++i)
		{
			if ( m_aData[ i ] == value )
			{
				return i;
			}
		}

		return TIKI_SIZE_T_MAX;
	}

	template< typename T, uintreg TCapacity >
	TIKI_FORCE_INLINE T& CopyableFixedArray< T, TCapacity >::operator[]( uintreg index )
	{
		TIKI_ASSERT( index < TCapacity );

		return m_aData[ index ];
	}

	template< typename T, uintreg TCapacity >
	TIKI_FORCE_INLINE const T& CopyableFixedArray< T, TCapacity >::operator[]( uintreg index ) const
	{
		TIKI_ASSERT( index < TCapacity );

		return m_aData[ index ];
	}
}

#endif // __TIKI_FIXEDARRAY_INL_INCLUDED__

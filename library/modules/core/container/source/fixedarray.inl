#pragma once
#ifndef __TIKI_FIXEDARRAY_INL_INCLUDED__
#define __TIKI_FIXEDARRAY_INL_INCLUDED__

namespace tiki
{
	template< typename T, uint TCapacity >
	TIKI_FORCE_INLINE void CopyableFixedArray< T, TCapacity >::create( ConstReference initValue, uint count /*= TCapacity */ )
	{
		TIKI_ASSERT( count <= TCapacity );

		for (uint i = 0u; i < count; ++i)
		{
			m_aData[ i ] = initValue;
		}
	}
	template< typename T, uint TCapacity >
	TIKI_FORCE_INLINE void CopyableFixedArray< T, TCapacity >::create( ConstIterator pInitData, uint count )
	{
		TIKI_ASSERT( count <= TCapacity );
		for (uint i = 0u; i < count; ++i)
		{
			m_aData[ i ] = pInitData[ i ];
		}
	}
	
	template< typename T, uint TCapacity >
	TIKI_FORCE_INLINE uint tiki::CopyableFixedArray< T, TCapacity >::getIndexOf( ConstIterator pValue ) const
	{
		TIKI_ASSERT( pValue >= m_aData );
		TIKI_ASSERT( pValue < getEnd() );

		return pValue - m_aData;
	}

	template< typename T, uint TCapacity >
	TIKI_FORCE_INLINE uint CopyableFixedArray< T, TCapacity >::getIndexOf( ConstReference value ) const
	{
		for (uint i = 0u; i < TCapacity; ++i)
		{
			if ( m_aData[ i ] == value )
			{
				return i;
			}
		}

		return TIKI_SIZE_T_MAX;
	}

	template< typename T, uint TCapacity >
	TIKI_FORCE_INLINE T& CopyableFixedArray< T, TCapacity >::operator[]( uint index )
	{
		TIKI_ASSERT( index < TCapacity );

		return m_aData[ index ];
	}

	template< typename T, uint TCapacity >
	TIKI_FORCE_INLINE const T& CopyableFixedArray< T, TCapacity >::operator[]( uint index ) const
	{
		TIKI_ASSERT( index < TCapacity );

		return m_aData[ index ];
	}
}

#endif // __TIKI_FIXEDARRAY_INL_INCLUDED__

#pragma once
#ifndef TIKI_FIXEDSIZEDARRAY_INL_INCLUDED
#define TIKI_FIXEDSIZEDARRAY_INL_INCLUDED

#include "tiki/base/assert.hpp"

namespace tiki
{
	template< typename T, uint TCapacity >
	TIKI_FORCE_INLINE FixedSizedArray< T, TCapacity >::FixedSizedArray()
		: m_count( 0u )
	{
	}

	template< typename T, uint TCapacity >
	TIKI_FORCE_INLINE FixedSizedArray< T, TCapacity >::~FixedSizedArray()
	{
		m_count = 0u;
	}

	template< typename T, uint TCapacity >
	TIKI_FORCE_INLINE void FixedSizedArray< T, TCapacity >::create( ConstReference initValue, uint count /*= TCapacity */ )
	{
		TIKI_ASSERT( count <= TCapacity );

		for (uint i = 0u; i < count; ++i)
		{
			m_aData[ i ] = initValue;
		}
		m_count = count;
	}

	template< typename T, uint TCapacity >
	TIKI_FORCE_INLINE void FixedSizedArray< T, TCapacity >::create( ConstIterator pInitData, uint count )
	{
		TIKI_ASSERT( count <= TCapacity );

		for (uint i = 0u; i < count; ++i)
		{
			m_aData[ i ] = pInitData[ i ];
		}
		m_count = count;
	}

	template< typename T, uint TCapacity >
	TIKI_FORCE_INLINE void FixedSizedArray< T, TCapacity >::clear()
	{
		m_count = 0u;
	}

	template< typename T, uint TCapacity >
	TIKI_FORCE_INLINE T& FixedSizedArray< T, TCapacity >::push()
	{
		TIKI_ASSERT( m_count < TCapacity );
		return m_aData[ m_count++ ];
	}

	template< typename T, uint TCapacity >
	TIKI_FORCE_INLINE T& FixedSizedArray< T, TCapacity >::push( ConstReference value )
	{
		TIKI_ASSERT( m_count < TCapacity );
		return m_aData[ m_count++ ] = value;
	}

	template< typename T, uint TCapacity >
	TIKI_FORCE_INLINE T* FixedSizedArray< T, TCapacity >::pushRange( uint count )
	{
		TIKI_ASSERT( count >= 1u );
		TIKI_ASSERT( m_count + count <= TCapacity );

		const uint currentCount = m_count;
		m_count += count;

		return &m_aData[ currentCount ];
	}

	template< typename T, uint TCapacity >
	TIKI_FORCE_INLINE T* FixedSizedArray< T, TCapacity >::pushRange( ConstIterator pData, uint count )
	{
		TIKI_ASSERT( count >= 1u );
		TIKI_ASSERT( m_count + count <= TCapacity );

		const uint currentCount = m_count;
		for (uint i = 0u; i < count; ++i)
		{
			m_aData[ m_count++ ] = pData[ i ];
		}

		return &m_aData[ currentCount ];
	}

	template< typename T, uint TCapacity >
	TIKI_FORCE_INLINE bool FixedSizedArray< T, TCapacity >::pop( Reference value )
	{
		if ( isEmpty() )
		{
			return false;
		}

		value = m_aData[ --m_count ];
		return true;
	}

	template< typename T, uint TCapacity >
	TIKI_FORCE_INLINE void FixedSizedArray< T, TCapacity >::removeUnsortedByIndex( uint index )
	{
		TIKI_ASSERT( index < m_count );
		m_aData[ index ] = m_aData[ --m_count ];
	}

	template< typename T, uint TCapacity >
	TIKI_FORCE_INLINE bool FixedSizedArray< T, TCapacity >::removeUnsortedByValue( ConstReference value )
	{
		for (uint i = 0u; i < m_count; ++i)
		{
			if ( m_aData[ i ] == value )
			{
				removeUnsortedByIndex( i );
				return true;
			}
		}

		return false;
	}

	template< typename T, uint TCapacity >
	TIKI_FORCE_INLINE uint FixedSizedArray< T, TCapacity >::getIndexOfIterator( ConstIterator pValue ) const
	{
		TIKI_ASSERT( pValue >= m_aData );
		TIKI_ASSERT( pValue < getEnd() );

		return pValue - m_aData;
	}

	template< typename T, uint TCapacity >
	TIKI_FORCE_INLINE uint FixedSizedArray< T, TCapacity >::getIndexOfValue( ConstReference value ) const
	{
		for (uint i = 0u; i < m_count; ++i)
		{
			if ( m_aData[ i ] == value )
			{
				return i;
			}
		}

		return TIKI_SIZE_T_MAX;
	}

	template< typename T, uint TCapacity >
	TIKI_FORCE_INLINE T& FixedSizedArray< T, TCapacity >::operator[]( uint index )
	{
		TIKI_ASSERT( index < m_count );
		return m_aData[ index ];
	}

	template< typename T, uint TCapacity >
	TIKI_FORCE_INLINE const T& FixedSizedArray< T, TCapacity >::operator[]( uint index ) const
	{
		TIKI_ASSERT( index < m_count );
		return m_aData[ index ];
	}

	template< typename T, uint TCapacity >
	TIKI_FORCE_INLINE Slice< T > FixedSizedArray< T, TCapacity >::toSlice()
	{
		return Slice< T >( m_aData, &m_count, TCapacity );
	}

	template< typename T, uint TCapacity >
	TIKI_FORCE_INLINE ArrayView< T > FixedSizedArray< T, TCapacity >::toArrayView() const
	{
		return ArrayView< T >( m_aData, m_count );
	}
}

#endif // TIKI_FIXEDSIZEDARRAY_INL_INCLUDED

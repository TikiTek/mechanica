#pragma once
#ifndef __TIKI_FIXEDSIZEDARRAY_INL_INCLUDED__
#define __TIKI_FIXEDSIZEDARRAY_INL_INCLUDED__

#include "tiki/base/fixedsizedarray.hpp"

namespace tiki
{
	template<typename T, uint TCapacity>
	FixedSizedArray<T, TCapacity>::FixedSizedArray()
		: m_count( 0u )
	{
	}

	template<typename T, uint TCapacity>
	FixedSizedArray<T, TCapacity>::~FixedSizedArray()
	{
		m_count = 0u;
	}

	template<typename T, uint TCapacity>
	TIKI_FORCE_INLINE void FixedSizedArray<T, TCapacity>::create( ConstReference initValue, uint count /*= TCapacity */ )
	{
		TIKI_ASSERT( count <= TCapacity );

		for (uint i = 0u; i < count; ++i)
		{
			m_data[ i ] = initValue;
		}
		m_count = count;
	}

	template<typename T, uint TCapacity>
	TIKI_FORCE_INLINE void FixedSizedArray<T, TCapacity>::create( ConstIterator pInitData, uint count )
	{
		TIKI_ASSERT( count <= TCapacity );

		for (uint i = 0u; i < count; ++i)
		{
			m_data[ i ] = pInitData[ i ];
		}
		m_count = count;
	}

	template<typename T, uint TCapacity>
	TIKI_FORCE_INLINE void FixedSizedArray<T, TCapacity>::clear()
	{
		m_count = 0u;
	}

	template<typename T, uint TCapacity>
	TIKI_FORCE_INLINE T& FixedSizedArray<T, TCapacity>::push()
	{
		TIKI_ASSERT( m_count < TCapacity );
		return m_data[ m_count++ ];
	}

	template<typename T, uint TCapacity>
	TIKI_FORCE_INLINE T& FixedSizedArray<T, TCapacity>::push( ConstReference value )
	{
		TIKI_ASSERT( m_count < TCapacity );
		return m_data[ m_count++ ] = value;
	}

	template<typename T, uint TCapacity>
	TIKI_FORCE_INLINE uint FixedSizedArray<T, TCapacity>::getIndex( ConstIterator pValue ) const
	{
		return getIndex( *pValue );
	}

	template<typename T, uint TCapacity>
	TIKI_FORCE_INLINE uint FixedSizedArray<T, TCapacity>::getIndex( ConstReference value ) const
	{
		TIKI_ASSERT( &value >= m_data );
		TIKI_ASSERT( &value < getEnd() );
		return value - m_data;
	}

	template<typename T, uint TCapacity>
	TIKI_FORCE_INLINE T& FixedSizedArray<T, TCapacity>::operator[]( uint index )
	{
		TIKI_ASSERT( index < TCapacity );
		return m_data[ index ];
	}

	template<typename T, uint TCapacity>
	TIKI_FORCE_INLINE const T& FixedSizedArray<T, TCapacity>::operator[]( uint index ) const
	{
		TIKI_ASSERT( index < TCapacity );
		return m_data[ index ];
	}
}

#endif // __TIKI_FIXEDSIZEDARRAY_INL_INCLUDED__

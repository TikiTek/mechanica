#pragma once
#ifndef TIKI_STATICARRAY_INL
#define TIKI_STATICARRAY_INL

#include "tiki/base/staticarray.hpp"
#include "tiki/base/assert.hpp"

namespace tiki
{
	template<typename T>
	TIKI_FORCE_INLINE StaticArray<T>::StaticArray()
	{
		m_pData = nullptr;
		m_count	= 0u;
	}

	template<typename T>
	TIKI_FORCE_INLINE StaticArray<T>::~StaticArray()
	{
		TIKI_ASSERT( m_pData == nullptr );
	}

	template<typename T>
	TIKI_FORCE_INLINE void StaticArray<T>::create( T* pData, size_t count )
	{
		TIKI_ASSERT( m_pData == nullptr );

		m_pData	= pData;
		m_count	= count;
	}

	template<typename T>
	TIKI_FORCE_INLINE void StaticArray<T>::dispose()
	{
		m_pData	= nullptr;
		m_count	= 0u;
	}

	template<typename T>
	TIKI_FORCE_INLINE uint StaticArray<T>::getIndexOf( const T* pValue ) const
	{
		TIKI_ASSERT( pValue != nullptr );
		TIKI_ASSERT( pValue >= getBegin() && pValue < getEnd() );

		return pValue - m_pData;
	}

	template<typename T>
	TIKI_FORCE_INLINE uint StaticArray<T>::getIndexOf( const T& value ) const
	{
		for (uint i = 0u; i < m_count; ++i)
		{
			if ( m_pData[ i ] == value )
			{
				return i;
			}
		}

		return TIKI_SIZE_T_MAX;
	}

	template<typename T>
	TIKI_FORCE_INLINE T& StaticArray<T>::operator[]( size_t index )
	{
		TIKI_ASSERT( index < m_count );
		return m_pData[ index ];
	}

	template<typename T>
	TIKI_FORCE_INLINE const T& StaticArray<T>::operator[]( size_t index ) const
	{
		TIKI_ASSERT( index < m_count );
		return m_pData[ index ];
	}
}

#endif // TIKI_STATICARRAY_INL

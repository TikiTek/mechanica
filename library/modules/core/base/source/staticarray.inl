#pragma once
#ifndef TIKI_STATICARRAY_INL
#define TIKI_STATICARRAY_INL

#include "tiki/base/staticarray.hpp"
#include "tiki/base/assert.hpp"

namespace tiki
{
	template<typename T>
	StaticArray<T>::StaticArray()
	{
		m_pData = nullptr;
		m_count	= 0u;
	}

	template<typename T>
	StaticArray<T>::~StaticArray()
	{
		TIKI_ASSERT( m_pData == nullptr );
	}

	template<typename T>
	TIKI_FORCE_INLINE void StaticArray<T>::create( const T* pData, size_t count )
	{
		TIKI_ASSERT( m_pData == nullptr );

		m_pData	= pData;
		m_count	= count;
	}

	template<typename T>
	TIKI_FORCE_INLINE void StaticArray<T>::dispose()
	{
		TIKI_ASSERT( m_pData );
		m_pData	= nullptr;
		m_count	= 0u;
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

	template<typename T>
	TIKI_FORCE_INLINE size_t StaticArray<T>::getCount() const
	{
		return m_count;
	}

	template<typename T>
	TIKI_FORCE_INLINE T* StaticArray<T>::getData()
	{
		TIKI_ASSERT( m_pData != nullptr );
		TIKI_ASSERT( m_count != 0 );
		return (T*)&m_pData[ 0 ];
	}

	template<typename T>
	TIKI_FORCE_INLINE const T* StaticArray<T>::getData() const
	{
		TIKI_ASSERT( m_pData != nullptr );
		TIKI_ASSERT( m_count != 0 );
		return &m_pData[ 0 ];
	}
}

#endif // TIKI_STATICARRAY_INL

#pragma once
#ifndef TIKI_BUFFER_INL__INCLUDED
#define TIKI_BUFFER_INL__INCLUDED

#include "tiki/base/assert.hpp"

namespace tiki
{
	template<typename T, uint binding>
	Buffer< T, binding >::Buffer()
		: m_count( 0u )
	{
		m_pBuffer = nullptr;
	}

	template<typename T, uint binding>
	Buffer< T, binding >::~Buffer()
	{
		TIKI_ASSERT( m_pBuffer == nullptr );
	}

	template<typename T, uint binding>
	TIKI_FORCE_INLINE void Buffer<T, binding>::apply( const uint index )
	{
		BaseBuffer::apply( index, sizeof( T ), m_count * sizeof( T ), binding );
	}

	template<typename T, uint binding>
	TIKI_FORCE_INLINE T* Buffer<T, binding>::map( const uint count )
	{
		TIKI_ASSERT( count != 0 );

		if( count > m_count )
		{
			m_count = count;
			BaseBuffer::create( sizeof( T ) * m_count, binding );
		}
		
		return (T*)BaseBuffer::map();
	}

	template<typename T, uint binding>
	TIKI_FORCE_INLINE void Buffer<T, binding>::unmap()
	{
		BaseBuffer::unmap();
	}

	template<typename T, uint binding >
	TIKI_FORCE_INLINE void tiki::Buffer<T, binding>::dispose()
	{
		BaseBuffer::dispose();
	}

}

#endif // TIKI_BUFFER_INL__INCLUDED

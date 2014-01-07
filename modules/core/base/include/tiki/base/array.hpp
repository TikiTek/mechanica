#pragma once
#ifndef __TIKI_ARRAY_HPP_INCLUDED__
#define __TIKI_ARRAY_HPP_INCLUDED__

#include "tiki/base/assert.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/base/inline.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	template<typename T>
	class Array
	{
		TIKI_NONCOPYABLE_CLASS( Array );

	public:

		typedef T			Type;
		typedef T&			Reference;
		typedef const T&	ConstReference;
		typedef T*			Iterator;
		typedef const T*	ConstIterator;

		Array()
			: m_pData( nullptr ), m_dataCount( 0u )
		{
		}

		~Array()
		{
			TIKI_ASSERT( m_pData == nullptr );
		}

		TIKI_INLINE void create( uint count, size_t aligment = TIKI_DEFAULT_ALIGNMENT )
		{
			TIKI_ASSERT( m_pData == nullptr );

			m_pData		 = memory::newArray< T >( count, aligment );
			m_dataCount  = count;
		}

		TIKI_INLINE void create( const T* pInitData, uint count, size_t aligment = TIKI_DEFAULT_ALIGNMENT )
		{
			create( count, aligment );

			for (size_t i = 0u; i < count; ++i)
			{
				m_pData[ i ] = pInitData[ i ];
			}
		}

		TIKI_INLINE void dispose()
		{
			if ( m_pData != nullptr )
			{
				memory::deleteArray( m_pData, m_dataCount );
			}

			m_pData		= nullptr;
			m_dataCount	= 0u;
		}

		TIKI_INLINE size_t getCount() const
		{
			return m_dataCount;
		}

		TIKI_INLINE T* getData()
		{
			return m_pData;
		}

		TIKI_INLINE const T* getData() const
		{
			return m_pData;
		}

		TIKI_INLINE T* getLast() 
		{
			return m_pData + ( m_dataCount - 1u );
		}

		TIKI_INLINE const T* getLast() const
		{
			return m_pData + ( m_dataCount - 1u );
		}

		TIKI_INLINE T* getEnd() 
		{
			return m_pData + m_dataCount;
		}

		TIKI_INLINE const T* getEnd() const
		{
			return m_pData + m_dataCount;
		}

		TIKI_INLINE size_t getIndex( const T& value ) const
		{
			TIKI_ASSERT( &value >= m_pData );
			TIKI_ASSERT( &value < getEnd() );

			return ( (size_t)&value - (size_t)m_pData ) / sizeof( T );
		}

		TIKI_INLINE const T& operator[]( uint index ) const
		{
			TIKI_ASSERT( index < m_dataCount );
			return m_pData[ index ];
		}

		TIKI_INLINE T& operator[]( uint index )
		{
			TIKI_ASSERT( index < m_dataCount );
			return m_pData[ index ];
		}

	private:

		T*		m_pData;
		uint	m_dataCount;

	};
}

#endif // __TIKI_ARRAY_HPP_INCLUDED__

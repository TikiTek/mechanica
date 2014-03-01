#pragma once
#ifndef __TIKI_ARRAY_INL_INCLUDED__
#define __TIKI_ARRAY_INL_INCLUDED__

namespace tiki
{
	template<typename T>
	TIKI_FORCE_INLINE Array< T >::Array()
		: m_pData( nullptr ), m_dataCount( 0u )
	{
	}

	template<typename T>
	TIKI_FORCE_INLINE Array< T >::~Array()
	{
		TIKI_ASSERT( m_pData == nullptr );
	}

	template<typename T>
	TIKI_FORCE_INLINE bool Array< T >::create( uint count, size_t aligment = TIKI_DEFAULT_ALIGNMENT )
	{
		TIKI_ASSERT( m_pData == nullptr );

		m_dataCount  = count;
		m_pData		 = memory::newArray< T >( count, aligment );

		return m_pData != nullptr;
	}

	template<typename T>
	TIKI_FORCE_INLINE bool Array< T >::create( const T* pInitData, uint count, size_t aligment = TIKI_DEFAULT_ALIGNMENT )
	{
		if ( create( count, aligment ) == false )
		{
			return false;
		}

		for (size_t i = 0u; i < count; ++i)
		{
			m_pData[ i ] = pInitData[ i ];
		}

		return true;
	}

	template<typename T>
	TIKI_FORCE_INLINE void Array< T >::dispose()
	{
		if ( m_pData != nullptr )
		{
			memory::deleteArray( m_pData, m_dataCount );
		}

		m_pData		= nullptr;
		m_dataCount	= 0u;
	}

	template<typename T>
	TIKI_FORCE_INLINE size_t Array< T >::getCount() const
	{
		return m_dataCount;
	}

	template<typename T>
	TIKI_FORCE_INLINE T* Array< T >::getData()
	{
		return m_pData;
	}

	template<typename T>
	TIKI_FORCE_INLINE const T* Array< T >::getData() const
	{
		return m_pData;
	}

	template<typename T>
	TIKI_FORCE_INLINE T* Array< T >::getLast() 
	{
		return m_pData + ( m_dataCount - 1u );
	}

	template<typename T>
	TIKI_FORCE_INLINE const T* Array< T >::getLast() const
	{
		return m_pData + ( m_dataCount - 1u );
	}

	template<typename T>
	TIKI_FORCE_INLINE T* Array< T >::getEnd() 
	{
		return m_pData + m_dataCount;
	}

	template<typename T>
	TIKI_FORCE_INLINE const T* Array< T >::getEnd() const
	{
		return m_pData + m_dataCount;
	}

	template<typename T>
	TIKI_FORCE_INLINE size_t Array< T >::getIndex( const T& value ) const
	{
		TIKI_ASSERT( &value >= m_pData );
		TIKI_ASSERT( &value < getEnd() );

		return ( (size_t)&value - (size_t)m_pData ) / sizeof( T );
	}

	template<typename T>
	TIKI_FORCE_INLINE const T& Array< T >::operator[]( uint index ) const
	{
		TIKI_ASSERT( index < m_dataCount );
		return m_pData[ index ];
	}

	template<typename T>
	TIKI_FORCE_INLINE T& Array< T >::operator[]( uint index )
	{
		TIKI_ASSERT( index < m_dataCount );
		return m_pData[ index ];
	}
}

#endif // __TIKI_ARRAY_INL_INCLUDED__

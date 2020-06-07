#pragma once

namespace tiki
{
	template< class T >
	DynamicQueue< T >::DynamicQueue()
	{
	}

	template< class T >
	DynamicQueue< T >::~DynamicQueue()
	{
		dispose();
	}

	template< class T >
	void DynamicQueue< T >::dispose()
	{
		clear();

		TIKI_FREE( m_pData );
		m_pData		= nullptr;
		m_capacity	= 0u;
	}

	template< class T >
	bool DynamicQueue< T >::isEmpty() const
	{
		return m_length == 0u;
	}

	template< class T >
	bool DynamicQueue< T >::hasElements() const
	{
		return m_length > 0u;
	}

	template< class T >
	uintreg DynamicQueue< T >::getLength() const
	{
		return m_length;
	}

	template< class T >
	uintreg DynamicQueue< T >::getCapacity() const
	{
		return m_capacity;
	}

	template< class T >
	void DynamicQueue< T >::clear()
	{
		uintreg index = m_top;
		for( uintreg i = 0u; i < m_length; ++i )
		{
			m_pData[ index ].~T();
			index &= m_capacity - 1u;
		}
		TIKI_ASSERT( index == m_bottom );

		m_top		= 0u;
		m_bottom	= 0u;
		m_length	= 0u;
	}

	template< class T >
	void DynamicQueue< T >::reserve( uintreg size )
	{
		checkCapacity( size );
	}

	template< class T >
	T& DynamicQueue< T >::pushBack()
	{
		checkCapacity( m_length + 1u );

		const uintreg index = m_bottom;
		m_bottom = ( m_bottom + 1u ) & (m_capacity - 1u);
		m_length++;

		T* pElement = new( &m_pData[ index ] ) T;
		return *pElement;
	}

	template< class T >
	void DynamicQueue< T >::pushBack( const T& value )
	{
		checkCapacity( m_length + 1u );

		const uintreg index = m_bottom;
		m_bottom = ( m_bottom + 1u ) & (m_capacity - 1u);
		m_length++;

		T* pElement = new( &m_pData[ index ] ) T;
		*pElement = value;
	}

	template< class T >
	void DynamicQueue< T >::popFront()
	{
		TIKI_ASSERT( m_length > 0u );

		m_pData[ m_top ].~T();

		m_top = ( m_top + 1u ) & (m_capacity - 1u);
		m_length--;
	}

	template< class T >
	bool DynamicQueue< T >::tryPopFront( T& target )
	{
		if( isEmpty() )
		{
			return false;
		}

		T& element = m_pData[ m_top ];
		target = element;
		element.~T();

		m_top = ( m_top + 1u ) & (m_capacity - 1u);
		m_length--;
		return true;
	}

	template< class T >
	T& DynamicQueue< T >::getFront()
	{
		TIKI_ASSERT( m_length > 0u );
		return m_pData[ m_top ];
	}

	template< class T >
	const T& DynamicQueue< T >::getFront() const
	{
		TIKI_ASSERT( m_length > 0u );
		return m_pData[ m_top ];
	}

	template< class T >
	T& DynamicQueue< T >::operator[]( uintreg index )
	{
		TIKI_ASSERT( index < m_length );

		const uintreg queueIndex = (m_top + index) & (m_capacity - 1u);
		return m_pData[ queueIndex ];
	}

	template< class T >
	const T& DynamicQueue< T >::operator[]( uintreg index ) const
	{
		TIKI_ASSERT( index < m_length );

		const uintreg queueIndex = (m_top + index) & (m_capacity - 1u);
		return m_pData[ queueIndex ];
	}

	template< class T >
	void DynamicQueue< T >::checkCapacity( uintreg capacity )
	{
		if( capacity <= m_capacity )
		{
			return;
		}

		const uintreg nextCapacity = getNextPowerOfTwo( capacity );
		T* pNewData = (T*)TIKI_ALLOC_ALIGNED( sizeof( T ) * nextCapacity, TIKI_ALIGNOF( T ) );
		TIKI_ASSERT( pNewData != nullptr );

		if( m_top > m_bottom )
		{
			const uintreg endLength = m_capacity - m_top;
			memory::copy( pNewData, m_pData + m_top, sizeof( T ) * endLength );
			memory::copy( pNewData + endLength, m_pData, sizeof( T ) * m_bottom );
		}
		else
		{
			memory::copy( pNewData, m_pData + m_top, sizeof( T ) * m_length );
		}

		TIKI_FREE( m_pData );

		m_pData		= pNewData;
		m_top		= 0u;
		m_bottom	= m_length;
		m_capacity	= nextCapacity;
	}
}


#include "tiki/base/queue.hpp"

#include "tiki/base/memory.hpp"

namespace tiki
{
	template<typename T>
	Queue<T>::Queue()
	{
		m_pData		= nullptr;

		m_top		= 0u;
		m_bottom	= 0u;
		m_capacity	= 0u;
	}

	template<typename T>
	Queue<T>::~Queue()
	{
		TIKI_ASSERT( m_pData == nullptr );
	}

	template<typename T>
	bool Queue<T>::create( uint capacity, uint alignment /*= TIKI_DEFAULT_ALIGNMENT */ )
	{
		m_pData = memory::newArray< T >( capacity, alignment );
		if ( m_pData == nullptr )
		{
			return false;
		}

		m_top		= 0u;
		m_bottom	= 0u;
		m_capacity	= capacity;

		return true;
	}

	template<typename T>
	void Queue<T>::dispose()
	{
		if ( m_pData != nullptr )
		{
			memory::deleteArray( m_pData, m_capacity );
			m_pData = nullptr;
		}

		m_top		= 0u;
		m_bottom	= 0u;
		m_capacity	= 0u;
	}

	template<typename T>
	bool Queue<T>::isEmpty() const
	{
		return m_top == m_bottom;
	}

	template<typename T>
	bool Queue<T>::isFull() const
	{
		return m_top == ( ( m_bottom + 1u ) % m_capacity );
	}

	template<typename T>
	T& Queue<T>::push()
	{
		TIKI_ASSERT( isFull() == false );

		const uint index = m_bottom;
		m_bottom = ( m_bottom + 1u ) % m_capacity;
		return m_pData[ index ];
	}

	template<typename T>
	T& Queue<T>::push( ConstReference value )
	{
		TIKI_ASSERT( isFull() == false );

		const uint index = m_bottom;
		m_bottom = ( m_bottom + 1u ) % m_capacity;
		m_pData[ index ] = value;
		return m_pData[ index ];
	}
	
	template<typename T>
	bool Queue<T>::pop( Reference target )
	{
		if ( isEmpty() == true )
		{
			return false;
		}

		const uint index = m_top;
		m_top = ( m_top + 1u ) % m_capacity;
		target = m_pData[ index ];
		return true;
	}

	template<typename T>
	Reference Queue<T>::getTop()
	{
		TIKI_ASSERT( !isEmpty() );
		return m_pData[ m_top ];
	}

	template<typename T>
	ConstReference Queue<T>::getTop() const
	{
		TIKI_ASSERT( !isEmpty() );
		return m_pData[ m_top ];
	}

	template<typename T>
	uint Queue<T>::getCount() const
	{
		const uint bottom = m_bottom + ( m_bottom < m_top ? m_capacity : 0u );
		return bottom - m_top;
	}

	template<typename T>
	uint Queue<T>::getCapacity() const
	{
		return m_capacity;
	}
}
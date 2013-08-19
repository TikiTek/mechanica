
#include "tiki/base/assert.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/base/functions.hpp"

#include "tiki/toolbase/memorystream.hpp"

namespace tiki
{
	MemoryStream::MemoryStream()
	{
		m_pData		= nullptr;
		m_pos		= 0u;
		m_length	= 0u;
		m_capacity	= 0u;
	}

	MemoryStream::~MemoryStream()
	{
		TIKI_ASSERT( m_pData == nullptr );
	}

	void MemoryStream::create( size_t capacity /*= 0u */ )
	{
		TIKI_ASSERT( m_pData == nullptr );

		if (capacity > 0u )
		{
			m_pData		= (uint8*)memory::allocAlign( capacity, 0u );
		}
		else
		{
			m_pData		= nullptr;
		}

		m_pos			= 0u;
		m_length		= 0u;
		m_capacity		= capacity;
	}

	void MemoryStream::create( const void* pData, size_t length )
	{
		TIKI_ASSERT( m_pData == nullptr );

		m_pData			= (uint8*)memory::allocAlign( length );
		m_pos			= 0u;
		m_length		= length;
		m_capacity		= length;

		memory::copy( m_pData, pData, length );
	}

	void MemoryStream::dispose()
	{
		TIKI_ASSERT( m_pData );

		memory::freeAlign( m_pData );

		m_pData		= nullptr;
		m_length	= 0u;
		m_capacity	= 0u;
		m_pos		= 0u;
	}

	bool MemoryStream::isOpen() const
	{
		return m_pData != nullptr;
	}

	const void* MemoryStream::getData() const
	{
		return m_pData;
	}

	size_t MemoryStream::read( void* pData, size_t length ) const
	{
		size_t length2 = TIKI_MIN( length, (m_length - m_pos) );
		memory::copy( pData, m_pData, length2 );
		return length2;
	}

	void MemoryStream::write( const void* pData, size_t length )
	{
		if ( m_pos + length >= m_capacity )
		{
			const size_t tl	= m_pos + length;
			const size_t cl	= nextPowerOfTwo( tl );

			uint8* pNewData	= (uint8*)memory::allocAlign( cl );
			memory::copy( pNewData, m_pData, m_length );
			memory::freeAlign( m_pData );
			
			m_pData			= pNewData;
			m_capacity		= cl;
		}

		memory::copy( m_pData + m_pos, pData, length );
		m_length	+= length;
		m_pos		+= length;
	}

	size_t MemoryStream::getLength() const
	{
		return m_length;
	}

	size_t MemoryStream::getPosition() const
	{
		return m_pos;
	}

	size_t MemoryStream::setPosition( size_t pos )
	{
		size_t pos2 = TIKI_MIN( pos, m_capacity );
		m_pos		= pos2;
		return pos2;
	}
}
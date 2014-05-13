
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

	MemoryStream::MemoryStream( const MemoryStream& copy )
	{
		*this = copy;
	}

	void MemoryStream::operator=( const MemoryStream& copy )
	{
		m_pos		= copy.m_pos;
		m_length	= copy.m_length;
		m_capacity	= copy.m_capacity;

		if ( m_pData != nullptr )
		{
			memory::freeAlign( m_pData );
		}

		m_pData = (uint8*)TIKI_MEMORY_ALLOC( m_capacity );
		memory::copy( m_pData, copy.m_pData, m_length );
	}

	MemoryStream::~MemoryStream()
	{
		dispose();
	}

	void MemoryStream::create( uint capacity /*= 0u */ )
	{
		TIKI_ASSERT( m_pData == nullptr );

		if (capacity > 0u )
		{
			m_pData		= (uint8*)TIKI_MEMORY_ALLOC( capacity );
		}
		else
		{
			m_pData		= nullptr;
		}

		m_pos			= 0u;
		m_length		= 0u;
		m_capacity		= capacity;
	}

	void MemoryStream::create( const void* pData, uint length )
	{
		TIKI_ASSERT( m_pData == nullptr );

		m_pData			= (uint8*)TIKI_MEMORY_ALLOC( length );
		m_pos			= 0u;
		m_length		= length;
		m_capacity		= length;

		memory::copy( m_pData, pData, length );
	}

	void MemoryStream::dispose()
	{
		if ( m_pData != nullptr )
		{
			memory::freeAlign( m_pData );
			m_pData = nullptr;
		}

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

	uint MemoryStream::read( void* pData, uint length ) const
	{
		uint length2 = TIKI_MIN( length, (m_length - m_pos) );
		memory::copy( pData, m_pData, length2 );
		return length2;
	}

	void MemoryStream::write( const void* pData, uint length )
	{
		if ( m_length + length >= m_capacity )
		{
			const uint tl	= m_length + length;
			const uint cl	= nextPowerOfTwo( tl );
			TIKI_ASSERT( cl >= tl );

			uint8* pNewData	= (uint8*)TIKI_MEMORY_ALLOC( cl );
			memory::copy( pNewData, m_pData, m_length );
			memory::freeAlign( m_pData );
			
			m_pData			= pNewData;
			m_capacity		= cl;
		}

		TIKI_ASSERT( m_pos + length <= m_capacity );
		memory::copy( m_pData + m_pos, pData, length );
		m_pos		+= length;
		m_length	= TIKI_MAX( m_length, m_pos );
	}

	void MemoryStream::writeAlignment( uint alignment )
	{
		uint bytesToWrite = alignment - ( m_length % alignment );

		if ( bytesToWrite == alignment )
		{
			return;
		}

		const uint zero = 0u;
		while ( bytesToWrite > 0u )
		{
			if ( bytesToWrite >= 8u )
			{
				write( &zero, 8u );
				bytesToWrite -= 8u;
			}
			else
			{
				write( &zero, bytesToWrite );
				bytesToWrite -= bytesToWrite;
			}
		}
	}

	uint MemoryStream::getLength() const
	{
		return m_length;
	}

	uint MemoryStream::getPosition() const
	{
		return m_pos;
	}

	uint MemoryStream::setPosition( uint pos )
	{
		uint pos2 = TIKI_MIN( pos, m_capacity );
		m_pos		= pos2;
		return pos2;
	}
}
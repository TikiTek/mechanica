
#include "tiki/base/assert.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/base/functions.hpp"

#include "tiki/io/memorystream.hpp"

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
			TIKI_MEMORY_FREE( m_pData );
		}

		m_pData = (uint8*)TIKI_ALLOC( (uint)m_capacity );
		memory::copy( m_pData, copy.m_pData, (uint)m_length );
	}

	MemoryStream::~MemoryStream()
	{
		dispose();
	}

	void MemoryStream::create( FileSize capacity /*= 0u */ )
	{
		TIKI_ASSERT( m_pData == nullptr );

		if (capacity > 0u )
		{
			m_pData		= (uint8*)TIKI_ALLOC( (uint)capacity );
		}
		else
		{
			m_pData		= nullptr;
		}

		m_pos			= 0u;
		m_length		= 0u;
		m_capacity		= capacity;
	}

	void MemoryStream::create( const void* pData, FileSize length )
	{
		TIKI_ASSERT( m_pData == nullptr );

		m_pData			= (uint8*)TIKI_ALLOC( (uint)length );
		m_pos			= 0u;
		m_length		= length;
		m_capacity		= length;

		memory::copy( m_pData, pData, (uint)length );
	}

	void MemoryStream::dispose()
	{
		if ( m_pData != nullptr )
		{
			TIKI_MEMORY_FREE( m_pData );
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

	FileSize MemoryStream::read( void* pTargetData, FileSize bytesToRead ) const
	{
		bytesToRead = TIKI_MIN( bytesToRead, (m_length - m_pos) );

		memory::copy( pTargetData, m_pData, (uint)bytesToRead );

		return bytesToRead;
	}

	FileSize MemoryStream::write( const void* pSourceData, FileSize bytesToWrite )
	{
		if ( m_length + bytesToWrite >= m_capacity )
		{
			if ( !increaseBuffer( m_length + bytesToWrite ) )
			{
				bytesToWrite = m_capacity - m_pos;
			}
		}

		TIKI_ASSERT( m_pos + bytesToWrite <= m_capacity );
		memory::copy( m_pData + m_pos, pSourceData, (uint)bytesToWrite );
		m_pos		+= bytesToWrite;
		m_length	= TIKI_MAX( m_length, m_pos );

		return bytesToWrite;
	}

	void MemoryStream::writeAlignment( FileSize alignment )
	{
		if ( alignment == 0u )
		{
			return;
		}

		FileSize bytesToWrite = alignment - ( m_length % alignment );
		if ( bytesToWrite == alignment )
		{
			return;
		}

		const FileSize zero = 0u;
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

	FileSize MemoryStream::getPosition() const
	{
		return m_pos;
	}

	void MemoryStream::setPosition( FileSize pos )
	{
		m_pos = TIKI_MIN( pos, m_capacity );
	}

	FileSize MemoryStream::seekPosition( FileOffset offset, DataStreamSeek method /*= DataStreamSeek_Current */ )
	{
		// TODO
		TIKI_ASSERT(false);

		return 0;
	}

	FileSize MemoryStream::getLength() const
	{
		return m_length;
	}

	void MemoryStream::setLength( FileSize length )
	{
		if ( length > m_length )
		{
			if ( length > m_capacity )
			{
				increaseBuffer( length );
			}

			memory::zero( m_pData + m_length, uint( length - m_length ) );
		}

		m_length = TIKI_MIN( m_capacity, length );
	}

	bool MemoryStream::increaseBuffer( FileSize requiredSize )
	{
		const FileSize bufferSize = getNextPowerOfTwo( requiredSize );
		TIKI_ASSERT( bufferSize >= requiredSize );

		uint8* pNewData	= (uint8*)TIKI_ALLOC( (uint)bufferSize );
		if ( pNewData != nullptr )
		{
			memory::copy( pNewData, m_pData, (uint)m_length );
			TIKI_MEMORY_FREE( m_pData );

			m_pData			= pNewData;
			m_capacity		= bufferSize;

			return true;
		}

		return false;
	}
}
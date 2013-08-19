
#include "tiki/converterbase/resourcewriter.hpp"

#include "tiki/base/fourcc.hpp"
#include "tiki/toolbase/filestream.hpp"

namespace tiki
{
	ResourceWriter::~ResourceWriter()
	{
		TIKI_ASSERT( m_fileStream.isOpen() == false );
	}

	void ResourceWriter::create( fourcc cc, const string& fileName, PlatformType platform /*= Platform_Win */ )
	{
		m_fourcc	= cc;
		m_fileName	= fileName;
		m_platform	= platform;

		m_fileStream.create();
	}

	void ResourceWriter::dispose()
	{
		FileStream stream;
		stream.create( m_fileName, FOM_Write );

		const fourcc tiki = TIKI_FOURCC( 'T', 'I', 'K', 'I' );
		const uint32 size = m_fileStream.getLength();
		const uint32 null = 0u;

		stream.write( &tiki, sizeof( fourcc ) );
		stream.write( &m_fourcc, sizeof( fourcc ) );
		stream.write( &size, sizeof( uint32 ) );
		stream.write( &null, sizeof( uint32 ) );

		stream.write( m_fileStream.getData(), size );

		stream.dispose();
		m_fileStream.dispose();
		m_fourcc	= 0u;
		m_fileName	= "";
	}

	void ResourceWriter::writeAlignment( size_t alignment )
	{
		const uint8 null = 0u;

		while ( m_fileStream.getLength() % alignment )
		{
			m_fileStream.write( &null, 1u );
		}
	}
	
	void ResourceWriter::writeData( const void* pData, size_t length )
	{
		m_fileStream.write( pData, length );
	}

	void ResourceWriter::writeUInt8( uint8 value )
	{
		m_fileStream.write( &value, sizeof( value ) );
	}

	void ResourceWriter::writeUInt16( uint16 value )
	{
		m_fileStream.write( &value, sizeof( value ) );
	}

	void ResourceWriter::writeUInt32( uint32 value )
	{
		m_fileStream.write( &value, sizeof( value ) );
	}

	void ResourceWriter::writeUInt64( uint64 value )
	{
		m_fileStream.write( &value, sizeof( value ) );
	}

	void ResourceWriter::writeSInt8( sint8 value )
	{
		m_fileStream.write( &value, sizeof( value ) );
	}

	void ResourceWriter::writeSInt16( sint16 value )
	{
		m_fileStream.write( &value, sizeof( value ) );
	}

	void ResourceWriter::writeSInt32( sint32 value )
	{
		m_fileStream.write( &value, sizeof( value ) );
	}

	void ResourceWriter::writeSInt64( sint64 value )
	{
		m_fileStream.write( &value, sizeof( value ) );
	}

	void ResourceWriter::writeFloat( float value )
	{
		m_fileStream.write( &value, sizeof( value ) );
	}

	void ResourceWriter::writeDouble( double value )
	{
		m_fileStream.write( &value, sizeof( value ) );
	}

	void ResourceWriter::writeString( const string& text )
	{
		writeUInt32( text.length() );
		writeData( text.cStr(), text.length() );
	}

}
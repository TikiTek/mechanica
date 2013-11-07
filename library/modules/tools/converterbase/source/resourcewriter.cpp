
#include "tiki/converterbase/resourcewriter.hpp"

#include "tiki/base/fourcc.hpp"
#include "tiki/toolbase/filestream.hpp"

namespace tiki
{
	ResourceWriter::~ResourceWriter()
	{
		TIKI_ASSERT( m_fileName.isEmpty() );
		TIKI_ASSERT( m_resources.isEmpty() );
	}

	void ResourceWriter::create( const string& fileName, PlatformType platform )
	{
		m_fileName	= fileName;
		m_platform	= platform;
	}

	void ResourceWriter::dispose()
	{
		FileStream stream;
		stream.create( m_fileName, FOM_Write );

		ResourceFileHeader fileHeader;
		fileHeader.tikiFourcc		= TIKI_FOURCC( 'T', 'I', 'K', 'I' );
		fileHeader.version			= ResourceFileHeader::CurrentFormatVersion;
		fileHeader.resourceCount	= m_resources.getCount();

		stream.write( &fileHeader, sizeof( ResourceFileHeader ) );

		for (uint i = 0u; i < m_resources.getCount(); ++i)
		{
			const ResourceData& resource = m_resources[ i ];

			// :TODO: write resource
		}

		stream.dispose();

		m_resources.dispose();
		m_fileName	= "";
		m_platform	= PlatformType_Invalid;
	}

	void ResourceWriter::openResource( const string& name, fourcc type, uint8 resourceFormatVersion )
	{
		TIKI_ASSERT( m_pCurrentResource == nullptr );

		ResourceData& resource = m_resources.add();
		resource.name		= name;
		resource.type		= type;
		resource.version	= resourceFormatVersion;

		m_pCurrentResource = &resource;
	}

	void ResourceWriter::closeResource()
	{
		TIKI_ASSERT( m_pCurrentResource != nullptr );
		m_pCurrentResource = nullptr;
	}

	void ResourceWriter::openDataSection( uint8 AllocatorId, uint8 alignment /*= TIKI_DEFAULT_ALIGNMENT */ )
	{
		TIKI_ASSERT( m_pCurrentResource != nullptr );
	}

	void ResourceWriter::closeDataSection()
	{
		TIKI_ASSERT( m_pCurrentSection != nullptr );

	}

	ReferenceKey ResourceWriter::addString( StringType type, const string& text )
	{
		return ReferenceKey();
	}

	ReferenceKey ResourceWriter::addResourceLink( const string& fileName, crc32 resourceKey )
	{
		return ReferenceKey();
	}

	ReferenceKey ResourceWriter::addDataPoint()
	{
		return ReferenceKey();
	}

	void ResourceWriter::writeAlignment( uint alignment )
	{
		TIKI_ASSERT( m_pCurrentSection != nullptr );

		const uint8 null = 0u;
		while ( m_pCurrentSection->binaryData.getLength() % alignment )
		{
			m_pCurrentSection->binaryData.write( &null, 1u );
		}
	}
	
	void ResourceWriter::writeReference( const ReferenceKey& key )
	{

	}

	void ResourceWriter::writeData( const void* pData, uint length )
	{
		TIKI_ASSERT( m_pCurrentSection != nullptr );
		m_pCurrentSection->binaryData.write( pData, length );
	}

	void ResourceWriter::writeUInt8( uint8 value )
	{
		TIKI_ASSERT( m_pCurrentSection != nullptr );
		m_pCurrentSection->binaryData.write( &value, sizeof( value ) );
	}

	void ResourceWriter::writeUInt16( uint16 value )
	{
		TIKI_ASSERT( m_pCurrentSection != nullptr );
		m_pCurrentSection->binaryData.write( &value, sizeof( value ) );
	}

	void ResourceWriter::writeUInt32( uint32 value )
	{
		TIKI_ASSERT( m_pCurrentSection != nullptr );
		m_pCurrentSection->binaryData.write( &value, sizeof( value ) );
	}

	void ResourceWriter::writeUInt64( uint64 value )
	{
		TIKI_ASSERT( m_pCurrentSection != nullptr );
		m_pCurrentSection->binaryData.write( &value, sizeof( value ) );
	}

	void ResourceWriter::writeSInt8( sint8 value )
	{
		TIKI_ASSERT( m_pCurrentSection != nullptr );
		m_pCurrentSection->binaryData.write( &value, sizeof( value ) );
	}

	void ResourceWriter::writeSInt16( sint16 value )
	{
		TIKI_ASSERT( m_pCurrentSection != nullptr );
		m_pCurrentSection->binaryData.write( &value, sizeof( value ) );
	}

	void ResourceWriter::writeSInt32( sint32 value )
	{
		TIKI_ASSERT( m_pCurrentSection != nullptr );
		m_pCurrentSection->binaryData.write( &value, sizeof( value ) );
	}

	void ResourceWriter::writeSInt64( sint64 value )
	{
		TIKI_ASSERT( m_pCurrentSection != nullptr );
		m_pCurrentSection->binaryData.write( &value, sizeof( value ) );
	}

	void ResourceWriter::writeFloat( float value )
	{
		TIKI_ASSERT( m_pCurrentSection != nullptr );
		m_pCurrentSection->binaryData.write( &value, sizeof( value ) );
	}

	void ResourceWriter::writeDouble( double value )
	{
		TIKI_ASSERT( m_pCurrentSection != nullptr );
		m_pCurrentSection->binaryData.write( &value, sizeof( value ) );
	}

}
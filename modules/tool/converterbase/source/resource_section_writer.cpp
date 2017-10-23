#include "tiki/converterbase/resource_section_writer.hpp"

namespace tiki
{
	ResourceSectionWriter::ResourceSectionWriter()
	{
		m_pResourceWriter	= nullptr;
		m_sectionId			= TIKI_SIZE_T_MAX;
	}

	ResourceSectionWriter::~ResourceSectionWriter()
	{
		TIKI_ASSERT( m_pResourceWriter == nullptr );
	}

	void ResourceSectionWriter::create( ResourceWriter* pResourceWriter, uint sectionId )
	{
		m_pResourceWriter	= pResourceWriter;
		m_sectionId			= sectionId;
	}

	void ResourceSectionWriter::dispose()
	{
		m_pResourceWriter	= nullptr;
		m_sectionId			= TIKI_SIZE_T_MAX;
		m_sectionData.dispose();
	}

	uint ResourceSectionWriter::getCurrentSize() const
	{
		TIKI_ASSERT( m_pResourceWriter != nullptr );
		return (uint)m_sectionData.getLength();
	}

	ReferenceKey ResourceSectionWriter::addString( const string& text )
	{
		TIKI_ASSERT( m_pResourceWriter != nullptr );
		return m_pResourceWriter->addString( text );
	}

	ReferenceKey ResourceSectionWriter::addResourceLink( const string& fileName, crc32 resourceKey, fourcc resourceType )
	{
		TIKI_ASSERT( m_pResourceWriter != nullptr );
		return m_pResourceWriter->addResourceLink( fileName, resourceKey, resourceType );
	}

	ReferenceKey ResourceSectionWriter::addDataPoint()
	{
		TIKI_ASSERT( m_pResourceWriter != nullptr );

		ReferenceKey key;
		key.type					= ReferenceType_Pointer;
		key.identifier				= m_sectionId;
		key.offsetInTargetSection	= (uint)m_sectionData.getLength();

		return key;
	}

	ResourceWriter& ResourceSectionWriter::getResourceWriter()
	{
		TIKI_ASSERT( m_pResourceWriter != nullptr );
		return *m_pResourceWriter;
	}

	void ResourceSectionWriter::writeAlignment( uint alignment )
	{
		TIKI_ASSERT( m_pResourceWriter != nullptr );

		const uint8 null = 0u;
		while( m_sectionData.getLength() % alignment )
		{
			m_sectionData.write( &null, 1u );
		}
	}

	void ResourceSectionWriter::writeReference( const ReferenceKey* pKey )
	{
		TIKI_ASSERT( m_pResourceWriter != nullptr );

		writeAlignment( 8u );

		if( pKey != nullptr )
		{
			ResourceReferenceData& data = m_sectionReferences.add();
			data.key				= *pKey;
			data.offsetInSection	= m_sectionData.getLength();
		}

		// reserve space for the pointer
		const uint64 zero = 0u;
		m_sectionData.write( &zero, 8u );
	}

	void ResourceSectionWriter::writeData( const void* pData, uint length )
	{
		TIKI_ASSERT( m_pResourceWriter != nullptr );
		m_sectionData.write( pData, length );
	}

	void ResourceSectionWriter::writeUInt8( uint8 value )
	{
		TIKI_ASSERT( m_pResourceWriter != nullptr );
		m_sectionData.write( &value, sizeof( value ) );
	}

	void ResourceSectionWriter::writeUInt16( uint16 value )
	{
		TIKI_ASSERT( m_pResourceWriter != nullptr );
		m_sectionData.write( &value, sizeof( value ) );
	}

	void ResourceSectionWriter::writeUInt32( uint32 value )
	{
		TIKI_ASSERT( m_pResourceWriter != nullptr );
		m_sectionData.write( &value, sizeof( value ) );
	}

	void ResourceSectionWriter::writeUInt64( uint64 value )
	{
		TIKI_ASSERT( m_pResourceWriter != nullptr );
		m_sectionData.write( &value, sizeof( value ) );
	}

	void ResourceSectionWriter::writeSInt8( sint8 value )
	{
		TIKI_ASSERT( m_pResourceWriter != nullptr );
		m_sectionData.write( &value, sizeof( value ) );
	}

	void ResourceSectionWriter::writeSInt16( sint16 value )
	{
		TIKI_ASSERT( m_pResourceWriter != nullptr );
		m_sectionData.write( &value, sizeof( value ) );
	}

	void ResourceSectionWriter::writeSInt32( sint32 value )
	{
		TIKI_ASSERT( m_pResourceWriter != nullptr );
		m_sectionData.write( &value, sizeof( value ) );
	}

	void ResourceSectionWriter::writeSInt64( sint64 value )
	{
		TIKI_ASSERT( m_pResourceWriter != nullptr );
		m_sectionData.write( &value, sizeof( value ) );
	}

	void ResourceSectionWriter::writeFloat( float value )
	{
		TIKI_ASSERT( m_pResourceWriter != nullptr );
		m_sectionData.write( &value, sizeof( value ) );
	}

	void ResourceSectionWriter::writeDouble( double value )
	{
		TIKI_ASSERT( m_pResourceWriter != nullptr );
		m_sectionData.write( &value, sizeof( value ) );
	}
}

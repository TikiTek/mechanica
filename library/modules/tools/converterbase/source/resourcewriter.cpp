
#include "tiki/converterbase/resourcewriter.hpp"

#include "tiki/base/bits.hpp"
#include "tiki/base/crc32.hpp"
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
		ResourceFileHeader fileHeader;
		fileHeader.tikiFourcc		= TIKI_FOURCC( 'T', 'I', 'K', 'I' );
		fileHeader.version			= ResourceFileHeader::CurrentFormatVersion;
		fileHeader.resourceCount	= m_resources.getCount();

		MemoryStream stream;
		stream.write( &fileHeader, sizeof( ResourceFileHeader ) );
				
		for (uint i = 0u; i < m_resources.getCount(); ++i)
		{
			const ResourceData& resource = m_resources[ i ];

			MemoryStream headerData;

			for (uint j = 0u; j < resource.links.getCount(); ++j)
			{
				const ResourceLinkData& linkData = resource.links[ j ];

				ResourceLinkItem item;
				item.fileKey		= crcString( linkData.fileName );
				item.resourceKey	= linkData.resourceKey;

				headerData.write( &item, sizeof( item ) );
			}

			for (uint j = 0u; j < resource.strings.getCount(); ++j)
			{
				const StringData& stringData = resource.strings[ j ];

				uint32 bitMask = setBitValue( 0u, 0u, 2u, stringData.type );
				bitMask = setBitValue( bitMask, 2u, 2u, stringData.sizeInBytes / stringData.text.getLength() );
				bitMask = setBitValue( bitMask, 4u, 28u, stringData.text.getLength() ) ;

				headerData.write( &bitMask, sizeof( bitMask ) );
				headerData.write( stringData.text.cStr(), stringData.sizeInBytes );
			}

			const uint baseOffset = stream.getLength() + ( resource.sections.getCount() * sizeof( SectionHeader ) ) + headerData.getLength();
			uint currentOffset = 0u;

			for (uint j = 0u; j < resource.sections.getCount(); ++j)
			{
				const SectionData& sectionData = resource.sections[ j ];

				currentOffset = alignValue( currentOffset, sectionData.alignment );

				SectionHeader sectionHeader;
				sectionHeader.alignment					= 64u - countLeadingZeros64( sectionData.alignment );
				sectionHeader.allocatorType_allocatorId = 0u;
				sectionHeader.referenceCount			= sectionData.references.getCount();
				sectionHeader.offsetInFile				= currentOffset;
				sectionHeader.sizeInBytes				= sectionData.binaryData.getLength();

				stream.write( &sectionHeader, sizeof( sectionHeader ) );

				currentOffset += sectionData.binaryData.getLength();
			} 

			stream.write( headerData.getData(), headerData.getLength() );
			headerData.dispose();

			for (uint j = 0u; j < resource.sections.getCount(); ++j)
			{
				const SectionData& sectionData = resource.sections[ j ];

				for (uint k = 0u; k < sectionData.references.getCount(); ++k)
				{
					const ReferenceData& referenceData = sectionData.references[ k ];

					ReferenceItem item;
					item.type					= (uint8)referenceData.key.type;
					item.targetId				= (uint16)referenceData.key.identifier;
					item.offsetInTargetSection	= referenceData.key.offsetInTargetSection;
					item.offsetInSection		= referenceData.position;

					stream.write( &item, sizeof( item ) );
				} 

				stream.writeAlignment( sectionData.alignment );
				stream.write( sectionData.binaryData.getData(), sectionData.binaryData.getLength() );
			}			
		}

		FileStream fileStream;
		fileStream.create( m_fileName, FOM_Write );

		fileStream.write( stream.getData(), stream.getLength() );

		fileStream.dispose();
		stream.dispose();

		m_resources.dispose();
		m_fileName	= "";
		m_platform	= PlatformType_Invalid;
	}

	void ResourceWriter::openResource( const string& name, fourcc type, uint16 resourceFormatVersion )
	{
		TIKI_ASSERT( m_pCurrentResource == nullptr );
		TIKI_ASSERT( m_pCurrentSection == nullptr );

		ResourceData& resource = m_resources.add();
		resource.name		= name;
		resource.type		= type;
		resource.version	= resourceFormatVersion;

		m_pCurrentResource = &resource;
	}

	void ResourceWriter::closeResource()
	{
		TIKI_ASSERT( m_pCurrentResource != nullptr );
		TIKI_ASSERT( m_pCurrentSection == nullptr );
		m_pCurrentResource = nullptr;
	}

	void ResourceWriter::openDataSection( uint8 allocatorId, AllocatorType allocatorType, uint alignment /*= TIKI_DEFAULT_ALIGNMENT */ )
	{
		TIKI_ASSERT( m_pCurrentResource != nullptr );
		TIKI_ASSERT( m_pCurrentSection == nullptr );
		TIKI_ASSERT( isPowerOfTwo( alignment ) );

		const uint id = m_pCurrentResource->sections.getCount();
		SectionData& data = m_pCurrentResource->sections.add();
		data.id				= id;
		data.alignment		= alignment;
		data.allocatorId	= allocatorId;
		data.allocatorType	= allocatorType;
	}

	void ResourceWriter::closeDataSection()
	{
		TIKI_ASSERT( m_pCurrentResource != nullptr );
		TIKI_ASSERT( m_pCurrentSection != nullptr );
		m_pCurrentSection = nullptr;
	}

	ReferenceKey ResourceWriter::addString( StringType type, const string& text )
	{
		TIKI_ASSERT( m_pCurrentResource != nullptr );

		ReferenceKey key;
		key.type		= ReferenceType_String;
		key.identifier	= m_pCurrentResource->strings.getCount();

		StringData& data = m_pCurrentResource->strings.add();
		data.type			= StringType_Char;
		data.text			= text;
		data.sizeInBytes	= text.getLength();

		return key;
	}

	ReferenceKey ResourceWriter::addResourceLink( const string& fileName, crc32 resourceKey )
	{
		TIKI_ASSERT( m_pCurrentResource != nullptr );

		ReferenceKey key;
		key.type		= ReferenceType_ResourceLink;
		key.identifier	= m_pCurrentResource->links.getCount();

		ResourceLinkData& data = m_pCurrentResource->links.add();
		data.fileName		= fileName;
		data.resourceKey	= resourceKey;

		return key;
	}

	ReferenceKey ResourceWriter::addDataPoint()
	{
		TIKI_ASSERT( m_pCurrentResource != nullptr );
		TIKI_ASSERT( m_pCurrentSection != nullptr );

		ReferenceKey key;
		key.type					= ReferenceType_Pointer;
		key.identifier				= m_pCurrentSection->id;
		key.offsetInTargetSection	= m_pCurrentSection->binaryData.getLength();

		return key;
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
		TIKI_ASSERT( m_pCurrentSection != nullptr );

		ReferenceData& data = m_pCurrentSection->references.add();
		data.key		= key;
		data.position	= m_pCurrentSection->binaryData.getLength();

		// reserve space for the pointer
		const uint64 zero = 0u;
		m_pCurrentSection->binaryData.write( &zero, sizeof( zero ) );
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
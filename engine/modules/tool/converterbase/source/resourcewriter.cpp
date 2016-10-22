
#include "tiki/converterbase/resourcewriter.hpp"

#include "tiki/base/bits.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/io/filestream.hpp"

namespace tiki
{
	ResourceWriter::~ResourceWriter()
	{
		TIKI_ASSERT( m_fileName.isEmpty() );
		TIKI_ASSERT( m_resources.isEmpty() );
	}

	void ResourceWriter::create( const string& fileName )
	{
		m_fileName	= fileName;

		m_pCurrentResource	= nullptr;
		m_pCurrentSection	= nullptr;
	}

	void ResourceWriter::dispose()
	{
		ResourceFileHeader fileHeader;
		fileHeader.tikiFourcc		= TIKI_FOURCC( 'T', 'I', 'K', 'I' );
		fileHeader.version			= ResourceFileHeader::CurrentFormatVersion;
		fileHeader.resourceCount	= uint16( m_resources.getCount() );

		MemoryStream stream;
		stream.write( &fileHeader, sizeof( ResourceFileHeader ) );

		List< ResourceHeader > resourceHeaders;
		for (uint i = 0u; i < m_resources.getCount(); ++i)
		{
			const ResourceData& resource = m_resources[ i ];

			ResourceHeader& header = resourceHeaders.add();
			header.type			= resource.type;
			header.key			= crcString( resource.name );
			header.definition	= resource.definition.definitionMask;
			header.version		= resource.version;

			header.linkCount	= uint16( resource.links.getCount() );
			header.sectionCount	= uint16( resource.sections.getCount() );
			header.stringCount	= uint16( resource.strings.getCount() );

			header.offsetInFile				= 0u;

			header.stringOffsetInResource	= 0u;
			header.stringSizeInBytes		= 0u;
		} 
		stream.write( resourceHeaders.getBegin(), sizeof( ResourceHeader ) * resourceHeaders.getCount() );

		for (uint resourceIndex = 0u; resourceIndex < m_resources.getCount(); ++resourceIndex)
		{
			const ResourceData& resource	= m_resources[ resourceIndex ];
			ResourceHeader& header			= resourceHeaders[ resourceIndex ];
			header.offsetInFile				= uint32( stream.getPosition() );

			List< SectionHeader > sectionHeaders;
			for (uint j = 0u; j < resource.sections.getCount(); ++j)
			{
				const SectionData& sectionData = resource.sections[ j ];

				SectionHeader& sectionHeader = sectionHeaders.add();
				sectionHeader.alignment					= uint8( 64u - countLeadingZeros64( sectionData.alignment ) );
				sectionHeader.allocatorType_allocatorId	= uint8( ( sectionData.allocatorType << 6u ) | sectionData.allocatorId );
				sectionHeader.referenceCount			= uint16( sectionData.references.getCount() );
				sectionHeader.sizeInBytes				= uint32( sectionData.binaryData.getLength() );
				sectionHeader.offsetInResource			= 0u;
			} 
			stream.write( sectionHeaders.getBegin(), sizeof( SectionHeader ) * sectionHeaders.getCount() );
			
			List< StringItem > stringItems;
			for (uint stringIndex = 0u; stringIndex < resource.strings.getCount(); ++stringIndex)
			{
				const StringData& stringData = resource.strings[ stringIndex ];

				uint32 bitMask = setBitValue( 0u, 0u, 2u, stringData.type );
				bitMask = setBitValue( bitMask, 2u, 2u, uint32( stringData.sizeInBytes / stringData.text.getLength() ) );
				bitMask = setBitValue( bitMask, 4u, 28u, uint32( stringData.text.getLength() ) );

				StringItem& stringItem = stringItems.add();
				stringItem.type_lengthModifier_textLength	= bitMask;
				stringItem.offsetInBlock					= 0u;

				header.stringSizeInBytes += uint32( stringData.text.getLength() + 1u );
			}
			stream.write( stringItems.getBegin(), sizeof( StringItem ) * stringItems.getCount() );

			for (uint linkIndex = 0u; linkIndex < resource.links.getCount(); ++linkIndex)
			{
				const ResourceLinkData& linkData = resource.links[ linkIndex ];

				ResourceLinkItem item;
				item.fileKey		= crcString( linkData.fileName );
				item.resourceKey	= linkData.resourceKey;
				item.resourceType	= linkData.resourceType;

				stream.write( &item, sizeof( item ) );
			}

			for (uint sectionIndex = 0u; sectionIndex < resource.sections.getCount(); ++sectionIndex)
			{
				const SectionData& sectionData = resource.sections[ sectionIndex ];

				stream.writeAlignment( sectionData.alignment );

				sectionHeaders[ sectionIndex ].offsetInResource = uint32( stream.getPosition() - header.offsetInFile );
				stream.write( sectionData.binaryData.getData(), sectionData.binaryData.getLength() );

				for (uint k = 0u; k < sectionData.references.getCount(); ++k)
				{
					const ReferenceData& referenceData = sectionData.references[ k ];

					ReferenceItem item;
					item.type					= uint8( referenceData.key.type );
					item.targetId				= uint16( referenceData.key.identifier );
					item.offsetInTargetSection	= uint32( referenceData.key.offsetInTargetSection );
					item.offsetInSection		= referenceData.position;

					stream.write( &item, sizeof( item ) );
				}
			}

			header.stringOffsetInResource = uint32( stream.getPosition() - header.offsetInFile );
			for (uint stringIndex = 0u; stringIndex < resource.strings.getCount(); ++stringIndex)
			{
				stringItems[ stringIndex ].offsetInBlock = uint32( stream.getPosition() - header.offsetInFile - header.stringOffsetInResource );

				const string& text = resource.strings[ stringIndex ].text;
				stream.write( text.cStr(), text.getLength() + 1u );
			} 

			stream.setPosition( header.offsetInFile );
			stream.write( sectionHeaders.getBegin(), sizeof( SectionHeader ) * sectionHeaders.getCount() );
			stream.write( stringItems.getBegin(), sizeof( StringItem ) * stringItems.getCount() );
			stream.setPosition( stream.getLength() );
		}

		stream.setPosition( sizeof( ResourceFileHeader ) );
		stream.write( resourceHeaders.getBegin(), sizeof( ResourceHeader ) * resourceHeaders.getCount() );

		FileStream fileStream;
		if (fileStream.create( m_fileName.cStr(), DataAccessMode_Write ))
		{
			fileStream.write( stream.getData(), stream.getLength() );
			fileStream.dispose();
		}
		else
		{
			TIKI_TRACE_ERROR( "[converter] Unable to open resource file '%s'!\n", m_fileName.cStr() );
		}

		stream.dispose();

		m_resources.dispose();
		m_fileName	= "";
	}

	void ResourceWriter::openResource( const string& name, fourcc type, const ResourceDefinition& definition, uint16 resourceFormatVersion )
	{
		TIKI_ASSERT( m_pCurrentResource == nullptr );
		TIKI_ASSERT( m_pCurrentSection == nullptr );

		ResourceData& resource = m_resources.add();
		resource.name		= name;
		resource.type		= type;
		resource.definition	= definition;
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
		TIKI_ASSERT( isPowerOfTwo( alignment ) );

		if ( m_pCurrentSection != nullptr )
		{
			m_sectionStack.add( m_pCurrentSection->id );
		}

		const uint id = m_pCurrentResource->sections.getCount();
		SectionData& section = m_pCurrentResource->sections.add();
		section.id				= id;
		section.alignment		= alignment;
		section.allocatorId		= allocatorId;
		section.allocatorType	= allocatorType;

		m_pCurrentSection = &section;		
	}

	void ResourceWriter::closeDataSection()
	{
		TIKI_ASSERT( m_pCurrentResource != nullptr );
		TIKI_ASSERT( m_pCurrentSection != nullptr );

		if ( m_sectionStack.isEmpty() )
		{
			m_pCurrentSection = nullptr;
		}
		else
		{
			const uint sectionId = m_sectionStack.getLast();
			m_sectionStack.removeSortedAtIndex( m_sectionStack.getCount() - 1u );

			m_pCurrentSection = &m_pCurrentResource->sections[ sectionId ];
		}
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

	ReferenceKey ResourceWriter::addResourceLink( const string& fileName, crc32 resourceKey, fourcc resourceType )
	{
		TIKI_ASSERT( m_pCurrentResource != nullptr );

		ReferenceKey key;
		key.type					= ReferenceType_ResourceLink;
		key.identifier				= m_pCurrentResource->links.getCount();
		key.offsetInTargetSection	= 0u;

		ResourceLinkData& data = m_pCurrentResource->links.add();
		data.fileName		= fileName;
		data.resourceKey	= resourceKey;
		data.resourceType	= resourceType;

		return key;
	}

	ReferenceKey ResourceWriter::addDataPoint()
	{
		TIKI_ASSERT( m_pCurrentResource != nullptr );
		TIKI_ASSERT( m_pCurrentSection != nullptr );

		ReferenceKey key;
		key.type					= ReferenceType_Pointer;
		key.identifier				= m_pCurrentSection->id;
		key.offsetInTargetSection	= (uint)m_pCurrentSection->binaryData.getLength();

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
	
	void ResourceWriter::writeReference( const ReferenceKey* pKey )
	{
		TIKI_ASSERT( m_pCurrentSection != nullptr );

		writeAlignment( 8u );

		if ( pKey != nullptr )
		{
			ReferenceData& data = m_pCurrentSection->references.add();
			data.key		= *pKey;
			data.position	= uint32( m_pCurrentSection->binaryData.getLength() );
		}

		// reserve space for the pointer
		const uint64 zero = 0u;
		m_pCurrentSection->binaryData.write( &zero, 8u );
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

	uint ResourceWriter::getSizeOfCurrentSection() const
	{
		TIKI_ASSERT( m_pCurrentSection != nullptr );
		return (uint)m_pCurrentSection->binaryData.getLength();
	}
}
#include "tiki/converterbase/resource_writer.hpp"

#include "tiki/base/bits.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/io/filestream.hpp"
#include "tiki/io/memorystream.hpp"

namespace tiki
{
	ResourceWriter::ResourceWriter()
	{
		m_pCurrentResource = nullptr;
	}

	ResourceWriter::~ResourceWriter()
	{
		TIKI_ASSERT( m_filePath.isEmpty() );
		TIKI_ASSERT( m_resources.isEmpty() );
	}

	void ResourceWriter::create( const Path& filePath )
	{
		m_filePath			= filePath;
		m_pCurrentResource	= nullptr;
	}

	void ResourceWriter::dispose()
	{
		m_resources.dispose();
		m_filePath.clear();
	}

	bool ResourceWriter::writeToFile()
	{
		ResourceFileHeader fileHeader;
		fileHeader.tikiFourcc		= TIKI_FOURCC( 'T', 'I', 'K', 'I' );
		fileHeader.version			= ResourceFileHeader::CurrentFormatVersion;
		fileHeader.resourceCount	= uint16( m_resources.getCount() );

		MemoryStream stream;
		stream.write( &fileHeader, sizeof( ResourceFileHeader ) );

		List< ResourceHeader > resourceHeaders;
		for( uint i = 0u; i < m_resources.getCount(); ++i )
		{
			const ResourceData& resource = m_resources[ i ];

			ResourceHeader& header = resourceHeaders.add();
			header.type			= resource.type;
			header.key			= crcString( resource.name );
			header.definition	= resource.definition;
			header.version		= resource.version;

			header.linkCount	= uint16( resource.links.getCount() );
			header.sectionCount	= uint16( resource.sections.getCount() );
			header.stringCount	= uint16( resource.strings.getCount() );

			header.offsetInFile				= 0u;

			header.stringOffsetInResource	= 0u;
			header.stringSizeInBytes		= 0u;
		}
		stream.write( resourceHeaders.getBegin(), sizeof( ResourceHeader ) * resourceHeaders.getCount() );

		for( uint resourceIndex = 0u; resourceIndex < m_resources.getCount(); ++resourceIndex )
		{
			const ResourceData& resource	= m_resources[ resourceIndex ];
			ResourceHeader& header			= resourceHeaders[ resourceIndex ];
			header.offsetInFile				= uint32( stream.getPosition() );

			List< SectionHeader > sectionHeaders;
			for( uint j = 0u; j < resource.sections.getCount(); ++j )
			{
				const SectionData& sectionData = resource.sections[ j ];

				SectionHeader& sectionHeader = sectionHeaders.add();
				sectionHeader.type						= sectionData.type;
				sectionHeader.alignment					= uint8( 64u - countLeadingZeros64( sectionData.alignment ) );
				sectionHeader.referenceCount			= uint16( sectionData.references.getCount() );
				sectionHeader.sizeInBytes				= uint32( sectionData.binaryData.getCount() );
				sectionHeader.offsetInResource			= 0u;
			}
			stream.write( sectionHeaders.getBegin(), sizeof( SectionHeader ) * sectionHeaders.getCount() );

			List< StringItem > stringItems;
			for( uint stringIndex = 0u; stringIndex < resource.strings.getCount(); ++stringIndex )
			{
				const StringData& stringData = resource.strings[ stringIndex ];

				StringItem& stringItem = stringItems.add();
				stringItem.sizeInBytes		= (uint32)stringData.text.getLength();
				stringItem.offsetInBlock	= 0u;

				header.stringSizeInBytes += uint32( stringData.text.getLength() + 1u );
			}
			stream.write( stringItems.getBegin(), sizeof( StringItem ) * stringItems.getCount() );

			for( uint linkIndex = 0u; linkIndex < resource.links.getCount(); ++linkIndex )
			{
				const ResourceLinkData& linkData = resource.links[ linkIndex ];

				ResourceLinkItem item;
				item.fileKey		= crcString( linkData.fileName );
				item.resourceKey	= linkData.resourceKey;
				item.resourceType	= linkData.resourceType;

				stream.write( &item, sizeof( item ) );
			}

			for( uint sectionIndex = 0u; sectionIndex < resource.sections.getCount(); ++sectionIndex )
			{
				const SectionData& sectionData = resource.sections[ sectionIndex ];

				stream.writeAlignment( sectionData.alignment );

				sectionHeaders[ sectionIndex ].offsetInResource = uint32( stream.getPosition() - header.offsetInFile );
				stream.write( sectionData.binaryData.getBegin(), sectionData.binaryData.getCount() );

				for( uint k = 0u; k < sectionData.references.getCount(); ++k )
				{
					const ResourceReferenceData& referenceData = sectionData.references[ k ];

					ReferenceItem item;
					item.type					= referenceData.key.type;
					item.targetId				= uint16( referenceData.key.identifier );
					item.offsetInTargetSection	= uint32( referenceData.key.offsetInTargetSection );
					item.offsetInSection		= uint32( referenceData.offsetInSection );

					stream.write( &item, sizeof( item ) );
				}
			}

			header.stringOffsetInResource = uint32( stream.getPosition() - header.offsetInFile );
			for( uint stringIndex = 0u; stringIndex < resource.strings.getCount(); ++stringIndex )
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
		if( fileStream.create( m_filePath.getCompletePath(), DataAccessMode_Write ) )
		{
			fileStream.write( stream.getData(), stream.getLength() );
		}
		else
		{
			TIKI_TRACE_ERROR( "[converter] Unable to open resource file '%s'!\n", m_filePath.getCompletePath() );
			return false;
		}

		return true;
	}

	void ResourceWriter::openResource( const string& name, fourcc type, const ResourceDefinition& definition, uint16 resourceFormatVersion )
	{
		TIKI_ASSERT( m_pCurrentResource == nullptr );

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
		m_pCurrentResource = nullptr;
	}

	void ResourceWriter::openDataSection( ResourceSectionWriter& sectionWriter, SectionType sectionType, uint alignment /* = TIKI_DEFAULT_ALIGNMENT */ )
	{
		TIKI_ASSERT( isPowerOfTwo( alignment ) );

		const uint id = m_pCurrentResource->sections.getCount();
		SectionData& section = m_pCurrentResource->sections.add();
		section.id				= id;
		section.type			= sectionType;
		section.alignment		= alignment;

		sectionWriter.create( this, id );
	}

	void ResourceWriter::closeDataSection( ResourceSectionWriter& sectionWriter )
	{
		TIKI_ASSERT( m_pCurrentResource != nullptr );

		SectionData& sectionData = m_pCurrentResource->sections[ sectionWriter.m_sectionId ];
		sectionData.binaryData.create( sectionWriter.m_sectionData.getData(), uint( sectionWriter.m_sectionData.getLength() ) );
		sectionData.references.create( sectionWriter.m_sectionReferences.getBegin(), sectionWriter.m_sectionReferences.getCount() );

		sectionWriter.dispose();
	}

	ReferenceKey ResourceWriter::addString( const string& text )
	{
		TIKI_ASSERT( m_pCurrentResource != nullptr );

		ReferenceKey key;
		key.type		= ReferenceType_String;
		key.identifier	= m_pCurrentResource->strings.getCount();

		StringData& data = m_pCurrentResource->strings.add();
		data.text = text;

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
}
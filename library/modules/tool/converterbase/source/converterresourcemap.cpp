
#include "tiki/converterbase/converterresourcemap.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/file.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/resource/resourcenamemapper.hpp"
#include "tiki/toolbase/list.hpp"
#include "tiki/toolbase/memorystream.hpp"

namespace tiki
{
	void ConverterResourceMap::create( const string& mapFilename )
	{
		m_fileName = mapFilename;

		if ( file::exists( mapFilename ) )
		{
			Array< uint8 > binaryData;
			file::readAllBytes( mapFilename, binaryData );

			ResourceNameMapper mapper;
			mapper.create( binaryData.getBegin() );

			const ResourceNameMapper::NameMap& map = mapper.getMap();

			for (uint i = 0u; i < map.getCount(); ++i)
			{
				const KeyValuePair< crc32, const char* >& kvp = map.getPairAt( i );
				m_map[ kvp.key ] = kvp.value;
			}

			mapper.dispose();
			binaryData.dispose();
		}
	}

	void ConverterResourceMap::dispose()
	{
		m_fileName = "";
		m_map.clear();
	}

	void ConverterResourceMap::registerResource( const string& fileName )
	{
		const crc32 crcName = crcString( fileName );
		m_map[ crcName ] = fileName;
	}

	void ConverterResourceMap::writeToFile()
	{
		Array< uint32 > crcData;
		Array< uint32 > offsetData;
		List< char > stringData;

		crcData.create( m_map.size() );
		offsetData.create( m_map.size() );

		uint index = 0u;
		for (ResourceMap::const_iterator it = m_map.begin(); it != m_map.end(); it++)
		{
			crcData[ index ]	= it->first;
			offsetData[ index ]	= uint32( stringData.getCount() );

			const string& fileName = it->second;
			stringData.addRange( fileName.cStr(), fileName.getLength() + 1u );

			index++;
		}

		MemoryStream stream;
		stream.create();

		const fourcc header			= TIKI_FOURCC( 'T', 'R', 'N', 'M' );
		const uint32 elementCount	= uint32( m_map.size() );
		const uint32 stringDataSize	= uint32( stringData.getCount() );

		stream.write( &header, sizeof( header ) );
		stream.write( &elementCount, sizeof( elementCount ) );
		stream.write( &stringDataSize, sizeof( stringDataSize ) );

		stream.write( crcData.getBegin(), sizeof( uint32 ) * crcData.getCount() );
		stream.write( offsetData.getBegin(), sizeof( uint32 ) * offsetData.getCount() );
		stream.write( stringData.getData(), sizeof( uint8 ) * stringData.getCount() );

		file::writeAllBytes(
			m_fileName,
			static_cast< const uint8* >( stream.getData() ),
			stream.getLength()
			);

		stream.dispose();
		crcData.dispose();
		offsetData.dispose();
		stringData.dispose();
	}
}

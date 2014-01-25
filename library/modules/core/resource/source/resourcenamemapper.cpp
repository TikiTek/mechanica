
#include "tiki/resource/resourcenamemapper.hpp"

#include "tiki/base/memory.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/base/fourcc.hpp"

namespace tiki
{
	struct ResourceNameMapperHeader
	{
		fourcc	header;
		uint32	elementCount;
		uint32	stringDataSize;
	};

	ResourceNameMapper::ResourceNameMapper()
	{
		m_pStringData = nullptr;
	}

	ResourceNameMapper::~ResourceNameMapper()
	{
		TIKI_ASSERT( m_pStringData == nullptr );
	}

	bool ResourceNameMapper::create( const void* pBinaryData )
	{
		const ResourceNameMapperHeader* pHeader = static_cast< const ResourceNameMapperHeader* >( pBinaryData );
		if ( pHeader->header != TIKI_FOURCC( 'T', 'R', 'N', 'M' ) )
		{
			return false;
		}
		
		const crc32* pKeys		= static_cast< const crc32* >( addPtr( pBinaryData, sizeof( ResourceNameMapperHeader ) ) );
		const uint32* pOffsets	= static_cast< const uint32* >( pKeys + pHeader->elementCount );
		const char* pStringBase	= static_cast< const char* >( static_cast< const void* >( pOffsets + pHeader->elementCount ) );

		m_pStringData = static_cast< char* >( TIKI_MEMORY_ALLOC( pHeader->stringDataSize ) );
		memory::copy( m_pStringData, pStringBase, pHeader->stringDataSize );

		m_map.create( pHeader->elementCount );

		for (uint i = 0u; i < pHeader->elementCount; ++i)
		{
			KeyValuePair< crc32, const char* >& kvp = m_map.getPairAt( i );

			kvp.key		= pKeys[ i ];
			kvp.value	= m_pStringData + pOffsets[ i ];
		}

		return true;
	}

	void ResourceNameMapper::dispose()
	{
		m_map.dispose();

		memory::freeAlign( m_pStringData );
		m_pStringData = nullptr;
	}

	const char* ResourceNameMapper::getResourceName( crc32 crcName ) const
	{
		const char* pName = nullptr;
		if ( m_map.findValue( &pName, crcName ) )
		{
			return pName;
		}

		return nullptr;
	}
}
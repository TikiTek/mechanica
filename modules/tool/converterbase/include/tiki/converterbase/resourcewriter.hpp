#pragma once
#ifndef TIKI_RESOURCEWRITER_HPP
#define TIKI_RESOURCEWRITER_HPP

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/platform.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/list.hpp"
#include "tiki/io/memorystream.hpp"
#include "tiki/resource/resourcedefinition.hpp"
#include "tiki/resource/resourcefile.hpp"

namespace tiki
{
	class ConverterBase;

	struct AllocationInfo
	{
		uint8	allocatorId;
	};

	struct ReferenceKey
	{
		ReferenceType	type;

		uint			identifier;
		uint			offsetInTargetSection;
	};

	class ResourceWriter
	{
		friend class ConverterBase;

	public:

		~ResourceWriter();

		void			openResource( const string& name, fourcc type, const ResourceDefinition& definition, uint16 resourceFormatVersion );
		void			closeResource();

		void			openDataSection( uint8 allocatorId, AllocatorType allocatorType, uint alignment = TIKI_DEFAULT_ALIGNMENT );
		void			closeDataSection();

		ReferenceKey	addString( StringType type, const string& text );
		ReferenceKey	addResourceLink( const string& fileName, crc32 resourceKey, fourcc resourceType );
		ReferenceKey	addDataPoint();

		void			writeAlignment( uint alignment );
		void			writeData( const void* pData, uint length );
		void			writeReference( const ReferenceKey* pKey );

		void			writeUInt8( uint8 value );
		void			writeUInt16( uint16 value );
		void			writeUInt32( uint32 value );
		void			writeUInt64( uint64 value );

		void			writeSInt8( sint8 value );
		void			writeSInt16( sint16 value );
		void			writeSInt32( sint32 value );
		void			writeSInt64( sint64 value );

		void			writeFloat( float value );
		void			writeDouble( double value );

		uint			getSizeOfCurrentSection() const;

	private:

		struct ReferenceData
		{
			ReferenceKey	key;
			
			uint32			position;
		};

		struct SectionData
		{
			uint					id;

			uint					alignment;
			AllocatorType			allocatorType;
			uint8					allocatorId;

			MemoryStream			binaryData;
			List< ReferenceData >	references;
		};
		
		struct StringData
		{
			StringType	type;
			string		text;
			uint		sizeInBytes;
		};

		struct ResourceLinkData
		{
			string	fileName;
			crc32	resourceKey;
			fourcc	resourceType;
		};

		struct ResourceData
		{
			fourcc						type;
			string						name;
			ResourceDefinition			definition;

			uint16						version;

			List< SectionData >			sections;
			List< StringData >			strings;
			List< ResourceLinkData >	links;
		};

		string					m_fileName;
		PlatformType			m_platform;

		ResourceData*			m_pCurrentResource;
		SectionData*			m_pCurrentSection;
		List< uint >			m_sectionStack;

		List< ResourceData >	m_resources;

		void					create( const string& fileName );
		void					dispose();

	};
}


#endif // TIKI_RESOURCEWRITER_HPP

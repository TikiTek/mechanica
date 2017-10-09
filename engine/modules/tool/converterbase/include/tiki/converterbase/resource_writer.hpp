#pragma once
#ifndef TIKI_RESOURCE_WRITER_HPP_INCLUDED
#define TIKI_RESOURCE_WRITER_HPP_INCLUDED

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/path.hpp"
#include "tiki/container/list.hpp"
#include "tiki/resource/resourcedefinition.hpp"
#include "tiki/resource/resourcefile.hpp"

namespace tiki
{
	class ConverterBase;
	class ResourceSectionWriter;

	struct ReferenceKey
	{
		ReferenceType	type;
		uint			identifier;
		uint			offsetInTargetSection;
	};

	class ResourceWriter
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( ResourceWriter );
		friend class ConverterBase;

	public:

						ResourceWriter();
						~ResourceWriter();

		void			openResource( const string& name, fourcc type, const ResourceDefinition& definition, uint16 resourceFormatVersion );
		void			closeResource();

		bool			openDataSection( ResourceSectionWriter& sectionWriter, SectionType allocatorType, uint alignment = TIKI_DEFAULT_ALIGNMENT );
		void			closeDataSection( ResourceSectionWriter& sectionWriter );

		ReferenceKey	addString( StringType type, const string& text );
		ReferenceKey	addResourceLink( const string& fileName, crc32 resourceKey, fourcc resourceType );
		ReferenceKey	addDataPoint();

	private:

		struct ReferenceData
		{
			ReferenceKey	key;
			uint32			position;
		};

		struct SectionData
		{
			uint					alignment;
			SectionType				type;

			List< uint8 >			binaryData;
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

		Path					m_filePath;
		PlatformType			m_platform;

		ResourceData*			m_pCurrentResource;
		SectionData*			m_pCurrentSection;
		List< uint >			m_sectionStack;

		List< ResourceData >	m_resources;

		void					create( const Path& filePath );
		void					dispose();

	};
}


#endif // TIKI_RESOURCE_WRITER_HPP_INCLUDED

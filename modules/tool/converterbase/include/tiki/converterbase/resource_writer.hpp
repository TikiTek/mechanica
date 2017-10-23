#pragma once
#ifndef TIKI_RESOURCE_WRITER_HPP_INCLUDED
#define TIKI_RESOURCE_WRITER_HPP_INCLUDED

#include "tiki/base/path.hpp"
#include "tiki/converterbase/resource_section_writer.hpp"

namespace tiki
{
	class ConverterBase;

	class ResourceWriter
	{
		TIKI_NONCOPYABLE_CLASS( ResourceWriter );
		friend class ConverterBase;

	public:

						ResourceWriter();
						~ResourceWriter();

		void			openResource( const string& name, fourcc type, const ResourceDefinition& definition, uint16 resourceFormatVersion );
		void			closeResource();

		void			openDataSection( ResourceSectionWriter& sectionWriter, SectionType sectionType, uint alignment = TIKI_DEFAULT_ALIGNMENT );
		void			closeDataSection( ResourceSectionWriter& sectionWriter );

		ReferenceKey	addString( const string& text );
		ReferenceKey	addResourceLink( const string& fileName, crc32 resourceKey, fourcc resourceType );

	private:

		struct SectionData
		{
			uint							id;
			uint							alignment;
			SectionType						type;

			List< uint8 >					binaryData;
			List< ResourceReferenceData >	references;
		};

		struct StringData
		{
			string	text;
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

		List< ResourceData >	m_resources;
		ResourceData*			m_pCurrentResource;

		void					create( const Path& filePath );
		void					dispose();

		bool					writeToFile();
	};
}


#endif // TIKI_RESOURCE_WRITER_HPP_INCLUDED

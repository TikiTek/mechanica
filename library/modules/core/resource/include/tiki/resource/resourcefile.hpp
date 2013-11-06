#pragma once
#ifndef __TIKI_RESOURCEFILE_HPP_INCLUDED__
#define __TIKI_RESOURCEFILE_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/base/endianness.hpp"

namespace tiki
{
	struct ResourceFileHeader
	{
		fourcc	tikiFourcc;
		uint8	version;
		uint8	resourceCount;
	};

	struct ResourceHeader
	{
		fourcc	type;
		crc32	key;

		uint8	version;
		
		uint8	sectionCount;
		uint16	stringCount;
		uint16	linkCount;

		uint32	stringSizeInBytes;
	};

	struct SectionHeader
	{
		uint8	allocatorId;
		uint8	alignment;

		uint16	referenceCount;

		uint32	sizeInBytes;
		uint32	offsetInFile;
	};

	enum ReferenceType
	{
		ReferenceType_Pointer,
		ReferenceType_String,
		ReferenceType_ResourceLink
	};

	struct ReferenceItem
	{
		uint32	type_offsetInSection; // 2 bits - type / 30 bits - offset

		uint32	offsetInTargetSection;
		uint16	targetId;
	};

	enum StringType
	{
		StringType_Char,
		StringType_WideChar,
		StringType_UTF8
	};

	struct StringItem
	{
		uint32	type_lengthModifier_textLength; // 4 bits - type / 4 bits - lengthModifier / 24 bits - textLength

		uint8	text[ 1u ];
	};

	struct ResourceLinkItem
	{
		crc32	fileKey;
		crc32	resourceKey;
	};

	namespace resource
	{
		Endianness		getEndianness( const ResourceFileHeader& header );

		const void*		getSectionData( const SectionHeader& header, const void* pBase );

		ReferenceType	getReferenceType( const ReferenceItem& item );
		uint			getReferenceOffsetInSection( const ReferenceItem& item );

		StringType		getStringType( const StringItem& item );
		uint			getStringLengthModifier( const StringItem& item );
		uint			getStringTextLength( const StringItem& item );
		uint			getStringLengthInBytes( const StringItem& item );
	}
}

#endif // __TIKI_RESOURCEFILE_HPP_INCLUDED__

#pragma once
#ifndef __TIKI_RESOURCEFILE_HPP_INCLUDED__
#define __TIKI_RESOURCEFILE_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/base/endianness.hpp"

namespace tiki
{
	// :TODO: eliminate all padding

	struct ResourceFileHeader
	{
		enum
		{
			CurrentFormatVersion = 1u
		};

		fourcc	tikiFourcc;
		uint8	version;
		uint8	resourceCount;
		// 2 bytes padding
	};

	struct ResourceHeader
	{
		fourcc	type;
		crc32	key;

		uint8	version;
		
		uint8	sectionCount;
		uint16	stringCount;
		uint16	linkCount;
		// 2 bytes padding
		uint32	stringSizeInBytes;
	};

	enum AllocatorType
	{
		AllocatorType_TemporaryMemory,
		AllocatorType_MainMemory,
		AllocatorType_GraphicsMemory
	};

	struct SectionHeader
	{
		uint8	allocatorType_allocatorId; // 2 bits - type / 6 bits - id // if type is AllocatorType_TemporaryMemory the id will be ignored
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
		// 2 bytes padding
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

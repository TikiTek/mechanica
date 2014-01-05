#pragma once
#ifndef __TIKI_RESOURCEFILE_HPP_INCLUDED__
#define __TIKI_RESOURCEFILE_HPP_INCLUDED__

#include "tiki/base/alignment.hpp"
#include "tiki/base/endianness.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	// :TODO: eliminate all padding

	struct ResourceFileHeader
	{
		enum
		{
			TikiMagicHostEndian		= TIKI_FOURCC( 'T', 'I', 'K', 'I' ),
			TikiMagicOtherEndian	= TIKI_FOURCC( 'I', 'K', 'I', 'T' ),
			CurrentFormatVersion	= 1u
		};

		fourcc	tikiFourcc;
		uint16	version;
		uint16	resourceCount;
	};

	struct ResourceHeader
	{
		fourcc	type;
		crc32	key;

		uint32	offsetInFile;

		uint16	version;
		
		uint16	sectionCount;
		uint16	stringCount;
		uint16	linkCount;

		uint32	stringOffsetInResource;
		uint32	stringSizeInBytes;
	};

	enum AllocatorType
	{
		AllocatorType_InitializaionMemory,
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
		uint32	offsetInResource;
	};

	enum ReferenceType
	{
		ReferenceType_Pointer,
		ReferenceType_String,
		ReferenceType_ResourceLink
	};

	struct ReferenceItem
	{
		uint8	type;			// see ReferenceType
		// 1 byte padding
		uint16	targetId;

		uint32	offsetInSection;
		uint32	offsetInTargetSection;
	};

	enum StringType
	{
		StringType_Char,
		StringType_WideChar,
		StringType_UTF8
	};

	struct StringItem
	{
		uint32	type_lengthModifier_textLength; // 2 bits - type / 2 bits - lengthModifier / 28 bits - textLength
		uint32	offsetInBlock;
	};

	struct ResourceLinkItem
	{
		crc32	fileKey;
		crc32	resourceKey;
		fourcc	resourceType;
	};

	template<typename T>
	struct ResRef
	{
	public:

		ResRef( const T* pRef )
			: m_pRef( pRef )
		{
		}

		ResRef( const ResRef< T >& ref )
			: m_pRef( ref.m_pRef )
		{
		}

		~ResRef()
		{
			m_pRef = nullptr;
		}

		TIKI_FORCE_INLINE const T*	getData() const { return m_pRef; }

		//T*		operator() const { return m_pRef; }
		TIKI_FORCE_INLINE const T*	operator->() const { return m_pRef; }

	private:

		TIKI_PRE_ALIGN( 8 )	struct
		{
			const T*	m_pRef;
#if TIKI_DISABLED( TIKI_BUILD_64BIT )
			void*		m_pPadding;
#endif
		} TIKI_POST_ALIGN( 8 );
	};

	namespace resource
	{
		Endianness			getEndianness( const ResourceFileHeader& header );

		const AllocatorType	getSectionAllocatorType( uint8 allocatorType_allocatorId );
		const uint8			getSectionAllocatorId( uint8 allocatorType_allocatorId );

		StringType			getStringType( const StringItem& item );
		uint				getStringLengthModifier( const StringItem& item );
		uint				getStringTextLength( const StringItem& item );
		uint				getStringLengthInBytes( const StringItem& item );
	}
}

#endif // __TIKI_RESOURCEFILE_HPP_INCLUDED__

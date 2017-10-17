#pragma once
#ifndef __TIKI_RESOURCEFILE_HPP_INCLUDED__
#define __TIKI_RESOURCEFILE_HPP_INCLUDED__

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
		fourcc				type;
		crc32				key;
		ResourceDefinition	definition;

		uint32				offsetInFile;

		uint16				version;

		uint16				sectionCount;
		uint16				stringCount;
		uint16				linkCount;

		uint32				stringOffsetInResource;
		uint32				stringSizeInBytes;
	};

	enum SectionType : uint8
	{
		SectionType_Initializaion,
		SectionType_Temporary,
		SectionType_Main
	};

	struct SectionHeader
	{
		SectionType	type;
		uint8		alignment;

		uint16		referenceCount;

		uint32		sizeInBytes;
		uint32		offsetInResource;
	};

	enum ReferenceType : uint8
	{
		ReferenceType_Pointer,
		ReferenceType_String,
		ReferenceType_ResourceLink
	};

	struct ReferenceItem
	{
		ReferenceType	type;
		// 1 byte padding
		uint16			targetId;

		uint32			offsetInSection;
		uint32			offsetInTargetSection;
	};

	struct StringItem
	{
		uint32	sizeInBytes;
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

		TIKI_FORCE_INLINE ResRef()
			: m_pRef( nullptr )
		{
		}

		TIKI_FORCE_INLINE ResRef( const T* pRef )
			: m_pRef( pRef )
		{
		}

		TIKI_FORCE_INLINE ResRef( const ResRef< T >& ref )
			: m_pRef( ref.m_pRef )
		{
		}

		TIKI_FORCE_INLINE ~ResRef()
		{
			m_pRef = nullptr;
		}

		TIKI_FORCE_INLINE const T* getData() const
		{
			return m_pRef;
		}

		TIKI_FORCE_INLINE const T* operator->() const
		{
			return m_pRef;
		}

	private:

#if TIKI_ENABLED( TIKI_BUILD_64BIT )
		const T*		m_pRef;
#else
		TIKI_ALIGN_PREFIX( 8 )	struct
		{
			const T*	m_pRef;
			size_t		m_pad0;
		} TIKI_ALIGN_POSTFIX( 8 );
#endif
	};

	template<typename T>
	struct ResArray
	{
	public:

		TIKI_FORCE_INLINE ResArray()
			: m_pData( nullptr )
			, m_size( 0u )
		{
		}

		TIKI_FORCE_INLINE ResArray( const T* pData, size_t size )
			: m_pData( pData )
			, m_size( size )
		{
		}

		TIKI_FORCE_INLINE ResArray( const ResArray< T >& copy )
			: m_pData( copy.m_pData )
			, m_size( copy.m_size )
		{
		}

		TIKI_FORCE_INLINE ~ResArray()
		{
			m_pData	= nullptr;
			m_size	= 0u;
		}

		TIKI_FORCE_INLINE size_t getCount() const
		{
			return m_size;
		}

		TIKI_FORCE_INLINE const T* getData() const
		{
			return m_pData;
		}

		TIKI_FORCE_INLINE const T& getAt( size_t index ) const
		{
			TIKI_ASSERT( index < m_size );
			return m_pData[ index ];
		}

		TIKI_FORCE_INLINE const T& operator[]( size_t index ) const
		{
			return getAt( index );
		}

	private:

#if TIKI_ENABLED( TIKI_BUILD_64BIT )
		const T*		m_pData;
		size_t			m_size;
#else
		TIKI_ALIGN_PREFIX( 8 )	struct
		{
			const T*	m_pData;
			size_t		m_pad0;
			size_t		m_size;
			size_t		m_pad1;
		} TIKI_ALIGN_POSTFIX( 8 );
#endif
	};
}

#endif // __TIKI_RESOURCEFILE_HPP_INCLUDED__

#pragma once
#ifndef TIKI_RESOURCE_FILE_HPP_INCLUDED
#define TIKI_RESOURCE_FILE_HPP_INCLUDED

#include "tiki/base/fourcc.hpp"
#include "tiki/base/types.hpp"
#include "tiki/resource/resource_definition.hpp"

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

		typedef T			Type;
		typedef T&			Reference;
		typedef const T&	ConstReference;
		typedef T*			Iterator;
		typedef const T*	ConstIterator;

		TIKI_FORCE_INLINE ResArray()
			: m_pData( nullptr )
			, m_count( 0u )
		{
		}

		TIKI_FORCE_INLINE ResArray( const T* pData, uintreg size )
			: m_pData( pData )
			, m_count( size )
		{
		}

		TIKI_FORCE_INLINE ResArray( const ResArray< T >& copy )
			: m_pData( copy.m_pData )
			, m_count( copy.m_count )
		{
		}

		TIKI_FORCE_INLINE ~ResArray()
		{
			m_pData	= nullptr;
			m_count	= 0u;
		}

		TIKI_FORCE_INLINE uintreg			getCount() const { return m_count; }
		TIKI_FORCE_INLINE uintreg			getCapacity() const { return m_count; }

		TIKI_FORCE_INLINE ConstIterator		getBegin() const { return m_pData; }
		TIKI_FORCE_INLINE ConstIterator		getEnd() const { return m_pData + m_count; }
		TIKI_FORCE_INLINE ConstReference	getFirst() const { return getAt( 0u ); }
		TIKI_FORCE_INLINE ConstReference	getLast() const { return getAt( m_count - 1u ); }

		TIKI_FORCE_INLINE const T&			getAt( uintreg index ) const { TIKI_ASSERT( index < m_count ); return m_pData[ index ]; }
		TIKI_FORCE_INLINE const T&			operator[]( uintreg index ) const { return getAt( index ); }

		TIKI_FORCE_INLINE ArrayView< T >	getView() const { return ArrayView< T >( m_pData, m_count ); }

		TIKI_FORCE_INLINE Iterator			begin() { return getBegin(); }
		TIKI_FORCE_INLINE ConstIterator		begin() const { return getBegin(); }

		TIKI_FORCE_INLINE Iterator			end() { return getEnd(); }
		TIKI_FORCE_INLINE ConstIterator		end() const { return getEnd(); }

	private:

#if TIKI_ENABLED( TIKI_BUILD_64BIT )
		const T*		m_pData;
		uintreg			m_count;
#else
		TIKI_ALIGN_PREFIX( 8 )	struct
		{
			const T*	m_pData;
			uintreg		m_pad0;
			uintreg		m_count;
			uintreg		m_pad1;
		} TIKI_ALIGN_POSTFIX( 8 );
#endif
	};
}

#endif // TIKI_RESOURCE_FILE_HPP_INCLUDED

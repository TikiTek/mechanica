#pragma once

#include "tiki/base/dynamic_string.hpp"
#include "tiki/container/list.hpp"
#include "tiki/io/memory_stream.hpp"
#include "tiki/resource/resource_file.hpp"

namespace tiki
{
	class ResourceWriter;

	struct ReferenceKey
	{
		ReferenceType	type;
		uint			identifier;
		uint			offsetInTargetSection;
	};

	struct ResourceReferenceData
	{
		ReferenceKey	key;
		uint			offsetInSection;
	};

	class ResourceSectionWriter
	{
		TIKI_NONCOPYABLE_CLASS( ResourceSectionWriter );
		friend class ResourceWriter;

	public:

						ResourceSectionWriter();
						~ResourceSectionWriter();

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

		uint			getCurrentSize() const;

		ReferenceKey	addString( const string& text );
		ReferenceKey	addResourceLink( const string& fileName, crc32 resourceKey, fourcc resourceType );
		ReferenceKey	addDataPoint();

		ResourceWriter&	getResourceWriter();

	private: // friend

		void	create( ResourceWriter*	pResourceWriter, uint sectionId );
		void	dispose();

	private:

		ResourceWriter*					m_pResourceWriter;

		uint							m_sectionId;
		MemoryStream					m_sectionData;
		List< ResourceReferenceData >	m_sectionReferences;
	};
}

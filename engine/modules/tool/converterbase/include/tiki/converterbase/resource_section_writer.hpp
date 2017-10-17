#pragma once
#ifndef TIKI_RESOURCE_SECTION_WRITER_HPP_INCLUDED
#define TIKI_RESOURCE_SECTION_WRITER_HPP_INCLUDED

#include "tiki/io/memorystream.hpp"
#include "tiki/base/dynamic_string.hpp"

namespace tiki
{
	class ResourceWriter;

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

		bool	create();
		void	dispose();

	private:

		ResourceWriter*	m_pResourceWriter;

		MemoryStream	m_dataStream;
	};
}

#endif // TIKI_RESOURCE_SECTION_WRITER_HPP_INCLUDED

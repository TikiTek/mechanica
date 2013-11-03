#pragma once
#ifndef TIKI_RESOURCEWRITER_HPP
#define TIKI_RESOURCEWRITER_HPP

#include "tiki/base/types.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/platform.hpp"
#include "tiki/toolbase/memorystream.hpp"

namespace tiki
{
	class ConverterBase;

	class ResourceWriter
	{
		friend class ConverterBase;

	public:

		~ResourceWriter();

		void			writeAlignment( size_t alignment );

		void			writeData( const void* pData, size_t length );

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

	private:

		string			m_fileName;
		MemoryStream	m_fileStream;

		fourcc			m_fourcc;
		PlatformType	m_platform;

		void			create( fourcc cc, const string& fileName, PlatformType platform = PlatformType_Win );
		void			dispose();

	};
}


#endif // TIKI_RESOURCEWRITER_HPP

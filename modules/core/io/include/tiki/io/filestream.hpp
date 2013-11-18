#pragma once
#ifndef __TIKI_FILESTREAM_HPP_INCLUDED__
#define __TIKI_FILESTREAM_HPP_INCLUDED__

#include "tiki/io/datastream.hpp"

#include "tiki/io/filesystem.hpp"

typedef void* HANDLE;

namespace tiki
{
	class FileStream : public DataStream
	{
		TIKI_NONCOPYABLE_CLASS( FileStream );

	public:

		FileStream();
		~FileStream();

		bool				open( cstring pFileName, DataAccessMode accessMode );
		virtual void		close();

		bool				isOpen() const { return m_fileHandle != nullptr; }

		virtual FileSize	read( void* pTargetData, uint bytesToRead );
		virtual FileSize	write( const void* pSourceData, uint bytesToWrite );

		virtual FileSize	getPosition();
		virtual FileSize	setPosition( FileSize position );
		virtual FileSize	seekPosition( FileSize seek );

		virtual FileSize	getLength();
		virtual void		setLength( FileSize length );

	private:

		HANDLE				m_fileHandle;

	};
}

#endif // __TIKI_FILESTREAM_HPP_INCLUDED__

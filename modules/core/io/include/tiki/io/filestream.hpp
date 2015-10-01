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

		bool				create( const char* pFileName, DataAccessMode accessMode );
		virtual void		dispose() TIKI_OVERRIDE TIKI_FINAL;

		bool				isOpen() const;

		virtual FileSize	read( void* pTargetData, FileSize bytesToRead ) const TIKI_OVERRIDE TIKI_FINAL;
		virtual FileSize	write( const void* pSourceData, FileSize bytesToWrite ) TIKI_OVERRIDE TIKI_FINAL;

		virtual FileSize	getPosition() const TIKI_OVERRIDE TIKI_FINAL;
		virtual void		setPosition( FileSize position ) TIKI_OVERRIDE TIKI_FINAL;
		virtual FileSize	seekPosition( FileOffset offset, DataStreamSeek method = DataStreamSeek_Current ) TIKI_OVERRIDE TIKI_FINAL;

		virtual FileSize	getLength() const TIKI_OVERRIDE TIKI_FINAL;
		virtual void		setLength( FileSize length ) TIKI_OVERRIDE TIKI_FINAL;

	private:

		HANDLE				m_fileHandle;

	};
}

#endif // __TIKI_FILESTREAM_HPP_INCLUDED__

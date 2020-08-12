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

		bool				open( const char* pFileName, DataAccessMode accessMode );
		virtual void		close() TIKI_OVERRIDE;

		bool				isOpen() const;

		virtual FileSize	read( void* pTargetData, uint bytesToRead ) TIKI_OVERRIDE;
		virtual FileSize	write( const void* pSourceData, uint bytesToWrite ) TIKI_OVERRIDE;

		virtual FileSize	getPosition() TIKI_OVERRIDE;
		virtual void		setPosition( FileSize position ) TIKI_OVERRIDE;
		virtual FileSize	seekPosition( FileSize seek ) TIKI_OVERRIDE;

		virtual FileSize	getLength() const TIKI_OVERRIDE;
		virtual void		setLength( FileSize length ) TIKI_OVERRIDE;

	private:

		HANDLE				m_fileHandle;

	};
}

#endif // __TIKI_FILESTREAM_HPP_INCLUDED__

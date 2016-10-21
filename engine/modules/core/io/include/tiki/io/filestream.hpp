#pragma once
#ifndef __TIKI_FILESTREAM_HPP_INCLUDED__
#define __TIKI_FILESTREAM_HPP_INCLUDED__

#include "tiki/io/datastream.hpp"

#include "tiki/io/filesystem.hpp"

#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
#	include "../../../source/win/platformdata_win.hpp"
#elif TIKI_ENABLED( TIKI_PLATFORM_LINUX )
#	include "../../../source/posix/platformdata_posix.hpp"
#else
#	error not supported
#endif

namespace tiki
{
	class FileStream : public DataStream
	{
		TIKI_NONCOPYABLE_CLASS( FileStream );

	public:

		FileStream();
		~FileStream();

		bool				create( const char* pFileName, DataAccessMode accessMode );
		virtual void		dispose() TIKI_OVERRIDE_FINAL;

		bool				isOpen() const;

		virtual FileSize	read( void* pTargetData, FileSize bytesToRead ) const TIKI_OVERRIDE_FINAL;
		virtual FileSize	write( const void* pSourceData, FileSize bytesToWrite ) TIKI_OVERRIDE_FINAL;

		virtual FileSize	getPosition() const TIKI_OVERRIDE_FINAL;
		virtual void		setPosition( FileSize position ) TIKI_OVERRIDE_FINAL;
		virtual FileSize	seekPosition( FileOffset offset, DataStreamSeek method = DataStreamSeek_Current ) TIKI_OVERRIDE_FINAL;

		virtual FileSize	getLength() const TIKI_OVERRIDE_FINAL;
		virtual void		setLength( FileSize length ) TIKI_OVERRIDE_FINAL;

	private:

		FileStreamPlatformData	m_platformData;

	};
}

#endif // __TIKI_FILESTREAM_HPP_INCLUDED__
